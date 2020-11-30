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
char *p_str = NULL;

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
			break;
		}
		
		
		/******************** ����ȷ�Ͻ��� ********************/
		case PAGE_SURE:
		{
			break;
		}
		
		/******************** ����ԴС�ݿ��ƽ��� ********************/
		case PAGE_CONTROL:
		{
			break;
		}
		
		/******************** ����ԴС����Ϣչʾ���� ********************/
		case PAGE_INFO:
		{   
			if (timeIndex == REFRESH_PHOTOV_INFO)
			{
				timeIndex = 0;
				
//				sprintf(charBuff, "t5.txt=\"%.3f V\"", p_vawPV->voltage);		/* ������� */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t7.txt=\"%.3f V\"", p_vawMT->voltage);		/* ������� */
//				SendHmiCmd((u8*)charBuff);
				
				sprintf(charBuff, "t5.txt=\"%.3f W\"", p_vawPV->power);           /* ������ʣ�1ƽ���ף� */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t6.txt=\"%.3f Wh\"", p_vawPV->wh);             /* �������(��ʱ) */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t7.txt=\"%.3f W\"", p_vawMT->power);           /* ������ʣ�1ƽ���ף� */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t8.txt=\"%.3f Wh\"", p_vawMT->wh);             /* �������(��ʱ) */
				SendHmiCmd((u8*)charBuff);
				
				p_vawPV->power = 0;
				p_vawMT->power = 0;
				
				
//				sprintf(charBuff, "t9.txt=\"%.3f V\"", p_vawPV->voltage);         /* ��ѹ */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t10.txt=\"%.3f mA\"", p_vawPV->current);        /* ���� */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t11.txt=\"%.3f W\"", p_vawPV->power);           /* ���� */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t12.txt=\"%.0f W/�O\"", p_envirParam->lightIntensity*(float)1.1/100);/* ����ǿ�� */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t13.txt=\"%.2f ��\"", p_envirParam->temp);    /* �¶� */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t14.txt=\"%.2f %%\"", p_envirParam->humi);    /* ʪ�� */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t15.txt=\"%.3f Wh\"", p_vawPV->wh);             /* ����(��ʱ) */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t16.txt=\"%d m\"", p_envirParam->altitude);   /* ���� */
//				SendHmiCmd((u8*)charBuff);
				
//				p_vawPV->voltage = 0;
//				p_vawPV->current = 0;
//				p_vawPV->power = 0;
				p_envirParam->lightIntensity = 0;
				p_envirParam->temp = 0;
				p_envirParam->humi = 0;
//				p_vawPV->wh = 0;
				p_envirParam->altitude = 0;
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
*Function Name  : CreateFile
*Author         : ����
*Version        : v1.0
*Date           : 2019-01-09
*Description    : ���������ı�
*Input          : serialNum: �ı����
*Output         : 
*Return         : 
*************************************************************/
void CreateFile(u8 serialNum)
{
	char charBuff[20];
	
	sprintf(charBuff, "0:/%d��ģ��.txt", serialNum);
	
	f_open(&fil, charBuff, FA_CREATE_ALWAYS | FA_WRITE);
	f_close(&fil);
}

#if 0
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
#endif

/*************************************************************
*Function Name  : PhoVolDataSava
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-02
*Description    : ������ģ����Ϣ��TFT��SD��
*                 ��������ѹ��V����������mA��������(W)������(Wh)����ǿ���¶ȡ�ʪ�ȡ�����
*Input          : serialNum: �ı����
*Output         : 
*Return         : 
*************************************************************/
void PhoVolDataSava(u8 serialNum)
{
    char fileBuff[20];
	char charBuff[60];
	sprintf(charBuff, "%.3f %.3f %.3f %.3f %.3f %.3f %.3f %d\r\n", p_vawPV->voltage, p_vawPV->current, p_vawPV->power, 
	p_vawPV->wh, p_envirParam->lightIntensity, p_envirParam->temp, p_envirParam->humi, p_envirParam->altitude);
	
	sprintf(fileBuff, "0:/%d��ģ��.txt", serialNum);
	res=f_open (&fil, fileBuff, FA_OPEN_EXISTING |FA_WRITE);	
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


/************************ �ݸ屾 ************************/
#if 0

//						/* ��"message.txt"�ļ������ڣ��򴴽����ļ� */
//						FILINFO fno;
//						if (FR_NO_FILE == f_stat("0:/message.txt", &fno))
//						{
//							f_open(&fil, "0:/message.txt", FA_CREATE_NEW);	
//							f_close(&fil);
//						}


	printf("%s", charBuff);
	printf("%d\r\n", GetStringLen(charBuff));

	res=f_open (&fil,"0:/message.txt", FA_OPEN_EXISTING |FA_WRITE);	
	res=f_lseek(&fil,f_size(&fil));
	f_write (&fil, charBuff, GetStringLen(charBuff), &bww);	
#endif

