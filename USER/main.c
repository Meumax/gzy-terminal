#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "rs485.h"	 
#include "led.h"
#include "stdio.h"
 
/************************************************
����ҽ��������Ŀ��λ������V2.0
����ʱ�䣺2018.10.17
������ʱ�䣺2019.01.15
����Ա�������� ʯ����
���ߣ�RO->RS485_TX->USART2_RX->PA3;
			RE->RS485_RE->PD7;
			DI->RS485_RX->USART2_TX->PA2;
��������ַ������һ0x01,������ʪ��0x02������PHֵ0x03
��ѯ֡���壺{��ַ�룬�����룬��ʼ��ַ�ߣ���ʼ��ַ�ͣ����ݳ��ȸߣ����ݳ��ȵͣ�У����ͣ�У�����}
Ӧ��֡���壺{��ַ�룬�����룬�ֽ�����ʪ��ֵ�ߣ�ʪ��ֵ�ͣ�10�������ֽڣ����ն�4�ֽڣ�У����}
����ʵ��������ʪ��ֵΪ��0x02,0x92,��ʪ��ֵ����Ϊ��0292(16����)=658(10����)=>65.8%RH
					�����¶�ֵΪ��0xff,0x9b,���¶�ֵ����Ϊ��ff9b(16����)=-101(10����)=>-10.1��
					������ն�Ϊ��0x00,0x01,0x77,0x34,��00017734(16����)=96052(10����)=>96052Lux
					���¶ȵ���0��ʱ�������Բ�����ʽ�ϴ���Ҳ�������ΪΪF��
���͸�ʽJSON��
							{
								"tSerialNumber":"343",
								"tTemperature":13.0,
								"tHumidity":83.0,
								"tLightIntensity":46,
								"tSoilTemperature01":13.1,"tSoilHumidity01":36.1,
								"tSoilTemperature02":13.2,"tSoilHumidity02":36.2,
								"tSoilTemperature03":13.3,"tSoilHumidity03":36.3,
								"tSoilTemperature04":13.4,"tSoilHumidity04":36.4,
								"tSoilTemperature05":13.5,"tSoilHumidity05":36.5,
								"tSoilTemperature06":13.6,"tSoilHumidity06":36.6,
								"tSoilTemperature07":13.7,"tSoilHumidity07":36.7,
								"tSoilTemperature08":13.8,"tSoilHumidity08":36.8,
								"tSoilTemperature09":13.9,"tSoilHumidity09":36.9,
							}
************************************************/         
extern u8 RS485_RX_BUF[64];
void Solid_Sensor_Read(u8 *SoliAsk0x,u16 SoilTemperature0x,u16 SoilHumidity0x);
	//����for����
	u8 i;
	
	//�洢Ŀ���ַ���
	char s[300];
	
	//����ʪ��ֵ
	u8 AtmosphereHumidity_H,AtmosphereHumidity_L;
	u16 AtmosphereHumidity;
	
  //�����¶�ֵ	 
	u8 AtmosphereTemperature_H,AtmosphereTemperature_L;
	u16 AtmosphereTemperature; 
	
	//����ǿ��ֵ
	u8 Illumination_01,Illumination_02,Illumination_03,Illumination_04;
	u32 Illumination;
	
	//����ʪ��ֵ
	u8 SoilHumidity_H,SoilHumidity_L;
	u16 SoilHumidity01,SoilHumidity02,SoilHumidity03,
			SoilHumidity04,SoilHumidity05,SoilHumidity06,
			SoilHumidity07,SoilHumidity08,SoilHumidity09;
	
	//�����¶�ֵ
	u8 SoilTemperature_H,SoilTemperature_L;
	u16 SoilTemperature01,SoilTemperature02,SoilTemperature03,
			SoilTemperature04,SoilTemperature05,SoilTemperature06,
			SoilTemperature07,SoilTemperature08,SoilTemperature09;
			
	//��ѯ֡�������
	u8 Threeinone_Ask[10] = {0x01,0x03,0x01,0xf4,0x00,0x08,0x04,0x02,0x00,0x00}; //����һ��ѯ֡
	u8 SoliAsk01[8] = {0x02,0x03,0x00,0x00,0x00,0x02,0xc4,0x38}; //1��λ������ѯ֡
	u8 SoliAsk02[8] = {0x03,0x03,0x00,0x04,0x00,0x02,0x84,0x28}; //2��λ������ѯ֡
	u8 SoliAsk03[8] = {0x04,0x03,0x00,0x04,0x00,0x02,0x85,0x9f}; //3��λ������ѯ֡
	u8 SoliAsk04[8] = {0x05,0x03,0x00,0x04,0x00,0x02,0x84,0x4e}; //4��λ������ѯ֡
	u8 SoliAsk05[8] = {0x06,0x03,0x00,0x04,0x00,0x02,0x84,0x7d}; //5��λ������ѯ֡
	u8 SoliAsk06[8] = {0x07,0x03,0x00,0x04,0x00,0x02,0x85,0xac}; //6��λ������ѯ֡
	u8 SoliAsk07[8] = {0x08,0x03,0x00,0x04,0x00,0x02,0x85,0x53}; //7��λ������ѯ֡
	u8 SoliAsk08[8] = {0x09,0x03,0x00,0x04,0x00,0x02,0x84,0x82}; //8��λ������ѯ֡
	u8 SoliAsk09[8] = {0x0a,0x03,0x00,0x04,0x00,0x02,0x84,0xb1}; //9��λ������ѯ֡
	
	void SolidSensor_Read(u8 *SoliAsk0x,u16 SoilHumidity0x,u16 SoilTemperature0x)
	{
		//��1��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk0x,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity0x = ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature0x = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
	}
	
	/*�����������*/
 int main(void)
 { 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	RS485_Init(9600);	//��ʼ��RS485
	LED_Init();		
		
	while(1)
	{
		//������һ������������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(Threeinone_Ask,10);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		AtmosphereHumidity_H = RS485_RX_BUF[3];
		AtmosphereHumidity_L = RS485_RX_BUF[4];
		AtmosphereHumidity = (u16)(AtmosphereHumidity_H << 8)| AtmosphereHumidity_L;
		//��ȡ�����¶�ֵ
		AtmosphereTemperature_H = RS485_RX_BUF[5];
		AtmosphereTemperature_L = RS485_RX_BUF[6];
		AtmosphereTemperature = (u16)(AtmosphereTemperature_H <<8)|AtmosphereTemperature_L;
		//��ȡ����ǿ��ֵ
		Illumination_01 = RS485_RX_BUF[15];
		Illumination_02 = RS485_RX_BUF[16];
		Illumination_03 = RS485_RX_BUF[17];
		Illumination_04 = RS485_RX_BUF[18];
		Illumination = ((u32)(Illumination_01 << 24 | Illumination_02 << 16 | Illumination_03 << 8) | Illumination_04);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;	
			
		//��1��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk01,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity01 = ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature01 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��2��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk02,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity02 = ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature02 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��3������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk03,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity03 = ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature03 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��4������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk04,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity04 = ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature04 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��5������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk05,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity05 = ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature05 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��6������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk06,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity06 = ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature06 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��7������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk07,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity07= ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature07 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��8������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk08,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity08= ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature08 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��9������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk09,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		SoilHumidity_H = RS485_RX_BUF[3];
		SoilHumidity_L = RS485_RX_BUF[4];
		SoilHumidity09 = ((u16)SoilHumidity_H << 8 | SoilHumidity_L);
		//��ȡ�����¶�ֵ
		SoilTemperature_H = RS485_RX_BUF[5];
		SoilTemperature_L = RS485_RX_BUF[6];
		SoilTemperature09 = ((u16)SoilTemperature_H << 8 | SoilTemperature_L);
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		
		sprintf(s,
							"{\"tSerialNum\":\"%d\",\"tTemp\":%d.%d,\"tHumi\":%d.%d,\"tLight\":%d,"              
							 "\"tSoilTemp01\":%d.%d,\"tSoilHumi01\":%d.%d," 
							 "\"tSoilTemp02\":%d.%d,\"tSoilHumi02\":%d.%d," 
							 "\"tSoilTemp03\":%d.%d,\"tSoilHumi03\":%d.%d," 
						 	 "\"tSoilTemp04\":%d.%d,\"tSoilHumi04\":%d.%d," 
							 "\"tSoilTemp05\":%d.%d,\"tSoilHumi05\":%d.%d," 
							 "\"tSoilTemp06\":%d.%d,\"tSoilHumi06\":%d.%d," 
							 "\"tSoilTemp07\":%d.%d,\"tSoilHumi07\":%d.%d," 
							 "\"tSoilTemp08\":%d.%d,\"tSoilHumi08\":%d.%d," 
							 "\"tSoilTemp09\":%d.%d,\"tSoilHumi09\":%d.%d," 
							 "}",
							 343,//�ն˱��
							 AtmosphereTemperature/10,AtmosphereTemperature%10,//�����¶�ֵ
							 AtmosphereHumidity/10,AtmosphereHumidity%10,//����ʪ��ֵ
							 Illumination,//����ǿ��ֵ
							 SoilTemperature01/10,SoilTemperature01%10,SoilHumidity01/10,SoilHumidity01%10,//1��������ʪ��
							 SoilTemperature02/10,SoilTemperature02%10,SoilHumidity02/10,SoilHumidity02%10,//2��������ʪ��
							 SoilTemperature03/10,SoilTemperature03%10,SoilHumidity03/10,SoilHumidity03%10,//3��������ʪ��
							 SoilTemperature04/10,SoilTemperature04%10,SoilHumidity04/10,SoilHumidity04%10,//4��������ʪ��
							 SoilTemperature05/10,SoilTemperature05%10,SoilHumidity05/10,SoilHumidity05%10,//5��������ʪ��
							 SoilTemperature06/10,SoilTemperature06%10,SoilHumidity06/10,SoilHumidity06%10,//6��������ʪ��
							 SoilTemperature07/10,SoilTemperature07%10,SoilHumidity07/10,SoilHumidity07%10,//7��������ʪ��
							 SoilTemperature08/10,SoilTemperature08%10,SoilHumidity08/10,SoilHumidity08%10,//8��������ʪ��
							 SoilTemperature09/10,SoilTemperature09%10,SoilHumidity09/10,SoilHumidity09%10 //9��������ʪ��
							 );  
		printf("%s",s);
		
		AtmosphereTemperature = 0;AtmosphereHumidity = 0;Illumination = 0;
		SoilHumidity01 = 0;SoilTemperature01 = 0;
		SoilHumidity02 = 0;SoilTemperature02 = 0;
		SoilHumidity03 = 0;SoilTemperature03 = 0;
		SoilHumidity04 = 0;SoilTemperature04 = 0;
		SoilHumidity05 = 0;SoilTemperature05 = 0;
		SoilHumidity06 = 0;SoilTemperature06 = 0;
		SoilHumidity07 = 0;SoilTemperature07 = 0;
		SoilHumidity08 = 0;SoilTemperature08 = 0;
		SoilHumidity09 = 0;SoilTemperature09 = 0;

		
		for(i=0;i<3;i++)
		{
			delay_ms(1000);
			LED0=0;
			LED1=1;
			delay_ms(1000);	 
			LED0=1;
			LED1=0;	
		}
	
	}
}

