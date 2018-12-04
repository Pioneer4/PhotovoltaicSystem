/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : game.c
*Author         : Walter
*Version        : v1.0
*Date           : 2018-11-24
*Description    : ���ļ��а����˵�Ƭ����hmi���ݽ���״̬��
*Function List  : 
*History        :
*************************************************************/
#include "myinclude.h"

/*************************************************************
*Function Name  : HmiChatGame
*Author         : Walter
*Version        : v1.0
*Date           : 2018-11-22
*Description    : ��Ƭ����hmi���ݽ���״̬����50ms����һ�Ρ�
*  	              ͨ��timeIndex���Ƹ���״̬����ʱ�䡣
*Input          : 
*Output         : 
*Return         : 
*************************************************************/
void HmiChatGame(void)
{
	char charBuff[20];  /* �����ַ���ת�� */
	
	switch (state)
	{
		/******************** logo���� ********************/
		case PAGE_LOGO:
		{
			break;
		}
		
		/******************** menu���� ********************/
		case PAGE_MENU:
		{
			if (timeIndex == REFRESH_MENU)
			{
				timeIndex = 0;
				
				if (SD_Init())  /* ��ⲻ��SD�� */
				{
					SendHmiCmd((u8*)"vis t1,1");
					sdInitFlag = 0;
				}
				else
				{
					/* ��δ��ʼ��SD�� */
					if (sdInitFlag == 0)     
					{	
						exfuns_init();        /* Ϊfatfs��ر��������ڴ� */
						f_mount(fs[0],"0:",1);/* ����SD�� */
						
						/* ��"message.txt"�ļ������ڣ��򴴽����ļ� */
						FILINFO fno;
						if (FR_NO_FILE == f_stat("0:/message.txt", &fno))
						{
							f_open(&fil, "0:/message.txt", FA_CREATE_NEW);	
							f_close(&fil);
						}
						sdInitFlag = 1;
					}
					
					SendHmiCmd((u8*)"vis t1,0");
				}	
			}
			else 
			{
				timeIndex++;
			}
			break;
		}
		
		/******************** ϵͳ���ý��� ********************/
		case PAGE_SYS_CFG:
		{
			break;
		}
		
		
		/******************** ����ȷ�Ͻ��� ********************/
		case PAGE_SURE:
		{
			break;
		}
		
		/******************** ���ģ����Ϣչʾ���� ********************/
		case PAGE_INFO:
		{   
			if (timeIndex == REFRESH_PHOTOV_INFO)
			{
				timeIndex = 0;
				
				if (headInfoFlag != 0)
				{
					HeaderInfoSava(headInfoFlag);
					headInfoFlag = 0;
				}
				
				PhoVolDataSava();  /* �������ݵ�FTF��SD�� */
				
				sprintf(charBuff, "t9.txt=\"%.3f V\"", p_vaw->voltage);          /* ��ѹ */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t10.txt=\"%.3f A\"", p_vaw->current);         /* ���� */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t11.txt=\"%.3f W\"", p_vaw->power);           /* ���� */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t12.txt=\"%.2f lux\"", p_envirParam->lightIntensity);/* ����ǿ�� */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t13.txt=\"%.2f ��\"", p_envirParam->temp);    /* �¶� */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t14.txt=\"%.2f %%\"", p_envirParam->humi);    /* ʪ�� */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t15.txt=\"%.2f Pa\"", p_envirParam->pressure);  /* ��ѹ */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t16.txt=\"%d m\"", p_envirParam->altitude);   /* ���� */
				SendHmiCmd((u8*)charBuff);
			}
			else
			{
				timeIndex++;
			}
			break;
		}
			
		default:
			break;
		
	}
}

/*************************************************************
*Function Name  : HeaderInfoSava
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-02
*Description    : ������Ϣͷ("x��ģ��")
*Input          : serialNum: ģ����
*Output         : 
*Return         : 
*************************************************************/
void HeaderInfoSava(u8 serialNum)
{
	char charBuff[10];
	
	sprintf(charBuff, "%d��ģ��\r\n", serialNum);	
	res=f_open (&fil,"0:/message.txt", FA_OPEN_EXISTING |FA_WRITE);	
	res=f_lseek(&fil,f_size(&fil));
	f_write (&fil, charBuff, GetStringLen(charBuff), &bww);
	f_close(&fil);
}

/*************************************************************
*Function Name  : PhoVolDataSava
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-02
*Description    : ������ģ����Ϣ��TFT��SD��
*                 ��������ѹ�����������ʡ���ǿ���¶ȡ�ʪ�ȡ���ѹ������
*Input          : 
*Output         : 
*Return         : 
*************************************************************/
void PhoVolDataSava(void)
{
	char charBuff[60];
	sprintf(charBuff, "%.3f %.3f %.3f %.3f %.3f %.3f %.3f %d\r\n", p_vaw->voltage, p_vaw->current, p_vaw->power, 
	p_envirParam->lightIntensity, p_envirParam->temp, p_envirParam->humi, p_envirParam->pressure, p_envirParam->altitude);
	
	printf("%s", charBuff);
	printf("%d\r\n", GetStringLen(charBuff));
	
	res=f_open (&fil,"0:/message.txt", FA_OPEN_EXISTING |FA_WRITE);	
	res=f_lseek(&fil,f_size(&fil));
	f_write (&fil, charBuff, GetStringLen(charBuff), &bww);
	f_close(&fil);
}


/*************************************************************
*Function Name  : GetStringLen
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-02
*Description    : �õ��ַ����ĳ��ȣ����з�'\n'Ϊ������־��
*Input          : *str �ַ���ָ��
*Output         : strָ����ַ����ĳ���
*Return         : 
*************************************************************/
u8 GetStringLen(char *str)
{
	u8 index = 0;
	char *str1 = strchr(str, '\n');
	index = str1 - str;

	return (index+1);
}