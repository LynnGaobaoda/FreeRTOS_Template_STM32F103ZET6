# STM32移植FreeRTOS：

主要是正点原子的delay、Sys文件、串口移植，使其支持FreeRTOS
硬件基于STM32F103ZET6 不过其他系列如F407、F429等也通用
工程模板在最后
**最主要的目的是使得delay_ms()能够替换FreeRTOS的vTaskDelay()以便兼容我们自己的代码(因为大量用到delay函数，不可能全部替换成vTaskDelay()**
<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

# 下载FreeRTOS源码：
官网链接: [https://www.freertos.org/a00104.html](https://www.freertos.org/a00104.html).
![在这里插入图片描述](https://img-blog.csdnimg.cn/e6eeadc74d584a8c863584e195772f50.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBA6IiS5YWL5byA6aOe5py65YG35rih6ZW_5rGf5aSn5qGl,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)
点击第一个，下载解压
算了，不废话，移植就是将**FreeRTOSv202111.00/FreeRTOS/Source/**下的源码复制并添加到自己的工程模板下其中需要复制的源码(以STMF1x系列为例：
直接复制所有Source下的所有文件到Keil中
然后protable只保留Keil、RVDS、MemMang文件夹
至于为啥是这些，不赘述了，资料很多，我这主要是怎么改配置
还有一个要点是从demo文件下复制一个 FreeRTOSConfig.h 到Keil中去

# 配置修改：
因为正点原子的源代码主要是针对UCOS来设置的，不过为了兼容其他OS改起来也很简单
**正点原子sys.h文件修改 宏 SYSTEM_SUPPORT_OS 为 1**
```c
#define SYSTEM_SUPPORT_OS		1		//定义系统文件夹是否支持UCOS
```
**中断文件 stm32f10x_it.c 注释三个中断函数**
FreeROTS和delay.c要用到这几个中断，防止重定义，注释掉
//void SVC_Handler(void)//FreeRTOS中重定义
//{
//}

//void PendSV_Handler(void)//FreeRTOS中重定义
//{
//}

//void SysTick_Handler(void) //delay.c重写使得delay替换vTaskDelay
//{
//}
**usart.c注释掉OSIntEnter()  OSIntExit();**
因为这是UCOS进入中断的函数，FreeRTOS不需要
```c
void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	//OSIntEnter();    //FreeRTOS 不需要
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	// OSIntExit();  	//FreeRTOS 不需要										 
#endif
} 
#endif
```
**delay.c中**
和UCOSⅡ有关的宏定义全部注释掉，怕弄错就别管了，这不重要，重要的是将用到的UCOS函数替换成FreeRTOS的
中断函数修改如下注释掉UCOS的代码改成FreeRTOS的，自己理解。
```c
//systick中断服务函数,使用ucos时用到
extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{	
//	if(delay_osrunning==1)						//OS开始跑了,才执行正常的调度处理
//	{
//		OSIntEnter();							//进入中断
//		OSTimeTick();       					//调用ucos的时钟服务程序               
//		OSIntExit();       	 					//触发任务切换软中断
//	}
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//FreeROTS系统已经运行
    {
        xPortSysTickHandler();
    }
}
```
delay_init()函数
注释掉的代码都是原来的UCOS的新加的FreeROTS在其后，主要是修改SysTick时钟频率，因为FreeRTOS用的是HSE时钟作为滴答时钟
```c
//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()
{
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	u32 reload;
#endif
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	//fac_us=SystemCoreClock/8000000; //为系统时钟的1/8 
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//FreeRTOS选择外部时钟  HCLK					
	fac_us=SystemCoreClock/1000000;	
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	////ucos
//	reload=SystemCoreClock/8000000;				//每秒钟的计数次数 单位为K	   
//	reload*=1000000/delay_ostickspersec;		//根据delay_ostickspersec设定溢出时间
//												//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
//	fac_ms=1000/delay_ostickspersec;			//代表OS可以延时的最少单位	   

//	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
//	SysTick->LOAD=reload; 						//每1/delay_ostickspersec秒中断一次	
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK  
	
	////RTOS
	reload=SystemCoreClock/1000000; //每秒钟的计数次数 单位为M
    reload*=1000000/configTICK_RATE_HZ; //根据configTICK_RATE_HZ 设定溢出
    //时间reload 为24 位寄存器,最大值:
    //16777216,在72M 下,约合0.233s 左右
    fac_ms=1000/configTICK_RATE_HZ; //代表OS 可以延时的最少单位
    SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk; //开启SYSTICK 中断
    SysTick->LOAD=reload; //每1/configTICK_RATE_HZ 秒中断 一次
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK

#else
	fac_ms=(u16)fac_us*1000;					//非OS下,代表每个ms需要的systick时钟数   
#endif
}
```
**delay_ms() delay_us() delay_xms()**
自己看注释掉的和新加的都有解释
```c
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*fac_us; 							//需要的节拍数	
	//普通延时FreeRTOS无法调度 不用设置阻止调度	
	//delay_osschedlock();						//阻止OS调度，防止打断us延时 属于UCOS us 
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//时间超过/等于要延迟的时间,则退出.
		}  
	};
	//delay_osschedunlock();						//恢复OS调度	 属于UCOS						    
}
//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{	
//	if(delay_osrunning&&delay_osintnesting==0)	//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//Free RTOS系统已经运行    
	{		 
		if(nms>=fac_ms)							//延时的时间大于OS的最少时间周期 
		{ 
//   			delay_ostimedly(nms/fac_ms);		//OS延时
			 vTaskDelay(nms/fac_ms); //FreeRTOS 延时 不会阻止调度
		}
		nms%=fac_ms;							//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(nms*1000));					//普通方式延时  
}

//延时nms,不会引起任务调度
//nms:要延时的ms 数
void delay_xms(u32 nms)
{
    u32 i;
    for(i=0;i<nms;i++) delay_us(1000);
}

#else //不用OS时
```
**！！！重点来了**
很多教程说的大部分都不够全面，我也是一直Debug，查资料，综合总结了许多FreeRTOS配置文件的一些问题
首先编译时，各种显示 undifend 这个相信能解决，在FreeRTOSconfig.h和FreeRTOS.h 中所搜一下相关宏定义 并由0改为 1即可 
**最后我们还要再FreeRTOSConfig.h中末尾添加以下宏定义
xPortPendSVHandler	 、vPortSVCHandler		因为直接从Demo复制过来的配置文件没有这个两个宏，导致程序无法运行，这个编译阶段不报错，真的脑壳疼**
```c
/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

#define xPortPendSVHandler	PendSV_Handler
#define vPortSVCHandler		SVC_Handler
#endif /* FREERTOS_CONFIG_H */
```
**还有一点就是如果创建任务时如果用到浮点运算，堆栈内存尽量分配的大一些，否则，内存溢出，导致程序崩溃**

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

# 简单案例：
代码结构相当清晰，自己稍加修改即可，我也会将这个工程模板推送到github主页上，直接下载后就可以用。在本文最后由github的仓库链接
```c
#include "stm32f10x.h"                  // Device header

#include "FreeRTOS.h"
#include "task.h"

#include "delay.h"
#include "Key.h"
#include "led.h"
#include "sys.h"
#include "oled.h"
#include "usart.h"

#define 		START_TASK_PRIO  	1 				//创建任务 优先级
#define 		START_STK_SIZE 		128 			//创建任务 堆栈大小
TaskHandle_t 	StartTask_Handler; 					//创建任务句柄
void 			start_task(void *pvParameters); 	//创建任务函数

#define 		LED0_TASK_PRIO 		2 				//LED0优先级
#define 		LED0_STK_SIZE 		50 				//LED0堆栈大小
TaskHandle_t 	LED0Task_Handler; 					//LED0句柄
void 			led0_task(void *p_arg); 			//LED0函数

#define 		OLED_TASK_PRIO 		3 				//OLED优先级
#define 		OLED_STK_SIZE 		128 			//OLED堆栈大小
TaskHandle_t 	OLEDTask_Handler; 					//OLED句柄
void 			OLED_task(void *p_arg); 

#define 		UART1_TASK_PRIO 	4 				//UART1优先级
#define 		UART1_STK_SIZE 		128 			//UART1堆栈大小
TaskHandle_t 	UART1Task_Handler; 					//UART1句柄
void 			UART1_task(void *p_arg); 


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init();
	
	led_init();	//RGB: PB0 PB1 PB5
	key_init();	//PA0
	uart_init(9600);
	OLED_Init();// PA2:SDL  PA3:SCL
	printf("aaa");
	

	//创建开始任务
    xTaskCreate(
		(TaskFunction_t )	start_task, 		//任务函数 *
		(const char* )		"start_task", 		//任务名称
		(uint16_t )			START_STK_SIZE, 	//任务堆栈大小 *
		(void* )			NULL, 				//传递给任务函数的参数
		(UBaseType_t )		START_TASK_PRIO, 	//任务优先级 *
		(TaskHandle_t* )	&StartTask_Handler 	//任务句柄 *
	); 
		
    vTaskStartScheduler(); //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); //进入临界区
	//创建LED0 任务
	xTaskCreate(
		(TaskFunction_t )	led0_task,
		(const char* )		"led0_task",
		(uint16_t )			LED0_STK_SIZE,
		(void* )			NULL,
		(UBaseType_t )		LED0_TASK_PRIO,
		(TaskHandle_t* )	&LED0Task_Handler
	);
    //创建OLED 任务
    xTaskCreate(
		(TaskFunction_t )	OLED_task,
		(const char* )		"OLED_task",
		(uint16_t )			OLED_STK_SIZE,
		(void* )			NULL,
		(UBaseType_t )		OLED_TASK_PRIO,
		(TaskHandle_t* )	&OLEDTask_Handler
	);
	//创建UART任务
    xTaskCreate(
		(TaskFunction_t )	UART1_task,
		(const char* )		"UART1_task",
		(uint16_t )			UART1_STK_SIZE,
		(void* )			NULL,
		(UBaseType_t )		UART1_TASK_PRIO,
		(TaskHandle_t* )	&UART1Task_Handler
	);
		
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL(); //退出临界区
	
}
//LED0 任务函数
void led0_task(void *p_arg)
{
	LEDg_PB0=LEDr_PB5=LEDb_PB1=1;
    while(1){
        LEDr_PB5=~LEDr_PB5;
        delay_ms(500);		
    }
}
//LED1 任务函数
void OLED_task(void *p_arg)
{
	uint32_t count = 0;
	
	OLED_Clear();
	OLED_ShowString(0,0,"hhh_Lynn",16);
	while(1){ 
		OLED_ShowNum(0,2,count++,6,16);
		//LEDb_PB1=~LEDb_PB1;
		delay_ms(500);
	}
}

//UART1 任务函数
void UART1_task(void *p_arg)
{
	static float float_num = 0.00;
	while(1){ 	
		float_num += 0.01f;
		//LEDg_PB0=~LEDg_PB0;
		printf("浮点运算值:%.4f\r\n", float_num);  //浮点占内存，将堆栈调大一些即可
		//printf("zjm值:\r\n");
		delay_ms(1000);
	}
}

```

# FreeRTOS STM32F103ZET6 工程模板

github链接: [https://github.com/LynnGaobaoda/FreeRTOS_Template_STM32F103ZET6](https://github.com/LynnGaobaoda/FreeRTOS_Template_STM32F103ZET6).
