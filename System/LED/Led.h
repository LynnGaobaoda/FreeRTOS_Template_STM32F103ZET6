
#ifndef _LED_H_
#define _LED_H_

#include "Sys.h"
#define LED_ON 							1
#define LED_OFF 						0
#define GPIOx_def_led 					GPIOB	//GPIO B 端口
#define GPIO_Pin_x_def_led 				GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5	//led引脚 pin 0
#define RCC_APB2Periph_GPIOx_def_led	RCC_APB2Periph_GPIOB	//ledGPIO B 对应时钟

#define LEDg_PB0 PBout(0)
#define LEDb_PB1 PBout(1)
#define LEDr_PB5 PBout(5)

void led_init(void);

#endif

