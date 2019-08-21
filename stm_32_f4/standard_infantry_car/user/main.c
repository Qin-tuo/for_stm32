/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       main.c/h
  * @brief      stm32��ʼ���Լ���ʼ����freeRTOS��h�ļ��������ȫ�ֺ궨���Լ�
  *             typedef һЩ������������
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
	
		#include "stm32f4xx.h"

		#include "adc.h"
		#include "buzzer.h"
		#include "can.h"
		#include "delay.h"
		#include "flash.h"
		#include "fric.h"
		#include "laser.h"
		#include "led.h"
		#include "power_ctrl.h"
		#include "rc.h"
		#include "rng.h"
		#include "sys.h"
		#include "timer.h"
#include "CAN_Receive.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "calibrate_task.h"
#include "remote_control.h"
#include "start_task.h"
#include "main.h"
#include "Gimbal_Task.h"
#include "gimbal_behaviour.h"
#include "stm32f4xx.h"
#include "Gimbal_Task.h"
#include "adc.h"
#include "buzzer.h"
#include "can.h"
#include "delay.h"
#include "flash.h"
#include "fric.h"
#include "laser.h"
#include "led.h"
#include "power_ctrl.h"
#include "rc.h"
#include "rng.h"
#include "sys.h"
#include "timer.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "calibrate_task.h"
#include "remote_control.h"
#include "start_task.h"
#include "chassis_task.h"
#include "math.h"
void BSP_init(void);
void shoot_init(void);
void pid1(int x,int y,int *p);
const motor_measure_t *motor1,*motor2,*motor3;//��̨���1��2���������ã���pid
const  motor_measure_t *motora1,*motora2,*motora3,*motora4;//���̵��1��2��3��4�������ݣ���pid
const RC_ctrl_t *rc1;
int 	pid2(int x);	
u8 USART2_RX_BUF[32]; //���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u16 USART2_RX_STA = 0;
u8 reverse=1;
u8 Res=0;
u16 anglex=0,angley=0,anglex2=0,angley2=0;
int main(void)
{ 
	u8 t,len;
	u16 times=0;//�����ڴ���
	
	BSP_init();
	motor1=get_Yaw_Gimbal_Motor_Measure_Point();//��̨1����
	motor2=get_Pitch_Gimbal_Motor_Measure_Point();//��̨2����
	motor3=get_Trigger_Motor_Measure_Point();//���̵������
	motora1=get_Chassis_Motor_Measure_Point(0x201);
	motora2=get_Chassis_Motor_Measure_Point(0x202);
	motora3=get_Chassis_Motor_Measure_Point(0x203);
	motora4=get_Chassis_Motor_Measure_Point(0x204);//���̵��1��2��3��4����
	rc1=get_remote_control_point();
	Adc_Init();    
	SPI_OLED_Init();
	SPI6Init();
	OLED_Init();
	int pid[8];//pid��8�ֽ�����
	u8 adcx;//���ڵ���adc
	float Adc1;//���ڵ���adc
	oled_clear(Pen_Clear);//�����ʾ����		
	delay_ms(200);
	fric1_on(1000),fric2_on(1000);
	int a=1000,b=-2500,e=0,count=0,vecd=a=motor3->ecd,h=0;//a��b����Ħ���ֿ���
	usart2_init(115200);
	
		while (1)
				{	
						
					//if(1)
					//USART_SendData(USART2, 8);	
					led_red_off();
					int vpid=0,dv1=0,dv2=0,dv3=0,dv4=0,v1=0,v2=0,v3=0,v4=0;
					int x=rc1->rc.ch[0]-1024,y=rc1->rc.ch[1]-1024,z=rc1->rc.ch[2]-1024,w=rc1->rc.ch[3]-1024,c,d;
					e++;//������ʾ����Ư��
					oled_display_on();//oled��ʾ����ÿ�ο���Ư��
					oled_shownum(1,0,         22, 1, 6);//oled��ʾ
					oled_shownum(1,7, 				motor1->ecd, 1, 6);
					oled_shownum(2,1, 				motor2->ecd, 1, 6);
					
					oled_shownum(3,2,     motora2->speed_rpm, 1, 6);
					oled_shownum(3,9, 		Res, 1, 6);
					oled_shownum(4,4,  anglex, 1, 6);
					oled_shownum(4,11, angley, 1, 6);
					
					
					oled_refresh_gram();						
			if(rc1->rc.s[0]==2&&rc1->rc.s[1]==2)//��̨��ͨ����ģʽ
				{
					
				int vx,vy,vs,tx=x*3,ty=y*2;
					pid1(tx,ty,&pid[0]);
					vx=pid[0];
					vy=pid[1];
					if(w<-500)
					{
					a+=3;
					delay_ms(3);
					}
					else
						a=1000;
						if (a>=1400)
						a=1400;
					fric1_on(a),fric2_on(a);
						if(w<-300&a>=1400)//��̨��λ�����������
						{
							delay_ms(20);
							count++;
								if(count<=12)
								{
									h=-2000; 		
								}
								else
								{h=10000-motor3->speed_rpm*10;}
								if(count>80)
									count=0;
						}
						else
						{	h=0;		buzzer_off();}
						CAN_CMD_GIMBAL(vx,vy,h,1000);
				}
				
				
				
				if(rc1->rc.s[0]==3&&rc1->rc.s[1]==2 )//auto_mode1,��̨��ֵ�Զ�ģʽ
				{
					
					int vx,vy,vs,tx=22.3*(anglex-132),ty=-22.3*(angley-50);
					pid1(tx,ty,&pid[0]);
					vx=pid[0];
					vy=pid[1];
					if(w<-500)
					{
					a+=3;
					delay_ms(3);
					}
					else
						a=1000;
						if (a>=1400)
						a=1400;
					fric1_on(a),fric2_on(a);
						if(w<-300&a>=1400)//��̨��λ�����������
						{
							delay_ms(20);
							count++;
								if(count<=12)
								{
									h=-2000; 		
								}
								else
								{h=10000-motor3->speed_rpm*10;}
								if(count>80)
									count=0;
						}
						else
						{	h=0;		buzzer_off();}
						CAN_CMD_GIMBAL(vx,vy,h,1000);
					}
				
				
				
				
				
				if(rc1->rc.s[0]==1&&rc1->rc.s[1]==2 )//pid_chassis_mode,����pidģʽ
				{
					CAN_CMD_GIMBAL(0,0,0,0);
					if(x==0&&y==0&&z==0&&w==0)
					CAN_CMD_CHASSIS(0,0,0,0);
					vpid=(-motora1->speed_rpm-motora2->speed_rpm+motora3->speed_rpm+motora4->speed_rpm)/4;
					v1=-motora1->speed_rpm,v2=-motora2->speed_rpm,v3=motora3->speed_rpm,v4=motora4->speed_rpm;
					dv1=vpid-v1,dv2=vpid-v2,dv3=vpid-v3,dv4=vpid-v4;
					v1=y*3+dv4*4,v2=-y*3-dv1*4,v3=-y*3-dv2*4,v4=3*y+dv3*4;
				if(x!=0|y!=0)
						{
					if(((y-1.7*x>0)&(y+1.7*x>0))|((y-1.7*x<0)&(y+1.7*x<0)))
					{
					CAN_CMD_CHASSIS(v1,v2,v3,v4);}
				else if(((x-1.7*y>0)&(x+1.7*y>0))|((x-1.7*y<0)&(x+1.7*y<0)))
					CAN_CMD_CHASSIS(-x*4,-x*4,x*4,x*4);
			else
			{
			if(x*y<0)
				CAN_CMD_CHASSIS(-x*8,100,x*8,100);
			if(x*y>=0)
				CAN_CMD_CHASSIS(100,-y*8,100,y*8);
		}
				}
						if(z!=0)
			CAN_CMD_CHASSIS(z*2,z*2,z*2,z*2);
						
				}
				}						
				}
	
