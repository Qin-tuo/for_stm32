#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "motor.h"
#include "pin.h"
#include "usart.h"
#include "adc.h"
//ALIENTEK Mini STM32�����巶������8
//PWM���ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
	u8 reverse=1;
	u8 Res;
 int main(void)
 {
	int com1,com2;
	u16 adc[6];
	u8 count=250;
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	pin_Init();
	pin_Init_led();
	uart_init(9600);
	Adc_Init();
	TIM3_PWM_Init(3600-1,200-1);
	TIM1_PWM_Init(3600-1,40-1);//100HZ,��3��1000��60000ֹͣ	 
   	while(1)
	{
	
 		adc[0]=	(Get_Adc_Average(1,4)/10)*10/3;
		adc[1]=	(Get_Adc_Average(2,4)/10)*10/3;
		adc[2]=	(Get_Adc_Average(3,4)/10)*10/3;
		adc[3]=	(Get_Adc_Average(4,4)/10)*10/3;
		com1=(adc[2]-adc[0]);
		com2=(adc[1]-adc[3]);
		if (com1<-7|| com1>7)
		{
		TIM1_PWM_Init(3600-1,230-1);
		if(com1<-4)
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
		if(com1>4)
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
		}
		else 
			TIM1_PWM_Init(3600-1,60000-1);
		
		if (com2<-7|| com2>7)
		{
		TIM3_PWM_Init(3600-1,230-1);
		if(com2<-4)
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		if(com2>4)
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
		}
		else 
			TIM3_PWM_Init(3600-1,60000-1);		
		TIM_SetCompare3(TIM4,3000);	
		TIM_SetCompare2(TIM3,3000);
			if(count==250)
					{
					USART1->DR=250;
					delay_ms(12);
					USART1->DR=adc[0]/3;//0-255
					delay_ms(1);
					count++;
					}
				if(count==251)
					{
					USART1->DR=251;
						delay_ms(12);
					USART1->DR=adc[1]/3;//0-255
						delay_ms(1);
					count++;
					}
					if(count==252)
					{
					USART1->DR=252;
					delay_ms(12);
					USART1->DR=adc[2]/3;//0-255
						delay_ms(1);
					count++;
					}
					if(count==253)
					{
					USART1->DR=253;
					delay_ms(12);
					USART1->DR=adc[3]/3;//0-255
						delay_ms(1);
					count=250;
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
		if(Res==8)
		{
			led_on();
			delay_ms(20);
			reverse=1;
		}	 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}

