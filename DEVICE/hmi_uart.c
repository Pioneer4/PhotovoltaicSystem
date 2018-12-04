/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : esp8266_uart.c
*Auther         : ����
*Version        : V1.0
*Date           : 2017-04-19
*Description    : ���ļ��԰����������жϽ����˳�ʼ��
*Function List  : 
*Histoty		:
1. Date: 2017-04-15    Auther: ����
   Modifycation: ���uCOS-II���ݳ���
*************************************************************/

#include "myinclude.h"


/*************************************************************
*Function Name  : UART3_Init
*Auther         : ����
*Version        : V2.0
*Date           : 2017-04-16
*Description    : ����uart3�ĳ�ʼ��
*Input          ��1. bound  ����ͨ�ŵĲ�����
*Output         ��
*Return         ��
*************************************************************/
void UART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIOA_InitStructure;
	USART_InitTypeDef UART3_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*******��ʼ��GPIOB��USART3��ʱ��************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/*****����GPIOB10��GPIOB11ΪUSART3***********/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART1);       //PB10:Tx
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART1);       //PB11:Rx
	
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOA_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIOA_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
    GPIO_Init(GPIOB, &GPIOA_InitStructure);
	
	UART3_InitStructure.USART_BaudRate = bound;                      //������
	UART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ��������
	UART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //���պͷ���ģʽ
	UART3_InitStructure.USART_Parity = USART_Parity_No;              //����żУ��
	UART3_InitStructure.USART_StopBits = USART_StopBits_1;           //1��ֹͣλ
	UART3_InitStructure.USART_WordLength = USART_WordLength_8b;      //8���ֳ�
	
	USART_Init(USART3, &UART3_InitStructure);                        //UART1��ʼ��
	
	/*******UART3�Ľ����ж�����************/
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3, ENABLE);  //ʹ��UART3
	
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}


/*************************************************************
*Function Name  : USART3_printf
*Auther         : ����
*Version        : V1.0
*Date           : 2017-04-19
*Description    : ��ʽ�������������C���е�printf��������û���õ�C��
                  �ܹ���� "\r" "\n"   "%d"  "%s" �Ľ���
*Input          ��1. *USARTx ����ͨ��
                  2. *Data   Ҫ���͵����ڵ����ݵ�ָ��
                  3. ...     ��������
*Output         ��
*Return         ��
*************************************************************/
void USART3_printf( USART_TypeDef* USARTx, char *Data, ... )
{
	char buf[128];
	char *ptr = buf;

	va_list ap;
	va_start(ap, Data);

	vsprintf(buf, Data, ap);
	va_end(ap);
	
	while (*ptr != '\0')
	{
		USART_SendData(USARTx, *ptr++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}

/*************************************************************
*Function Name  : USART3_Send_Data
*Auther         : ����
*Version        : V1.0
*Date           : 2017-04-19
*Description    : UART3���ݷ���
*Input          ��1. *data Ҫ���͵����ݵ�ָ��
                  2. len   ���ݳ���
*Output         ��
*Return         ��
*************************************************************/
void USART3_Send_Data(u8 *data, u8 len)
{
	u8 i=0;
	
	while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
	
	for(i = 0; i < len; i++)
	{
		USART_SendData(USART3,	data[i]);
		while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
	}
}

/*************************************************************
*Function Name  : SendHmiCmd
*Author         : Walter
*Version        : v1.0
*Date           : 2018-11-24
*Description    : ����ָ���HMI�����з�װ��HMIҪ���ָ���ʽ
*Input          : *cmd ָ��ָ��
*Output         : 
*Return         : 
*************************************************************/
void SendHmiCmd(u8 *cmd)
{
	int i = 0;
	
	while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
	
	while (*cmd != '\0')
	{
		USART_SendData(USART3, *cmd++);
		while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
	}
	
	/* ����ָ������� 0xff 0xff 0xff */
	for (i=0; i<3; i++)
	{
		USART_SendData(USART3, 0xff);
		while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
	}
	
}


/*************************************************************
*Function Name  : USART3_IRQHandler
*Author         : Walter
*Version        : v1.0
*Date           : 2018-11-24
*Description    : USART3���жϷ�������
*Input          ��
*Output         ��
*Return         ��
*Histoty        :
*************************************************************/
void USART3_IRQHandler(void)
{	u8 rev;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)     //������ռĴ�����Ϊ��
	{
		rev = USART_ReceiveData(USART3);
		if (rev == 0x55)  /* ���յ�HMI���͵��û�ָ�� */
		{
			point3 = 0;
		}
		
		UART3_RX_BUF[point3++] = rev;  /* ���ݴ�����ջ����� */
		
		if (point3 == 3)
		{
			point3 = 0;
			if (UART3_RX_BUF[0] == 0x55)/* ��ֹ��������,�ٴμ��� */
			{
				switch (UART3_RX_BUF[1])
				{
					/* ���ڴ��������͸���Ƭ����״ָ̬�� */
					case 0x01:
					{
						if (UART3_RX_BUF[2]==0x00) state = PAGE_LOGO;
						else if (UART3_RX_BUF[2]==0x01) state = PAGE_MENU;
						else if (UART3_RX_BUF[2]==0x02) state = PAGE_SYS_CFG;
						else if (UART3_RX_BUF[2]==0x03) state = PAGE_SURE;
						else if (UART3_RX_BUF[2]==0x04) state = PAGE_INFO;
						timeIndex = 0;
						
						break;
					}
					
					/* ���ڴ��������͸���Ƭ���Ŀ���ָ�� */
					case 0x02:
					{
						break;
					}
					
					/* ���ڴ��������͸���Ƭ���ġ����������Ϣ */
					case 0x03:      
					{
						if (UART3_RX_BUF[2]==0x01)       headInfoFlag = 1; /* 1��ģ�� */
						else if (UART3_RX_BUF[2]==0x02)  headInfoFlag = 2; /* 2��ģ�� */
						else if (UART3_RX_BUF[2]==0x03)  headInfoFlag = 3; /* 3��ģ�� */
						else if (UART3_RX_BUF[2]==0x04)  headInfoFlag = 4; /* 4��ģ�� */
						else if (UART3_RX_BUF[2]==0x05)  headInfoFlag = 5; /* 5��ģ�� */
						break;
					}
					default:
						break;
				}
			}
		}
	}
}
