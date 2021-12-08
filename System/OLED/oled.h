#ifndef __OLED_H
#define __OLED_H	

#include "stm32f10x.h"                  // Device header
#include "sys.h"

#define OLED_MODE 		1		//反显白 1:不反显  0:反显
#define SIZE 			8
//#define XLevelL		0x00
//#define XLevelH		0x10
#define Max_Column		128
#define Max_Row			64
#define	Brightness		0xFF 	//显示全白，QLED_On() 参数
#define X_WIDTH 		128
#define Y_WIDTH 		64	    						  

 		     
#define OLED_CMD  	0	//写命令
#define OLED_DATA 	1	//写数据


/******************************************************
				*****用户API主要接口*****
*******************************************************/				
//OLED控制用函数
void OLED_Init(void);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Clear(void);
void OLED_On(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);	
 

//void OLED_DrawPoint(u8 x,u8 y,u8 t);
//void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
//void Picture(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);

/*正常读写字节*/
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);

/*反显白*/
void OLED_WR_Byte_2(unsigned dat,unsigned cmd);
void Write_IIC_Data_2(unsigned char IIC_Data);	//修改 写入数据下的 发送I2C帧的第三个字节的像素数据
void Write_IIC_Byte_2(unsigned char IIC_Byte);	//为发送 反显白的数据 的底层代码


#endif  
	 

//#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SCL
//#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)
//#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA
//#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)


