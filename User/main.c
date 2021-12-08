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
	OLED_ShowString(0,0,"Lynn & Z, + M",16);
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
		printf("zjm值:%.4f\r\n", float_num);  //浮点占内存，将堆栈调大一些即可
		//printf("zjm值:\r\n");
		delay_ms(1000);
	}
}


