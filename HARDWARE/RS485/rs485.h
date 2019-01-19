#ifndef __RS485_2_H
#define __RS485_2_H			 
#include "sys.h"	 								  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供广中医物联网项目下位机使用，未经作者许可，不得用于其它任何用途
//RS485驱动 代码	   
//修改日期:2018/10/17
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 石鸣鸣
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	  		  	
extern u8 RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度

//模式控制
#define RS485_TX_EN2		PDout(7)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 	1			//0,不接收;1,接收.


void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);


#endif	   

