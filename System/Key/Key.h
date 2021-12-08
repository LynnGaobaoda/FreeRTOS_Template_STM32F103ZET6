#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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


#define KEY1  	GPIO_ReadInputDataBit(GPIOx_def_key1,GPIO_Pin_x_def_key1) //��ȡ����1
#define KEY2  	GPIO_ReadInputDataBit(GPIOx_def_key2,GPIO_Pin_x_def_key2)//��ȡ����2

#define KEY1_PRES 	 1	//KEY1����
#define KEY2_PRES	 2	//KEY2����

void 	key_init(void);//IO��ʼ��
u8 		key_scan(u8 mode);  	//����ɨ�躯��					    
#endif
