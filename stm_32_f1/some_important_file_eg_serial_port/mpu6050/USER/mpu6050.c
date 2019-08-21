
#include "mpu6050.h"
#include "delay.h"
 

struct ACCELSTRUCT accelStruct = {0,0,0};

struct GYROSTRUCT	gyroStruct = {0,0,0};

 

 

//IO��������

#define MPU_SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}

#define MPU_SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

 

//IO��������	 

#define MPU_SCL    PCout(10) //MPU SCL

#define MPU_SDA    PCout(11) //MPU SDA	 

#define MPU_READ_SDA   PCin(11)  //����SDA 

 

 

/**************************MPU5883 IIC��������*********************************/

 

static void MPU5883IOInit(void)

{

	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	

    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PC10 PC11

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    MPU_SCL = 1;//��ʼ����Ϊ����״̬

    MPU_SDA = 1;

}

 

 

 

//����IIC��ʼ�ź�

static void ComStart(void)

{

	MPU_SDA_OUT();     //sda�����

    MPU_SDA=1;	  	  

    MPU_SCL=1;

    delay_us(5);

    MPU_SDA=0;//START:when CLK is high,DATA change form high to low 

    delay_us(5);

    MPU_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������

}

//����IICֹͣ�ź�

static void ComStop(void)

{

	MPU_SDA_OUT();//sda�����

    MPU_SDA=0;//STOP:when CLK is high DATA change form low to high

    MPU_SCL=1;

    delay_us(5);

    MPU_SDA=1;//����I2C���߽����ź�

    delay_us(5);		

}

//�ȴ�ACK,Ϊ1������ACK Ϊ0����ȵ���ACK

static u8 ComWaitAck(void)

{

	u8 waitTime = 0;

	MPU_SDA_OUT();//sda�����

	MPU_SDA = 1;

	delay_us(5);

    MPU_SDA_IN();      //SDA����Ϊ����

	MPU_SCL=1;

	delay_us(5);

	while(MPU_READ_SDA)

	{

		waitTime++;

		delay_us(1);

		if(waitTime > MPU_ACK_WAIT_TIME)

		{

			ComStop();

			return 1;

		}

	}

	MPU_SCL = 0;

	return 0;

	

}

 

//static void ComSendAck(void)

//{

//	MPU_SCL = 0;

//	MPU_SDA_OUT();

//    MPU_SDA = 0;

//	DelayUs(2);

//    MPU_SCL = 1;

//    DelayUs(5);

//    MPU_SCL = 0;

//    DelayUs(5);

//}

 

static void ComSendNoAck(void)

{

	MPU_SCL = 0;

	MPU_SDA_OUT();

    MPU_SDA = 1;

	delay_us(2);

    MPU_SCL = 1;

    delay_us(5);

    MPU_SCL = 0;

    delay_us(5);

}

//����0 д���յ�ACK ����1д��δ�յ�ACK

static u8 ComSendByte(u8 byte)

{

	u8 t;   

    MPU_SDA_OUT(); 	

    for(t=0;t<8;t++)

    {              

        MPU_SDA=(byte&0x80)>>7;

        byte<<=1; 	   

        MPU_SCL=1;

        delay_us(5); 

        MPU_SCL=0;	

        delay_us(5);

    }	 

    return ComWaitAck();

}

 

static void ComReadByte(u8* byte)

{

	u8 i,receive=0;

    MPU_SDA_IN();//SDA����Ϊ����

    for(i=0;i<8;i++ )

    {

        receive <<= 1;

        MPU_SCL=1; 

        delay_us(5);

        if(MPU_READ_SDA)receive++;

        MPU_SCL=0; 

        delay_us(5); 

    }					  

    *byte = receive;

}

 

/**************************MPU5883 IIC��������*********************************/

 

 

//��MPUд��һ���ֽ�����,ʧ�ܷ���1 �ɹ�����0

u8 MPUWriteReg(u8 regValue,u8 setValue)

{

	u8 res;

    ComStart();                 	//��ʼ�ź�

    res = ComSendByte(MPU_ADDR);    //�����豸��ַ+д�ź�

	if(res)

	{

		#ifdef MPU_DEBUG

		printf("file=%s,func=%s,line=%d\r\n",__FILE__,__FUNCTION__,__LINE__);

		#endif

		return res;

	}

    res = ComSendByte(regValue);    //�ڲ��Ĵ�����ַ

	if(res)

	{

		#ifdef MPU_DEBUG

		printf("file=%s,func=%s,line=%d\r\n",__FILE__,__FUNCTION__,__LINE__);

		#endif

		return res;

	}

    res = ComSendByte(setValue);    //�ڲ��Ĵ�������

	if(res)

	{

		#ifdef MPU_DEBUG

		printf("file=%s,func=%s,line=%d\r\n",__FILE__,__FUNCTION__,__LINE__);

		#endif

		return res;

	}

    ComStop();                   	//����ֹͣ�ź�

	return res;

}

 

//**************************************

