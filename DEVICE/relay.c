/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : realy.c
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-25
*Description    : ���ļ������˼̵������ƽӿڵ���غ���
*Function List  : 
*************************************************************/

#include "myinclude.h"


/*************************************************************
*Function Name  : RelayInit
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-25
*Description    : ��ʼ��RelayPhov(PE0)��RelaySamp(PE1)
				  1.RelayPhov�̵��������ڱ���������·�������������
				  �̵����Ͽ������Թ�����·
				  2.RelaySamp�̵��������ڲ�����·�л�
*Input          ��
*Output         ��
*Return         ��
*************************************************************/
void RelayInit(void)
{
	GPIO_InitTypeDef GPIOE_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIOE_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           /* ���ģʽ */
	GPIOE_InitStructure.GPIO_OType = GPIO_OType_PP;          /* ������� */
	GPIOE_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  /* GPIOE0��GPIOE1 */
	GPIOE_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            /* ���� */
	GPIOE_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      /* �ٶ�100M */
	
	GPIO_Init(GPIOE, &GPIOE_InitStructure);                  /* ��ʼ��PE0��PE1 */
	
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);          /* ��ƽ���� ���̵������մ���պϣ���������Ͽ��� */
}

/*************************************************************
*Function Name  : LedSwitch
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-25
*Description    : ���Ƽ̵���������������·������
*Input          ��BitAction: �̵���������ʽ
				  @arg ON : ��LED��
				  @arg OFF: �ر�LED��
*Output         ��
*Return         ��
*************************************************************/
void LedSwitch(BitAction state) 
{
	if (state == OFF)        
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_0);
	}
	else if (state == ON)   
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	}
}

/*************************************************************
*Function Name  : WarmSwitch
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-25
*Description    : ���Ƽ̵����������л���·������
*Input          ��BitAction: �̵���������ʽ
				  @arg ON : ��ů��
				  @arg OFF: �ر�ů��
*Output         ��
*Return         ��
*************************************************************/
void WarmSwitch(BitAction state) 
{
	if (state == OFF)        
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_1);
	}
	else if (state == ON)   
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	}
}
