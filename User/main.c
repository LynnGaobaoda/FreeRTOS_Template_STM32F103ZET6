#include "stm32f10x.h"                  // Device header

#include "FreeRTOS.h"
#include "task.h"

#include "delay.h"
#include "Key.h"
#include "led.h"
#include "sys.h"
#include "oled.h"
#include "usart.h"

#define 		START_TASK_PRIO  	1 				//�������� ���ȼ�
#define 		START_STK_SIZE 		128 			//�������� ��ջ��С
TaskHandle_t 	StartTask_Handler; 					//����������
void 			start_task(void *pvParameters); 	//����������

#define 		LED0_TASK_PRIO 		2 				//LED0���ȼ�
#define 		LED0_STK_SIZE 		50 				//LED0��ջ��С
TaskHandle_t 	LED0Task_Handler; 					//LED0���
void 			led0_task(void *p_arg); 			//LED0����

#define 		OLED_TASK_PRIO 		3 				//OLED���ȼ�
#define 		OLED_STK_SIZE 		128 			//OLED��ջ��С
TaskHandle_t 	OLEDTask_Handler; 					//OLED���
void 			OLED_task(void *p_arg); 

#define 		UART1_TASK_PRIO 	4 				//UART1���ȼ�
#define 		UART1_STK_SIZE 		128 			//UART1��ջ��С
TaskHandle_t 	UART1Task_Handler; 					//UART1���
void 			UART1_task(void *p_arg); 


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init();
	
	led_init();	//RGB: PB0 PB1 PB5
	key_init();	//PA0
	uart_init(9600);
	OLED_Init();// PA2:SDL  PA3:SCL
	printf("aaa");
	

	//������ʼ����
    xTaskCreate(
		(TaskFunction_t )	start_task, 		//������ *
		(const char* )		"start_task", 		//��������
		(uint16_t )			START_STK_SIZE, 	//�����ջ��С *
		(void* )			NULL, 				//���ݸ��������Ĳ���
		(UBaseType_t )		START_TASK_PRIO, 	//�������ȼ� *
		(TaskHandle_t* )	&StartTask_Handler 	//������ *
	); 
		
    vTaskStartScheduler(); //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); //�����ٽ���
	//����LED0 ����
	xTaskCreate(
		(TaskFunction_t )	led0_task,
		(const char* )		"led0_task",
		(uint16_t )			LED0_STK_SIZE,
		(void* )			NULL,
		(UBaseType_t )		LED0_TASK_PRIO,
		(TaskHandle_t* )	&LED0Task_Handler
	);
    //����OLED ����
    xTaskCreate(
		(TaskFunction_t )	OLED_task,
		(const char* )		"OLED_task",
		(uint16_t )			OLED_STK_SIZE,
		(void* )			NULL,
		(UBaseType_t )		OLED_TASK_PRIO,
		(TaskHandle_t* )	&OLEDTask_Handler
	);
	//����UART����
    xTaskCreate(
		(TaskFunction_t )	UART1_task,
		(const char* )		"UART1_task",
		(uint16_t )			UART1_STK_SIZE,
		(void* )			NULL,
		(UBaseType_t )		UART1_TASK_PRIO,
		(TaskHandle_t* )	&UART1Task_Handler
	);
		
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL(); //�˳��ٽ���
	
}
//LED0 ������
void led0_task(void *p_arg)
{
	LEDg_PB0=LEDr_PB5=LEDb_PB1=1;
    while(1){
        LEDr_PB5=~LEDr_PB5;
        delay_ms(500);		
    }
}
//LED1 ������
void OLED_task(void *p_arg)
{
	uint32_t count = 0;
	
	OLED_Clear();
	OLED_ShowString(0,0,"Lynn & Z, + M",16);
	while(1){ 
		OLED_ShowNum(0,2,count++,6,16);
		//LEDb_PB1=~LEDb_PB1;
		delay_ms(500);
	}
}

//UART1 ������
void UART1_task(void *p_arg)
{
	static float float_num = 0.00;
	while(1){ 	
		float_num += 0.01f;
		//LEDg_PB0=~LEDg_PB0;
		printf("zjmֵ:%.4f\r\n", float_num);  //����ռ�ڴ棬����ջ����һЩ����
		//printf("zjmֵ:\r\n");
		delay_ms(1000);
	}
}


