#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "adc.h"
u8 reverse=1;
float adc[6];
  u16 Res; 	
 int main(void)
 { 
	u8 sign=0;
	u8 len,t;
	u16 adcx,times=0;
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	LCD_Init();
 	Adc_Init();		  		//ADC��ʼ��	    
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"Mini STM32");	
	LCD_ShowString(60,70,200,16,16,"ADC TEST");	
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(60,90,200,16,16,"your messege:");	    
	while(1)
	{
		adcx=Get_Adc_Average(ADC_Channel_1,10);	
		LCD_ShowxNum(90,110,Res,3,16,0);
		if(Res>=250)
		{
		sign=Res-250;
			while(Res>=250);
			adc[sign]=3*Res;
		}
		
		
	}											    
}	
 
void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������	
									
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}
