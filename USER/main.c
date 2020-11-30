/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : main.c
*Author         : ����
*Version        : v1.0
*Date           : 
*Description    : ���ģ�����ϵͳ(��������ϵͳ)
                  ϵͳƵ������Ϊ168MHz
*Function List  : 2018-11-30
*History        :
*************************************************************/
#define CPP_GLOBALS

#include "myinclude.h"

int main(void)
{	
	p_envirParam = &envirParam;                     /* ָ����������ٷ��� */
	p_vawPV = &vawPV;
	p_vawMT = &vawMT;
	coefficient = 500;								/* �������� 25mm * 80mm = 0.002�O */
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* ϵͳ�ж����ȼ�����2 */
	RelayInit();                                    /* ��ʼ���̵������ƶ˿� */
	UART1_Init(115200);                             /* ��ʼ������1 */
	UART3_Init(115200);                             /* ��ʼ������3(��HMIͨ��) */
	delay_init(168);                                /* ϵͳƵ��168MHz */
	LedInit();
	KeyInit();
	PwmLedInit();
	PwmMosInit();
	LCD_Init();				  						/* ��ʼ��LCD */
	AdcInit();
	TIM7_Init();
	//tp_dev.init();            						/* ��ʼ�������� */
	TIM6_Init();
	IwdgInit(4,1500);           					/* ��ʼ��iwdg����ʱ����3S */
	GY39UartInit();                                 /* ��ʼ��UART4����GY-39ͨ�ţ� */
	Load_Drow_Dialog();	      						/* LCD���Ͻ�RST������־ */
	
	SendHmiCmd((u8*)"page logo");                   /* HMI������ҳ */
	
	printf("��ʼ���ԣ�\r\n");

	while(1)
	{	
	}
		
}		

/******************************* draft���ݸ����� *******************************/
#if 0

#endif

