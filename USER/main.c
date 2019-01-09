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
	p_vaw = &vaw;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 系统中断优先级分组2 */
	RelayInit();                                    /* 初始化继电器控制端口 */
	UART1_Init(115200);                             /* 初始化串口1 */
	UART3_Init(115200);                             /* 初始化串口3(与HMI通信) */
	delay_init(168);                                /* 系统频率168MHz */
	LedInit();
	KeyInit();
	PwmLedInit();
	LCD_Init();				  						/* 初始化LCD */
	AdcInit();
	TIM7_Init();
	//tp_dev.init();            						/* 初始化触摸屏 */
	TIM6_Init();
	IwdgInit(4,1500);           					/* 初始化iwdg，超时周期3S */
	GY39UartInit();                                 /* 初始化UART4（与GY-39通信） */
	Load_Drow_Dialog();	      						/* LCD右上角RST清屏标志 */
	
	SendHmiCmd((u8*)"page logo");                   /* HMI返回首页 */
	
	while(1)
	{	
		
	}
		
}		

/******************************* draft（草稿区） *******************************/
#if 0
printf("lightIntensity: %.2f\r\n", p_envirParam->lightIntensity);
			printf("temp: %.2f\r\n", p_envirParam->temp);
			printf("humi: %.2f\r\n", p_envirParam->humi);
			printf("pressure: %.2f\r\n", p_envirParam->pressure);
			printf("altitude: %d\r\n", p_envirParam->altitude);

u8 res=0;
	u8 testIndex = 0;
	char charBuff[20];  
	for (testIndex=0; testIndex<10; testIndex++) 
	{
		sprintf(charBuff, "Hybrid Forklift: %d\r\n", testIndex);
		res=f_open (&fil,"0:/message.txt", FA_OPEN_EXISTING |FA_WRITE);	
		res=f_lseek(&fil,f_size(&fil));
		f_write (&fil, charBuff, 20, &bww);
		f_close(&fil);
		memset(charBuff, 0, sizeof(charBuff));
		delay_ms(50);
	};
	LCD_ShowString(10,210,230,24,24,(u8 *)"save OK!");
	
	
	LCD_ShowString(10, 10, 80, 16, 16, (u8*)"0x18FF8F03");
		LCD_ShowString(10, 40, 80, 16, 16, (u8*)"0x18FF8E03");
		LCD_ShowString(10, 70, 80, 16, 16, (u8*)"0x18FF8D03");
		
		LCD_DrawRectangle(10, 10, 90, 26);
		LCD_DrawRectangle(10, 40, 90, 56);
		LCD_DrawRectangle(10, 70, 90, 86);
		
		LCD_ShowString(30,150,200,16,16, (u8*)"                      ");
		sprintf(charBuff, "vol: %.3f", p_vaw->voltage);          /* 电压 */
		LCD_ShowString(30,150,200,16,16, (u8*)charBuff);	//先在固定位置显示小数点    
		
		if (flagTP & 0x01)
		{	
			flagTP = 0x00;
			
			tp_dev.scan(0);
			if (tp_dev.x[0]>(lcddev.width-24) && tp_dev.y[0]<16)
			{
				Load_Drow_Dialog();     //清除
				brightControl(0);
				
				//state = PAGE_DCHRG_BASIC;
			} 
			else if (tp_dev.x[0]>10 && tp_dev.x[0]<90 && tp_dev.y[0]>10 && tp_dev.y[0]<26)
			{
				brightControl(99);
			}
			else if (tp_dev.x[0]>10 && tp_dev.x[0]<90 && tp_dev.y[0]>40 && tp_dev.y[0]<56)
			{
				
			}
			else if (tp_dev.x[0]>10 && tp_dev.x[0]<90 && tp_dev.y[0]>70 && tp_dev.y[0]<86)
			{
				
			}
			else 
			{
				brightControl(50);
				TP_Draw_Big_Point(tp_dev.x[0], tp_dev.y[0], RED);
			}
		}
		delay_ms(50);
		
		u8 val = 0;
		for (val=0; val<10; val++)
		{
			brightControl(val*10);
			delay_ms(2000);
		}
	
#endif

