#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ������ҽ��������Ŀ��λ��ʹ�ã�δ��������ɣ��������������κ���;
//RS485���� ����	   
//�޸�����:2018/10/17
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
