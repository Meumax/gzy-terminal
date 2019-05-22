#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "rs485.h"	 
#include "led.h"
#include "timer.h"
   
extern u8 RS485_RX_BUF[64]; //����485ģ���������
const u8 TerminalNumber = 0x02;//�ն˱��

//�ж������Ƿ���ָ�����
int flag = 0;

//����for����
u8 i;
	
//�洢�������ɼ���������
u8 data[49];
											
//��ѯ֡�������
u8 ThreeinoneAsk01[10] = {0x01,0x03,0x01,0xf4,0x00,0x08,0x04,0x02,0x00,0x00}; //1�Ŵ���������һ��ѯ֡
u8 SoliAsk02[8] = {0x02,0x03,0x00,0x04,0x00,0x02,0x85,0xf9}; //2��λ������ѯ֡
u8 SoliAsk03[8] = {0x03,0x03,0x00,0x04,0x00,0x02,0x84,0x28}; //3��λ������ѯ֡
u8 SoliAsk04[8] = {0x04,0x03,0x00,0x04,0x00,0x02,0x85,0x9f}; //4��λ������ѯ֡
u8 SoliAsk05[8] = {0x05,0x03,0x00,0x04,0x00,0x02,0x84,0x4e}; //5��λ������ѯ֡
u8 SoliAsk06[8] = {0x06,0x03,0x00,0x04,0x00,0x02,0x84,0x7d}; //6��λ������ѯ֡
u8 SoliAsk07[8] = {0x07,0x03,0x00,0x04,0x00,0x02,0x85,0xac}; //7��λ������ѯ֡
u8 SoliAsk08[8] = {0x08,0x03,0x00,0x04,0x00,0x02,0x85,0x53}; //8��λ������ѯ֡
u8 SoliAsk09[8] = {0x09,0x03,0x00,0x04,0x00,0x02,0x84,0x82}; //9��λ������ѯ֡
u8 SoliAsk10[8] = {0x0a,0x03,0x00,0x04,0x00,0x02,0x84,0xb1}; //10��λ������ѯ֡
u8 SoliPHAsk11[8] = {0x0b,0x03,0x00,0x08,0x00,0x01,0x05,0x62}; //11��λ����PH��ѯ֡

//��������������
void SensorOperation(u8 terminal);

	/*�����������*/
int main(void)
{ 
	//��ʼ������
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	RS485_Init(9600);	//��ʼ��RS485
	LED_Init();	
	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	data[47] = 0x0d;//�������ݽ�β������0d 0a
	data[48] = 0x0a;
		
	//����ѭ��ִ��
	while(1)
	{
		//����⵽�����յ�����
		if(flag)
		{	
			//�ж��Ƿ�ѯ�ʱ��豸������
			if(USART_RX_BUF[0] == TerminalNumber)
			{
				SensorOperation(TerminalNumber);//ִ�д������ɼ����򲢷�������				
			}
			flag = 0;//������ձ�־���ȴ���һ�ν���	
			USART_RX_STA = 0;			
		}					
	}
}

//��������������
void SensorOperation(u8 terminal)
{
	Relay_On = 1;
	delay_ms(3000);
	
		data[0] = TerminalNumber;//�ն˺�
	
	//��1������һ������������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(ThreeinoneAsk01,10);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[1] = RS485_RX_BUF[3];//����һ�Ŀ���ʪ�ȸ�8λ
		data[2] = RS485_RX_BUF[4];//����һ�Ŀ���ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[3] = RS485_RX_BUF[5];//����һ�Ŀ����¶ȸ�8λ
		data[4] = RS485_RX_BUF[6];//����һ�Ŀ����¶ȵ�8λ
		//��ȡ����ǿ��ֵ
		data[5] = RS485_RX_BUF[15];//����һ�Ĺ���ǿ��01λ
		data[6] = RS485_RX_BUF[16];//����һ�Ĺ���ǿ��02λ
		data[7] = RS485_RX_BUF[17];//����һ�Ĺ���ǿ��03λ
		data[8] = RS485_RX_BUF[18];//����һ�Ĺ���ǿ��04λ
		//��շ���
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;	
		
		//��2��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk02,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[9] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[10] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[11] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[12] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
				
		//��3��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk03,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[13] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[14] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[15] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[16] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��4��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk04,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[17] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[18] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[19] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[20] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
			
		//��5��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk05,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[21] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[22] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[23] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[24] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��6��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk06,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[25] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[26] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[27] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[28] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��7��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk07,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[29] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[30] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[31] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[32] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��8��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk08,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[33] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[34] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[35] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[36] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��9��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk09,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[37] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[38] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[39] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[40] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��10��������ʪ�ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliAsk10,8);
		delay_ms(100);
		//��ȡ����ʪ��ֵ
		data[41] = RS485_RX_BUF[3];//ʪ�ȸ�8λ
		data[42] = RS485_RX_BUF[4];//ʪ�ȵ�8λ
		//��ȡ�����¶�ֵ
		data[43] = RS485_RX_BUF[5];//�¶ȸ�8λ
		data[44] = RS485_RX_BUF[6];//�¶ȵ�8λ	
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//��11���������ȴ�����������ѯ֡�����淵�ص�Ӧ��֡
		RS485_Send_Data(SoliPHAsk11,8);
		delay_ms(100);
		//��ȡ����PHֵ
		data[45] = RS485_RX_BUF[3];//PH��8λ
		data[46] = RS485_RX_BUF[4];//PH��8λ
		//��շ���			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//�����ط�������
		for(i=0;i<49;i++)
		{
			USART_SendData(USART1, data[i]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		
		//�������
		for(i=1;i<47;i++)
		{
			data[i] = 0;
		}
		Relay_On = 0;
}
