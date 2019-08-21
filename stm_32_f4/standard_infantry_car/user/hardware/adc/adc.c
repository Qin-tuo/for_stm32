#include "adc.h"
#include "delay.h"
#include "stm32f4xx.h"

static uint16_t get_ADC(uint8_t ch);
static void temperature_ADC_Reset(void);
void temperature_ADC_init(void)
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);

    ADC_TempSensorVrefintCmd(ENABLE);

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);
    temperature_ADC_Reset();

    ADC_RegularChannelConfig(ADC1, ADC_Channel_18, 1, ADC_SampleTime_15Cycles);
    ADC_Cmd(ADC1, ENABLE);
}
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE); 
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInit(&ADC_CommonInitStructure);//3?��??��
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12???�꨺?
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��?����?��?�꨺?	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//1?��?��?D?��a??
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//???1�䣤����?��2a��?��1��?����?t�䣤����
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//����????	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1??��a???��1??��D����D?D ��2?����???��a??1??��D����D1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC3?��??��
	
 
	ADC_Cmd(ADC1, ENABLE);//?a??AD��a???��	

}				  
uint16_t Get_Adc(uint8_t ch)   
{
	  	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC�����̨�,480???��?��,����??2��?������???����?����????����?��			    
  
	ADC_SoftwareStartConv(ADC1);		//��1?��???����?ADC1��?����?t��a?????��1|?��	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�̨���y��a???����?

	return ADC_GetConversionValue(ADC1);	//����??��??����?��?ADC11??��������?��a???��1?
}

uint16_t Get_Adc_Average(uint8_t ch,uint8_t times)
{
	u32 temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 
static void temperature_ADC_Reset(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

}

fp32 get_temprate(void)
{
    uint16_t adcx = 0;
    fp32 temperate;
    temperature_ADC_Reset();
    adcx = get_ADC(ADC_Channel_18);
    temperate = (fp32)adcx * (3.3f / 4096.0f);
    temperate = (temperate - 0.76f) / 0.0025f + 25.0f;
    return temperate;
}

static uint16_t get_ADC(uint8_t ch)
{

    ADC_ClearFlag(ADC1,ADC_FLAG_STRT|ADC_FLAG_OVR|ADC_FLAG_EOC);
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_15Cycles);

    ADC_SoftwareStartConv(ADC1);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
        ;
    }
    return ADC_GetConversionValue(ADC1);
}
