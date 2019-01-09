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
	p_vaw = &vaw;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* ϵͳ�ж����ȼ�����2 */
	RelayInit();                                    /* ��ʼ���̵������ƶ˿� */
	UART1_Init(115200);                             /* ��ʼ������1 */
	UART3_Init(115200);                             /* ��ʼ������3(��HMIͨ��) */
	delay_init(168);                                /* ϵͳƵ��168MHz */
	LedInit();
	KeyInit();
	PwmLedInit();
	LCD_Init();				  						/* ��ʼ��LCD */
	AdcInit();
	TIM7_Init();
	//tp_dev.init();            						/* ��ʼ�������� */
	TIM6_Init();
	IwdgInit(4,1500);           					/* ��ʼ��iwdg����ʱ����3S */
	GY39UartInit();                                 /* ��ʼ��UART4����GY-39ͨ�ţ� */
	Load_Drow_Dialog();	      						/* LCD���Ͻ�RST������־ */
	
	SendHmiCmd((u8*)"page logo");                   /* HMI������ҳ */
	
	while(1)
	{	
		
	}
		
}		

/******************************* draft���ݸ����� *******************************/
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
		sprintf(charBuff, "vol: %.3f", p_vaw->voltage);          /* ��ѹ */
		LCD_ShowString(30,150,200,16,16, (u8*)charBuff);	//���ڹ̶�λ����ʾС����    
		
		if (flagTP & 0x01)
		{	
			flagTP = 0x00;
			
			tp_dev.scan(0);
			if (tp_dev.x[0]>(lcddev.width-24) && tp_dev.y[0]<16)
			{
				Load_Drow_Dialog();     //���
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

