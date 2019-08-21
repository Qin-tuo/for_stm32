#include "fric.h"

#include "stm32f4xx.h"

void fric_PWM_configuration(uint32_t arr,uint32_t psc,uint32_t date) //
{

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, DISABLE);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_Pulse = date;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_Cmd(TIM1, ENABLE);

    fric_off();

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    
}

void fric_off(void)
{
    TIM_SetCompare1(TIM1, Fric_OFF);
    TIM_SetCompare4(TIM1, Fric_OFF);
}
void fric1_on(uint16_t cmd)
{
    TIM_SetCompare1(TIM1, cmd);
}
void fric2_on(uint16_t cmd)
{
    TIM_SetCompare4(TIM1, cmd);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void TIM4_PWM_Init(uint32_t arr,uint32_t psc,u32 date)//��һ��pwm���PD12
{                              

    
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);      //TIM3ʱ��ʹ��    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);     //ʹ��PORTAʱ��    
    
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOA6����Ϊ��ʱ��3
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           //GPIOFA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOD,&GPIO_InitStructure);              //��ʼ��PA6
      
    TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��3
    
    //��ʼ��TIM14 Channel1 PWMģʽ     
    TIM_OCInitStructure.TIM_Pulse=date;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
    
    TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
                               
}  


void TIM5_PWM2_Init(uint32_t arr,uint32_t psc,u32 date)//�ڶ���pwm���PH11//TIM5ch2
{                              

    
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);      //TIM4ʱ��ʹ��    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);     //ʹ��PORTAʱ��    
    
    GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5); //GPIOA6����Ϊ��ʱ��3
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //GPIOFA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOH,&GPIO_InitStructure);              //��ʼ��PA6
      
    TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//��ʼ����ʱ��3
    
    //��ʼ��TIM14 Channel1 PWMģʽ     
    TIM_OCInitStructure.TIM_Pulse=date;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
    TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPEʹ�� 
    
    TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM3
                               
} 
void TIM8_PWM1_Init(uint32_t arr,uint32_t psc,u32 date)//���ĸ�pwm���PI5 tim8 ch1
{                              
GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE); 	//ʹ��PORTAʱ��	
	
	//���ò���д������,�������
	//GPIO_PinAFConfig(GPIOA,GPIO_PinSource8|GPIO_PinSource11,GPIO_AF_TIM1); //GPIO����Ϊ��ʱ��1
	
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_TIM8); //����GPIOA_Pin8ΪTIM1_Ch1, 
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOI,&GPIO_InitStructure);              //��ʼ��P
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//Ĭ�Ͼ�Ϊ0
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
 
 
 
	//��ʼ��TIM1  PWMģʽ	 
	//PWM ģʽ 1�C�C �ڵ�������ģʽ�£�ֻҪ TIMx_CNT<TIMx_CCR1��ͨ�� 1 ��Ϊ��Ч״̬������Ϊ��Ч״̬���ڵݼ�����ģʽ�£�
	  
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM1Ϊ����ռ�ձ�ģʽ��PWM2Ϊ������ģʽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�,��Ч��ƽΪ�͵�ƽ
 
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//�ڿ���ʱ���     ��,��������ÿ��Ըı�TIM_OCPolarity ���û��䣬��1ͨ��������
	
	TIM_OCInitStructure.TIM_Pulse = date; //����ͨ��1 CCR1��ռ�ձ���ֵ��
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); //Ch1��ʼ��
	
	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���,CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPEʹ�� 
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM1
	TIM_CtrlPWMOutputs(TIM8, ENABLE);//ʹ��TIM1��PWM�����TIM1��TIM8��Ч,���û�����л�����
						  
                           
} 


void TIM2_PWM3_Init(uint32_t arr,uint32_t psc,u32 date)//������pwm���PA0//TIM2ch1
{                              

    
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      //TIM4ʱ��ʹ��    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);     //ʹ��PORTAʱ��    
    
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); //GPIOA6����Ϊ��ʱ��3
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOFA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA6
      
    TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ��3
    
    //��ʼ��TIM14 Channel1 PWMģʽ     
    TIM_OCInitStructure.TIM_Pulse=date;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ�� 
    
    TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM3
                               
} 







void Out_EXTI_Configuration1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

/* GPIOF Periph clock enable */
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ע��Ҫ��SYSCFGʱ��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    EXTI_InitTypeDef EXTI_InitStructure;
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3); //������ǰ��F103���õ����ú���
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);    //407ʹ�õ����ú���
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;


  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
void Out_EXTI_Configuration2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

/* GPIOF Periph clock enable */
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ע��Ҫ��SYSCFGʱ��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    EXTI_InitTypeDef EXTI_InitStructure;
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3); //������ǰ��F103���õ����ú���
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);    //407ʹ�õ����ú���
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;


  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =10;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void vcc_Init()//���Ƹߵ͵�ƽPE5,6,4
{
		GPIO_InitTypeDef GPIO_InitStructure;    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);     //ʹ��PORTAʱ��      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_4;           //GPIOFA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOE,&GPIO_InitStructure);     
		GPIO_SetBits(GPIOE,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_4);
}


void gpio2_Init(void)//pf0
{
GPIO_InitTypeDef GPIO_Struct1;	
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
GPIO_Struct1.GPIO_Pin=GPIO_Pin_0;
GPIO_Struct1.GPIO_Mode=GPIO_Mode_IN;
GPIO_Struct1.GPIO_Speed=GPIO_Speed_100MHz;
GPIO_Struct1.GPIO_PuPd=GPIO_PuPd_UP;	
GPIO_Init(GPIOE,&GPIO_Struct1);

}