void	pid1(int x,int y,int *p)//��̨pid,xΪx����Ƕ�-800~4000��yΪy����Ƕ�-500~1000��2000ʱ��ֱ��
{
int xp,yp,dx,dy,vx,vy,px,py;
	if(y<=-200)
		y=-200;
	px=7400-x,py=6800-y;
	if(motor1->ecd<=3000)
		xp=motor1->ecd+8191;
	else
		xp=motor1->ecd;
	yp=motor2->ecd;
	dx=(px-xp),dy=-(py-yp);
	if(fabs(dx)>500)
	vx=dx*20;
	else if(fabs(dx)>300)
		vx=dx*40;
	else 
		vx=dx*70;
	if(fabs(dy)>=300)
	vy=-dy*40;
	else
	vy=-dy*70;
	*p=vx;
	*(p+1)=vy;
}

#define USART_RX_LEN 16
void USART2_IRQHandler(void)//����2�жϴ�����
{    
if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
Res=USART_ReceiveData(USART2);//(USART1->DR); //��ȡ���յ�����
if(Res>=100&&Res<=200)
anglex=Res;
if(Res>30&&Res<100)
angley=Res;
}  


//�ĸ�24v ��� ���ο��� ��� 709us
#define POWER_CTRL_ONE_BY_ONE_TIME 709

void BSP_init(void)
{
    //�ж��� 4
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    //��ʼ���δ�ʱ��
    delay_init(configTICK_RATE_HZ);
    //��ˮ�ƣ����̵Ƴ�ʼ��
    led_configuration();
    //stm32 �����¶ȴ�������ʼ��
    temperature_ADC_init();
#if GIMBAL_MOTOR_6020_CAN_LOSE_SLOVE
    //stm32 �������������ʼ��
    RNG_init();
#endif
    //24������ƿ� ��ʼ��
    power_ctrl_configuration();
    //Ħ���ֵ��PWM��ʼ��
    fric_PWM_configuration();
    //��������ʼ��
    buzzer_init(30000, 90);
    //����IO��ʼ��
    laser_configuration();
    //��ʱ��6 ��ʼ��
    TIM6_Init(60000, 90);
    //CAN�ӿڳ�ʼ��
    CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
    CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);

    //24v ��� �����ϵ�
    for (uint8_t i = POWER1_CTRL_SWITCH; i < POWER4_CTRL_SWITCH + 1; i++)
    {
        power_ctrl_on(i);
        delay_us(POWER_CTRL_ONE_BY_ONE_TIME);
    }
    //ң������ʼ��
    remote_control_init();
    //flash��ȡ��������У׼ֵ�Żض�Ӧ����
    cali_param_init();
}
