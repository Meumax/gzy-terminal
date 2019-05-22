#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "rs485.h"	 
#include "led.h"
#include "timer.h"
   
extern u8 RS485_RX_BUF[64]; //用于485模块接收数据
const u8 TerminalNumber = 0x02;//终端编号

//判断网关是否发送指令过来
int flag = 0;

//用于for计数
u8 i;
	
//存储传感器采集到的数据
u8 data[49];
											
//问询帧存放数组
u8 ThreeinoneAsk01[10] = {0x01,0x03,0x01,0xf4,0x00,0x08,0x04,0x02,0x00,0x00}; //1号传感器三合一问询帧
u8 SoliAsk02[8] = {0x02,0x03,0x00,0x04,0x00,0x02,0x85,0xf9}; //2号位土壤问询帧
u8 SoliAsk03[8] = {0x03,0x03,0x00,0x04,0x00,0x02,0x84,0x28}; //3号位土壤问询帧
u8 SoliAsk04[8] = {0x04,0x03,0x00,0x04,0x00,0x02,0x85,0x9f}; //4号位土壤问询帧
u8 SoliAsk05[8] = {0x05,0x03,0x00,0x04,0x00,0x02,0x84,0x4e}; //5号位土壤问询帧
u8 SoliAsk06[8] = {0x06,0x03,0x00,0x04,0x00,0x02,0x84,0x7d}; //6号位土壤问询帧
u8 SoliAsk07[8] = {0x07,0x03,0x00,0x04,0x00,0x02,0x85,0xac}; //7号位土壤问询帧
u8 SoliAsk08[8] = {0x08,0x03,0x00,0x04,0x00,0x02,0x85,0x53}; //8号位土壤问询帧
u8 SoliAsk09[8] = {0x09,0x03,0x00,0x04,0x00,0x02,0x84,0x82}; //9号位土壤问询帧
u8 SoliAsk10[8] = {0x0a,0x03,0x00,0x04,0x00,0x02,0x84,0xb1}; //10号位土壤问询帧
u8 SoliPHAsk11[8] = {0x0b,0x03,0x00,0x08,0x00,0x01,0x05,0x62}; //11号位酸碱度PH问询帧

//传感器工作函数
void SensorOperation(u8 terminal);

	/*程序运行入口*/
int main(void)
{ 
	//初始化设置
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	RS485_Init(9600);	//初始化RS485
	LED_Init();	
	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
	data[47] = 0x0d;//串口数据结尾必须是0d 0a
	data[48] = 0x0a;
		
	//程序循环执行
	while(1)
	{
		//若检测到串口收到数据
		if(flag)
		{	
			//判断是否询问本设备的数据
			if(USART_RX_BUF[0] == TerminalNumber)
			{
				SensorOperation(TerminalNumber);//执行传感器采集程序并发送网关				
			}
			flag = 0;//清除接收标志，等待下一次接收	
			USART_RX_STA = 0;			
		}					
	}
}

//传感器工作函数
void SensorOperation(u8 terminal)
{
	Relay_On = 1;
	delay_ms(3000);
	
		data[0] = TerminalNumber;//终端号
	
	//向1号三合一传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(ThreeinoneAsk01,10);
		delay_ms(100);
		//获取空气湿度值
		data[1] = RS485_RX_BUF[3];//三合一的空气湿度高8位
		data[2] = RS485_RX_BUF[4];//三合一的空气湿度低8位
		//获取空气温度值
		data[3] = RS485_RX_BUF[5];//三合一的空气温度高8位
		data[4] = RS485_RX_BUF[6];//三合一的空气温度低8位
		//获取光照强度值
		data[5] = RS485_RX_BUF[15];//三合一的光照强度01位
		data[6] = RS485_RX_BUF[16];//三合一的光照强度02位
		data[7] = RS485_RX_BUF[17];//三合一的光照强度03位
		data[8] = RS485_RX_BUF[18];//三合一的光照强度04位
		//清空返回
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;	
		
		//向2号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk02,8);
		delay_ms(100);
		//获取土壤湿度值
		data[9] = RS485_RX_BUF[3];//湿度高8位
		data[10] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[11] = RS485_RX_BUF[5];//温度高8位
		data[12] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
				
		//向3号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk03,8);
		delay_ms(100);
		//获取土壤湿度值
		data[13] = RS485_RX_BUF[3];//湿度高8位
		data[14] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[15] = RS485_RX_BUF[5];//温度高8位
		data[16] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//向4号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk04,8);
		delay_ms(100);
		//获取土壤湿度值
		data[17] = RS485_RX_BUF[3];//湿度高8位
		data[18] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[19] = RS485_RX_BUF[5];//温度高8位
		data[20] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
			
		//向5号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk05,8);
		delay_ms(100);
		//获取土壤湿度值
		data[21] = RS485_RX_BUF[3];//湿度高8位
		data[22] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[23] = RS485_RX_BUF[5];//温度高8位
		data[24] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//向6号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk06,8);
		delay_ms(100);
		//获取土壤湿度值
		data[25] = RS485_RX_BUF[3];//湿度高8位
		data[26] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[27] = RS485_RX_BUF[5];//温度高8位
		data[28] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//向7号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk07,8);
		delay_ms(100);
		//获取土壤湿度值
		data[29] = RS485_RX_BUF[3];//湿度高8位
		data[30] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[31] = RS485_RX_BUF[5];//温度高8位
		data[32] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//向8号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk08,8);
		delay_ms(100);
		//获取土壤湿度值
		data[33] = RS485_RX_BUF[3];//湿度高8位
		data[34] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[35] = RS485_RX_BUF[5];//温度高8位
		data[36] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//向9号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk09,8);
		delay_ms(100);
		//获取土壤湿度值
		data[37] = RS485_RX_BUF[3];//湿度高8位
		data[38] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[39] = RS485_RX_BUF[5];//温度高8位
		data[40] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//向10号土壤温湿度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliAsk10,8);
		delay_ms(100);
		//获取土壤湿度值
		data[41] = RS485_RX_BUF[3];//湿度高8位
		data[42] = RS485_RX_BUF[4];//湿度低8位
		//获取土壤温度值
		data[43] = RS485_RX_BUF[5];//温度高8位
		data[44] = RS485_RX_BUF[6];//温度低8位	
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//向11号土壤酸碱度传感器发送问询帧并保存返回的应答帧
		RS485_Send_Data(SoliPHAsk11,8);
		delay_ms(100);
		//获取土壤PH值
		data[45] = RS485_RX_BUF[3];//PH高8位
		data[46] = RS485_RX_BUF[4];//PH低8位
		//清空返回			
		for(i=0;i<64;i++)RS485_RX_BUF[i]=0;
		
		//向网关发送数据
		for(i=0;i<49;i++)
		{
			USART_SendData(USART1, data[i]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		
		//清除数据
		for(i=1;i<47;i++)
		{
			data[i] = 0;
		}
		Relay_On = 0;
}
