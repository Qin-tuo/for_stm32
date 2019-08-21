#include "pin.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "motor.h"
void pin_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIO����ʱ��ʹ��	                                                                     	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_3; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	
}

void pin_Init_led(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIO����ʱ��ʹ��	                                                                     	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void led_on()
{
GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}
void led_off()
{
GPIO_SetBits(GPIOC,GPIO_Pin_13);
}