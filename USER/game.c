/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : game.c
*Author         : Walter
*Version        : v1.0
*Date           : 2018-11-24
*Description    : 该文件中包含了单片机与hmi数据交互状态机
*Function List  : 
*History        :
*************************************************************/
#include "myinclude.h"

/*************************************************************
*Function Name  : HmiChatGame
*Author         : Walter
*Version        : v1.0
*Date           : 2018-11-22
*Description    : 单片机与hmi数据交互状态机，50ms运行一次。
*  	              通过timeIndex控制各个状态运行时间。
*Input          : 
*Output         : 
*Return         : 
*************************************************************/
char *p_str = NULL;

void HmiChatGame(void)
{
	char charBuff[20];  /* 用于字符串转换 */
	
	switch (state)
	{
		/******************** logo界面 ********************/
		case PAGE_LOGO:
		{
			break;
		}
		
		/******************** menu界面 ********************/
		case PAGE_MENU:
		{
			break;
		}
		
		
		/******************** 操作确认界面 ********************/
		case PAGE_SURE:
		{
			break;
		}
		
		/******************** 新能源小屋控制界面 ********************/
		case PAGE_CONTROL:
		{
			break;
		}
		
		/******************** 新能源小屋信息展示界面 ********************/
		case PAGE_INFO:
		{   
			if (timeIndex == REFRESH_PHOTOV_INFO)
			{
				timeIndex = 0;
				
//				sprintf(charBuff, "t5.txt=\"%.3f V\"", p_vawPV->voltage);		/* 光伏功率 */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t7.txt=\"%.3f V\"", p_vawMT->voltage);		/* 风机功率 */
//				SendHmiCmd((u8*)charBuff);
				
				sprintf(charBuff, "t5.txt=\"%.3f W\"", p_vawPV->power);           /* 光伏功率（1平方米） */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t6.txt=\"%.3f Wh\"", p_vawPV->wh);             /* 光伏电量(瓦时) */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t7.txt=\"%.3f W\"", p_vawMT->power);           /* 风机功率（1平方米） */
				SendHmiCmd((u8*)charBuff);
				sprintf(charBuff, "t8.txt=\"%.3f Wh\"", p_vawMT->wh);             /* 风机电量(瓦时) */
				SendHmiCmd((u8*)charBuff);
				
				p_vawPV->power = 0;
				p_vawMT->power = 0;
				
				
//				sprintf(charBuff, "t9.txt=\"%.3f V\"", p_vawPV->voltage);         /* 电压 */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t10.txt=\"%.3f mA\"", p_vawPV->current);        /* 电流 */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t11.txt=\"%.3f W\"", p_vawPV->power);           /* 功率 */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t12.txt=\"%.0f W/㎡\"", p_envirParam->lightIntensity*(float)1.1/100);/* 光照强度 */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t13.txt=\"%.2f ℃\"", p_envirParam->temp);    /* 温度 */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t14.txt=\"%.2f %%\"", p_envirParam->humi);    /* 湿度 */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t15.txt=\"%.3f Wh\"", p_vawPV->wh);             /* 电量(瓦时) */
//				SendHmiCmd((u8*)charBuff);
//				sprintf(charBuff, "t16.txt=\"%d m\"", p_envirParam->altitude);   /* 海拔 */
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
*Author         : 张沁
*Version        : v1.0
*Date           : 2019-01-09
*Description    : 创建测试文本
*Input          : serialNum: 文本编号
*Output         : 
*Return         : 
*************************************************************/
void CreateFile(u8 serialNum)
{
	char charBuff[20];
	
	sprintf(charBuff, "0:/%d号模组.txt", serialNum);
	
	f_open(&fil, charBuff, FA_CREATE_ALWAYS | FA_WRITE);
	f_close(&fil);
}

#if 0
/*************************************************************
*Function Name  : HeaderInfoSava
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-02
*Description    : 保存信息头("x号模组")
*Input          : serialNum: 模组编号
*Output         : 
*Return         : 
*************************************************************/
void HeaderInfoSava(u8 serialNum)
{
	char charBuff[10];
	
	sprintf(charBuff, "%d号模组\r\n", serialNum);	
	res=f_open (&fil,"0:/message.txt", FA_OPEN_EXISTING |FA_WRITE);	
	res=f_lseek(&fil,f_size(&fil));
	f_write (&fil, charBuff, GetStringLen(charBuff), &bww);
	f_close(&fil);
}
#endif

/*************************************************************
*Function Name  : PhoVolDataSava
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-02
*Description    : 保存光伏模组信息至TFT或SD卡
*                 包括：电压（V）、电流（mA）、功率(W)、电量(Wh)、光强、温度、湿度、海拔
*Input          : serialNum: 文本编号
*Output         : 
*Return         : 
*************************************************************/
void PhoVolDataSava(u8 serialNum)
{
    char fileBuff[20];
	char charBuff[60];
	sprintf(charBuff, "%.3f %.3f %.3f %.3f %.3f %.3f %.3f %d\r\n", p_vawPV->voltage, p_vawPV->current, p_vawPV->power, 
	p_vawPV->wh, p_envirParam->lightIntensity, p_envirParam->temp, p_envirParam->humi, p_envirParam->altitude);
	
	sprintf(fileBuff, "0:/%d号模组.txt", serialNum);
	res=f_open (&fil, fileBuff, FA_OPEN_EXISTING |FA_WRITE);	
	res=f_lseek(&fil,f_size(&fil));
	f_write (&fil, charBuff, GetStringLen(charBuff), &bww);	
	f_close(&fil);
}


/*************************************************************
*Function Name  : GetStringLen
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-02
*Description    : 得到字符串的长度（换行符'\n'为结束标志）
*Input          : *str 字符串指针
*Output         : str指向的字符串的长度
*Return         : 
*************************************************************/
u8 GetStringLen(char *str)
{
	u8 index = 0;
	char *str1 = strchr(str, '\n');
	index = str1 - str;

	return (index+1);
}


/************************ 草稿本 ************************/
#if 0

//						/* 若"message.txt"文件不存在，则创建该文件 */
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

