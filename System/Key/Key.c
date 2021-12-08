#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
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
								    
//������ʼ������
void key_init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOx_def_key1| \
							RCC_APB2Periph_GPIOx_def_key2, \
							ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_x_def_key1;//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOx_def_key1, &GPIO_InitStructure);//

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_x_def_key2;//KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOx_def_key2, &GPIO_InitStructure);//

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY1����
//2��KEY2����
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2
u8 key_scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==1||KEY2==1)) //�����������룬key ����Ӧ��Ϊ0
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==1)			return KEY1_PRES;
		else if(KEY2==1)	return KEY2_PRES; //KEY1 >KEY2
		
	}
	else if(KEY1==0&&KEY2==0)
		key_up=1; 	    
 	return 0;// �ް�������
}
