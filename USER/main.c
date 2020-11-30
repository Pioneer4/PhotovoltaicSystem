/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : main.c
*Author         : 张沁
*Version        : v1.0
*Date           : 
*Description    : 光伏模组测试系统(不带操作系统)
                  系统频率设置为168MHz
*Function List  : 2018-11-30
*History        :
*************************************************************/
#define CPP_GLOBALS

#include "myinclude.h"

int main(void)
{	
	p_envirParam = &envirParam;                     /* 指针操作，加速访问 */
	p_vawPV = &vawPV;
	p_vawMT = &vawMT;
	coefficient = 500;								/* 光伏板面积 25mm * 80mm = 0.002㎡ */
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 系统中断优先级分组2 */
	RelayInit();                                    /* 初始化继电器控制端口 */
	UART1_Init(115200);                             /* 初始化串口1 */
	UART3_Init(115200);                             /* 初始化串口3(与HMI通信) */
	delay_init(168);                                /* 系统频率168MHz */
	LedInit();
	KeyInit();
	PwmLedInit();
	PwmMosInit();
	LCD_Init();				  						/* 初始化LCD */
	AdcInit();
	TIM7_Init();
	//tp_dev.init();            						/* 初始化触摸屏 */
	TIM6_Init();
	IwdgInit(4,1500);           					/* 初始化iwdg，超时周期3S */
	GY39UartInit();                                 /* 初始化UART4（与GY-39通信） */
	Load_Drow_Dialog();	      						/* LCD右上角RST清屏标志 */
	
	SendHmiCmd((u8*)"page logo");                   /* HMI返回首页 */
	
	printf("开始测试：\r\n");

	while(1)
	{	
	}
		
}		

/******************************* draft（草稿区） *******************************/
#if 0

#endif

