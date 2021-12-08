#ifndef _I2C_H_
#define _I2C_H_
#include "stm32f10x.h"                  // Device header
#include "Sys.h"
//���Ŷ���
//SCL :PA_5
//SDL :PA_7
//PA2:SDL  
//PA3:SCL
//-----------------IIC�˿ڶ���----------------  
//ʹ��λ��
#define I2C_SCL PAout(3)   
#define I2C_SDA PAout(2)
#define I2C_SDA_IN PAin(2)	

#define RCC_APB2Periph_GPIOx_def_i2c 	RCC_APB2Periph_GPIOA
#define GPIOx_def_i2c					GPIOA
#define GPIO_Pin_x_def_i2c			GPIO_Pin_2|GPIO_Pin_3	//PA2:SDL  PA3:SCL


//CRL32/4=P0-P7��8������
//CRH32/4=P0-P7��8������
//����GPIOB->CRL[31:28] GPIOB_Pin_7 1000:����/�������� 0111:��©���50MHz
//������ֻ��Ҫ�޸�GPIOx,CRx 0xFFFFFFFF ��ĳһ��FΪ0,���һ��F��ʾPin7\Pin15
#define SDA_IN()  {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=(u32)8<<8;}
#define SDA_OUT() {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=(u32)3<<8;}

void 			I2c_Init(void); //I2C_Init �⺯��ռ��
void 			I2C_Start(void);
void 			I2C_Stop(void);
void 			I2C_SendByte(unsigned char Byte);
unsigned char 	I2C_ReceiveByte(void);
void 			I2C_SendAck(unsigned char AckBit);
unsigned char 	I2C_ReceiveAck(void);

#endif
