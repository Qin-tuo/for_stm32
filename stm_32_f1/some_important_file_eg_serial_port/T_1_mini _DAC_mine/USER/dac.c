#include "pin.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "motor.h"
#include "dac.h"

void init_dac()
	{	
		GPIO_InitTypeDef GPIO_InitStructure;//gpio��ʼ���Ľṹ��
		DAC_InitTypeDef DAC_InitStruct;//DAC��ʼ���ṹ��		//��GPIOAʱ�ӣ�DAC����PA4��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		//��DACʱ�ӣ���DAC����ADC���տ�ʼ�Ҵ򿪵���ADC��ʱ�ӣ����˺þò��ҵ�ԭ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);		//DAC���ʱIOҪ����Ϊģ�����룬���SSTM32���Ĳο��ֲ� ��12.2�½���󲿷��н���	
		//��DAC���ܺ�PA4���Զ����ӵ�DAC��������STM32���Ĳο��ֲ� ��12.2�½���󲿷��н���	
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;	
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
		GPIO_Init(GPIOA,&GPIO_InitStructure);		
		DAC_DeInit();	//�����ÿ�����ö����Բ鿴�������õ��ǼĴ������ĸ�λ��ע����STM32F10X_DAC.H�ļ���ĵ�130-135�У���ʵ����DAC_Init()�����������ĵط�
		DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//�������ֻ���ڲ��η������Ż��õ�	
		DAC_InitStruct.DAC_OutputBuffer=DAC_OutputBuffer_Disable;//���������	
		DAC_InitStruct.DAC_Trigger=DAC_Trigger_None;//����Ҫ�����¼�	
		DAC_InitStruct.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η�����	
		DAC_Init(DAC_Channel_1,&DAC_InitStruct);//����ʹ�õ���DAC��ͨ��1	
		DAC_Cmd(DAC_Channel_1,ENABLE);//ʹ��DAC		
		DAC_SetChannel1Data(DAC_Align_8b_R,0);//��ʼ�����0v
		}
	



		