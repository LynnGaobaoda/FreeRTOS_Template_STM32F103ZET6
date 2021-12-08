#include "I2C.h"
#include "sys.h"
#include "delay.h"
/**
	I2C����ͨ�Ŵ���Э�飬1ʱ����+1������
	��Ƭ����ʱ���ź�1us��500Hz �㹻I2Cʱ��ת��������ʱ ���ù�holdTime��
	����������������Ҫ�� GPIO ����Ϊ��������
	���ߣ�Ching
  */


void I2c_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_def_i2c, ENABLE);	//ʹ��A�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin 	=	GPIO_Pin_x_def_i2c;	 	//����ѡ��
 	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_Out_PP; 		 	//�������
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;			//�ٶ�50MHz
	
 	GPIO_Init(GPIOx_def_i2c, &GPIO_InitStructure);	  				//��ʼ��GPIODx,
 	GPIO_SetBits(GPIOx_def_i2c,GPIO_Pin_x_def_i2c);
}


/**
  *	@brief	I2C ��ʼλ ʱ��
  *	@param	��	
  *	@retval	��
  */
void I2C_Start(void)
{
	SDA_OUT();     //sda�����
	I2C_SDA=1;
	I2C_SCL=1;
	delay_us(4);	
	I2C_SDA=0;	
	delay_us(4);
	I2C_SCL=0;
}

/**
  *	@brief	I2C ֹͣλ ʱ��
  *	@param	��	
  *	@retval	��
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
  *	@brief	I2C ��������λ ʱ�� �ȸ�λ���λ
  *	@param	Byte ���͵�����һ���ֽ�
  *	@retval	��
  */
void I2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	SDA_OUT();
	I2C_SCL=0;
	for(i=0;i<8;i++){
		//I2C_SDA=Byte&(0x80>>i); ****** C51 �÷���
		//********* λ�� ��������һ�� ���ֽڡ���1Byte = 0x01;  IO�ڲ�Ϊ1
		//*********Byte&(0x80>>i) != 0x01
		//*********Byte&(0x80>>i) = 0x80 ��0x00 �߼���Ϊ 1 ������ֱ��λ��
		///********��ȷд������:
		I2C_SDA=Byte&(0x80>>i)?1:0; //��Byte&(0x80>>i)?0x01:0x00;
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		I2C_SCL=1;
		delay_us(2); 
		I2C_SCL=0;
		delay_us(2); 
	}
}	

/**
  *	@brief	I2C ��������λ ʱ�� �ȸ�λ���λ
  *	@param	��
  *	@retval	���յ���һ���ֽ�
  */
unsigned char I2C_ReceiveByte(void)
{	
	unsigned char i, Byte=0x00;
	I2C_SDA=1;
	SDA_IN();//SDA����Ϊ����
	
	for(i=0;i<8;i++){	
		I2C_SCL=1;	//���߶�ȡSDA
		if(I2C_SDA_IN){Byte |=(0x80>>i);}
		delay_us(1);
		I2C_SCL=0;	//�½����µ������Զ�����
		delay_us(2);
	}
	return Byte;	
}

/**
  *	@brief	I2C ����Ӧ�� ʱ��
  *	@param	AckBit �Ƿ�Ӧ�� 0 Ӧ��1��Ӧ��
  *	@retval	��
  */
void I2C_SendAck(unsigned char AckBit)
{//bit51��Ƭ��������������
	I2C_SCL=0;
	SDA_OUT();
	I2C_SDA=AckBit;
	delay_us(2);
	I2C_SCL=1;
	delay_us(2);
	I2C_SCL=0;
}

/**
  *	@brief	I2C ����Ӧ�� ʱ��
  *	@param	��
  *	@retval	�Ƿ�Ӧ�� 0���ӻ��յ����ݣ�Ӧ��. 1 �ӻ���Ӧ��
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