//��I2C�豸��ȡһ���ֽ����� ����ֵ ��ȡ�ɹ���ʧ��

//**************************************

u8 MPUReadReg(u8 regAddr,u8* readValue)

{

    u8 res;

    ComStart();                 		//��ʼ�ź�

    res = ComSendByte(MPU_ADDR);    	//�����豸��ַ+д�ź�

	if(res)

	{

		#ifdef MPU_DEBUG

		printf("file=%s,func=%s,line=%d\r\n",__FILE__,__FUNCTION__,__LINE__);

		#endif

		return res;

	}

    res = ComSendByte(regAddr);     	//���ʹ洢��Ԫ��ַ����0��ʼ	

	if(res)

	{

		#ifdef MPU_DEBUG

		printf("file=%s,func=%s,line=%d\r\n",__FILE__,__FUNCTION__,__LINE__);

		#endif

		return res;

	}

    ComStart();                 		//��ʼ�ź�

    res = ComSendByte(MPU_ADDR+1);  	//�����豸��ַ+���ź�

	if(res)

	{

		#ifdef MPU_DEBUG

		printf("file=%s,func=%s,line=%d\r\n",__FILE__,__FUNCTION__,__LINE__);

		#endif

		return res;

	}

    ComReadByte(readValue);     		//�����Ĵ�������

    ComSendNoAck();               		//���ͷ�Ӧ���ź�

    ComStop();                  		//ֹͣ�ź�

    return res;

}

 

//MPU��ȡ�����ֽڵ�����

s16 MpuReadTwoByte(u8 addr)

{

    u8 H,L;

    MPUReadReg(addr,&H);

    MPUReadReg(addr+1,&L);

    return (s16)((((u16)H)<<8)+L);   //�ϳ�����

}

 

/*

 *��ʼ��������0����ʧ�� ����1����ɹ�

 **/

u8 MpuInit(void)

{

	u8 result;

	u8 id = 0;

    MPU5883IOInit();

	result = MPUReadReg(MPU6050_RA_WHO_AM_I,&id);

	if(result)	return result;	//IIC���ߴ���

	else 

	{

		id &= 0x7e;//��ȥ���λ���λ

		id>>= 1;

		if(id != 0x34) return 1;	//��ȡ����оƬID����

	}

    //��ʼ���ɹ������ò���

    MPUWriteReg(MPU6050_RA_PWR_MGMT_1,0x01);			// �˳�˯��ģʽ����ȡ��ʱ��Ϊ����X�ᡣ

    MPUWriteReg(MPU6050_RA_SMPLRT_DIV,0x04);			// ȡ��ʱ��4��Ƶ��1k/4��ȡ����Ϊ25Hz��

    MPUWriteReg(MPU6050_RA_CONFIG,2);				// ��ͨ�˲�����ֹƵ��100Hz���ҡ�

    MPUWriteReg(MPU6050_RA_GYRO_CONFIG,3<<3);			// �������̣�2000dps

    MPUWriteReg(MPU6050_RA_ACCEL_CONFIG,2<<3);			// ���ٶȼ����̣�8g��

    MPUWriteReg(MPU6050_RA_INT_PIN_CFG,0x32);					// �ж��ź�Ϊ�ߵ�ƽ�����������ֱ���ж�ȡ��������ʧ��ֱͨ����I2C��

    MPUWriteReg(MPU6050_RA_INT_ENABLE,0x01);					// ʹ�á�����׼���á��жϡ�

    MPUWriteReg(MPU6050_RA_USER_CTRL,0x00);					// ��ʹ�ø���I2C��

    return 0;

}

 

 

//��ȡ��Ӧ�Ĳ�������
s16 temp = 0;
void MpuGetData(void)

{

	

    accelStruct.accelX = MpuReadTwoByte(MPU6050_RA_ACCEL_XOUT_H);

    accelStruct.accelY = MpuReadTwoByte(MPU6050_RA_ACCEL_YOUT_H);

    accelStruct.accelZ = MpuReadTwoByte(MPU6050_RA_ACCEL_ZOUT_H);

    gyroStruct.gyroX = MpuReadTwoByte(MPU6050_RA_GYRO_XOUT_H);

    gyroStruct.gyroY = MpuReadTwoByte(MPU6050_RA_GYRO_YOUT_H);

    gyroStruct.gyroZ = MpuReadTwoByte(MPU6050_RA_GYRO_ZOUT_H);

	temp = MpuReadTwoByte(MPU6050_RA_TEMP_OUT_H);

	#ifdef MPU_DEBUG

	printf("accel  x = %d  ,y =  %d  ,z = %d  \r\n",accelStruct.accelX,accelStruct.accelY,accelStruct.accelZ);

	printf("gyro  x = %d  ,y =  %d  ,z = %d  \r\n",gyroStruct.gyroX,gyroStruct.gyroY,gyroStruct.gyroZ);

	printf("temp is %0.3f \r\n",(((float)temp)/340.0 + 36.53));

	#endif

	

}








