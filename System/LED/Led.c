#include "stm32f10x.h"                  // Device header
#include "Led.h"


void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x_def_led;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_def_led,ENABLE);
	
	GPIO_Init(GPIOx_def_led,&GPIO_InitStructure);	
}


