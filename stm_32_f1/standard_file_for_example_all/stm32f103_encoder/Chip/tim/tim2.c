#include "tim2.h"


//�߼���ʱ��1pwm�����ʼ��
//arr���Զ���װֵ�����ڣ�  psc��ʱ��Ԥ��Ƶ��
void tim2_encoderInit(uint16_t arr, uint16_t psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // ʹ�ܶ�ʱ��1������ʱ��
 	RCC_APB2PeriphClockCmd(TIM2_CH1_GPIO_CLK | TIM2_CH2_GPIO_CLK, ENABLE);  //ʹ��GPIO����ʱ��ʹ��                               	

   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = TIM2_CH1_PIN | TIM2_CH2_PIN; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�����������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIM2_CH1_PORT, &GPIO_InitStructure);
  GPIO_Init(TIM2_CH2_PORT, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //(ENCODER_RESOLUTION * 4) - 1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1| TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x06;
  TIM_ICInit(TIM2,&TIM_ICInitStructure);
  
  TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
  
  //TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM2,0);
  
  TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM1
}




