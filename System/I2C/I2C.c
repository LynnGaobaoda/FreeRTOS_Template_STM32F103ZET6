#include "I2C.h"
#include "sys.h"
#include "delay.h"
/**
	I2C串行通信传输协议，1时钟线+1数据线
	单片机的时钟信号1us，500Hz 足够I2C时序翻转，不用延时 不用管holdTime等
	！！！接收数据需要将 GPIO 设置为浮空输入
	作者：Ching
  */


void I2c_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_def_i2c, ENABLE);	//使能A端口时钟
	
	GPIO_InitStructure.GPIO_Pin 	=	GPIO_Pin_x_def_i2c;	 	//引脚选择
 	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_Out_PP; 		 	//推挽输出
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;			//速度50MHz
	
 	GPIO_Init(GPIOx_def_i2c, &GPIO_InitStructure);	  				//初始化GPIODx,
 	GPIO_SetBits(GPIOx_def_i2c,GPIO_Pin_x_def_i2c);
}


/**
  *	@brief	I2C 起始位 时序
  *	@param	无	
  *	@retval	无
  */
void I2C_Start(void)
{
	SDA_OUT();     //sda线输出
	I2C_SDA=1;
	I2C_SCL=1;
	delay_us(4);	
	I2C_SDA=0;	
	delay_us(4);
	I2C_SCL=0;
}

/**
  *	@brief	I2C 停止位 时序
  *	@param	无	
  *	@retval	无
  */
void I2C_Stop(void)
{
	SDA_OUT();
	I2C_SCL=0;
	I2C_SDA=0;
	delay_us(4);
	I2C_SCL=1;
	I2C_SDA=1;
	delay_us(4);
}

/**
  *	@brief	I2C 发送数据位 时序 先高位后低位
  *	@param	Byte 发送的数据一个字节
  *	@retval	无
  */
void I2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	SDA_OUT();
	I2C_SCL=0;
	for(i=0;i<8;i++){
		//I2C_SDA=Byte&(0x80>>i); ****** C51 用法：
		//********* 位带 操作的是一个 《字节》，1Byte = 0x01;  IO口才为1
		//*********Byte&(0x80>>i) != 0x01
		//*********Byte&(0x80>>i) = 0x80 或0x00 逻辑上为 1 但不能直接位带
		///********正确写法如下:
		I2C_SDA=Byte&(0x80>>i)?1:0; //或Byte&(0x80>>i)?0x01:0x00;
		delay_us(2);   //对TEA5767这三个延时都是必须的
		I2C_SCL=1;
		delay_us(2); 
		I2C_SCL=0;
		delay_us(2); 
	}
}	

/**
  *	@brief	I2C 接收数据位 时序 先高位后低位
  *	@param	无
  *	@retval	接收到的一个字节
  */
unsigned char I2C_ReceiveByte(void)
{	
	unsigned char i, Byte=0x00;
	I2C_SDA=1;
	SDA_IN();//SDA设置为输入
	
	for(i=0;i<8;i++){	
		I2C_SCL=1;	//拉高读取SDA
		if(I2C_SDA_IN){Byte |=(0x80>>i);}
		delay_us(1);
		I2C_SCL=0;	//下降沿新的数据自动跟新
		delay_us(2);
	}
	return Byte;	
}

/**
  *	@brief	I2C 发送应答 时序
  *	@param	AckBit 是否应答 0 应答，1非应答
  *	@retval	无
  */
void I2C_SendAck(unsigned char AckBit)
{//bit51单片机特有数据类型
	I2C_SCL=0;
	SDA_OUT();
	I2C_SDA=AckBit;
	delay_us(2);
	I2C_SCL=1;
	delay_us(2);
	I2C_SCL=0;
}

/**
  *	@brief	I2C 接收应答 时序
  *	@param	无
  *	@retval	是否应答 0，从机收到数据，应答. 1 从机不应答
  */	
unsigned char I2C_ReceiveAck(void)
{
	unsigned char AckBit;
	I2C_SDA=1;
	SDA_IN();
	
	delay_us(1);
	I2C_SCL=1;delay_us(2);
	AckBit=I2C_SDA_IN;
	I2C_SCL=0;
	return AckBit;
}

