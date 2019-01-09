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
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);          /* ��ƽ���� ���̵������մ���պϣ���������Ͽ��� */
}

/*************************************************************
*Function Name  : RelayPhovSwitch
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-25
*Description    : ���Ƽ̵���������������·������
*Input          ��BitAction: �̵���������ʽ
				  @arg OPEN_CONTACT : ���մ���պϣ���������Ͽ�
				  @arg CLOSE_CONTACT: ���մ���Ͽ�����������պ�
*Output         ��
*Return         ��
*************************************************************/
void RelayPhovSwitch(BitAction state) 
{
	if (state == OPEN_CONTACT)         /* ���մ���պϣ���������Ͽ� */
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	}
	else if (state == CLOSE_CONTACT)   /* ���մ���Ͽ�����������պ� */
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_0);
	}
}

/*************************************************************
*Function Name  : RelaySampSwitch
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-25
*Description    : ���Ƽ̵����������л���·������
*Input          ��BitAction: �̵���������ʽ
				  @arg OPEN_CONTACT : ���մ���պϣ���������Ͽ�
				  @arg CLOSE_CONTACT: ���մ���Ͽ�����������պ�
*Output         ��
*Return         ��
*************************************************************/
void RelaySampSwitch(BitAction state) 
{
	if (state == OPEN_CONTACT)         /* ���մ���պϣ���������Ͽ� */
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	}
	else if (state == CLOSE_CONTACT)   /* ���մ���Ͽ�����������պ� */
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_1);
	}
}

