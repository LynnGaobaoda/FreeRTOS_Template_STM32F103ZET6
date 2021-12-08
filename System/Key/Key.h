#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
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

#define RCC_APB2Periph_GPIOx_def_key1 RCC_APB2Periph_GPIOA
#define RCC_APB2Periph_GPIOx_def_key2 RCC_APB2Periph_GPIOC

#define GPIOx_def_key1 GPIOA 
#define GPIOx_def_key2 GPIOC

#define GPIO_Pin_x_def_key1 GPIO_Pin_0
#define GPIO_Pin_x_def_key2 GPIO_Pin_13

//#define KEY1 PAin(0)   	//PE4
//#define KEY2 PCin(13)	//PE3 


#define KEY1  	GPIO_ReadInputDataBit(GPIOx_def_key1,GPIO_Pin_x_def_key1) //读取按键1
#define KEY2  	GPIO_ReadInputDataBit(GPIOx_def_key2,GPIO_Pin_x_def_key2)//读取按键2

#define KEY1_PRES 	 1	//KEY1按下
#define KEY2_PRES	 2	//KEY2按下

void 	key_init(void);//IO初始化
u8 		key_scan(u8 mode);  	//按键扫描函数					    
#endif
