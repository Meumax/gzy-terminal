#include "led.h"

/******************************************************************
广东工业大学，实验三号楼  801实验室
项目：				调试每个串口用的		 
开发人员：@Meumax，@Edmundshi
创建日期：2018.12.26
版权所有，盗版必究
******************************************************************/

//初始化PE5和PE6为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PE.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOE,GPIO_Pin_5);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	    		 //LED1-->PE.6 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_6); 						 //PE.5 输出高 
}
 
