#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供广中医物联网项目下位机使用，未经作者许可，不得用于其它任何用途
//RS485驱动 代码	   
//修改日期:2018/10/17
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 孔立斌
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
