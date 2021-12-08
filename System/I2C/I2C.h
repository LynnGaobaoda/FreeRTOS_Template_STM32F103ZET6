#ifndef _I2C_H_
#define _I2C_H_
#include "stm32f10x.h"                  // Device header
#include "Sys.h"
//引脚定义
//SCL :PA_5
//SDL :PA_7
//PA2:SDL  
//PA3:SCL
//-----------------IIC端口定义----------------  
//使用位带
#define I2C_SCL PAout(3)   
#define I2C_SDA PAout(2)
#define I2C_SDA_IN PAin(2)	

#define RCC_APB2Periph_GPIOx_def_i2c 	RCC_APB2Periph_GPIOA
#define GPIOx_def_i2c					GPIOA
#define GPIO_Pin_x_def_i2c			GPIO_Pin_2|GPIO_Pin_3	//PA2:SDL  PA3:SCL


//CRL32/4=P0-P7低8个引脚
//CRH32/4=P0-P7高8个引脚
//例：GPIOB->CRL[31:28] GPIOB_Pin_7 1000:上拉/下拉输入 0111:开漏输出50MHz
//换引脚只需要修改GPIOx,CRx 0xFFFFFFFF 的某一个F为0,如第一个F表示Pin7\Pin15
#define SDA_IN()  {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=(u32)8<<8;}
#define SDA_OUT() {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=(u32)3<<8;}

void 			I2c_Init(void); //I2C_Init 库函数占用
void 			I2C_Start(void);
void 			I2C_Stop(void);
void 			I2C_SendByte(unsigned char Byte);
unsigned char 	I2C_ReceiveByte(void);
void 			I2C_SendAck(unsigned char AckBit);
unsigned char 	I2C_ReceiveAck(void);

#endif
