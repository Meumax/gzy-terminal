#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
/******************************************************************
广东工业大学，实验三号楼  801实验室
项目：				调试每个串口用的		 
开发人员：@Meumax，@Edmundshi
创建日期：2018.12.26
版权所有，盗版必究
******************************************************************/
#define LED0 PEout(5)// PE5
#define LED1 PEout(6)// PE6	

#define Relay_On PDout(5)// PD5

void LED_Init(void);//初始化

		 				    
#endif
