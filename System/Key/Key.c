#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数
void key_init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOx_def_key1| \
							RCC_APB2Periph_GPIOx_def_key2, \
							ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_x_def_key1;//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
 	GPIO_Init(GPIOx_def_key1, &GPIO_InitStructure);//

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_x_def_key2;//KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
 	GPIO_Init(GPIOx_def_key2, &GPIO_InitStructure);//

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY1按下
//2，KEY2按下
//注意此函数有响应优先级,KEY1>KEY2
u8 key_scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==1||KEY2==1)) //若是上拉输入，key 输入应该为0
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==1)			return KEY1_PRES;
		else if(KEY2==1)	return KEY2_PRES; //KEY1 >KEY2
		
	}
	else if(KEY1==0&&KEY2==0)
		key_up=1; 	    
 	return 0;// 无按键按下
}
