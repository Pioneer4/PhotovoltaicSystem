/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : esp8266_uart.c
*Auther         : 张沁
*Version        : V1.0
*Date           : 2017-04-19
*Description    : 该文件对按键及按键中断进行了初始化
*Function List  : 
*Histoty		:
1. Date: 2017-04-15    Auther: 张沁
   Modifycation: 添加uCOS-II兼容程序
*************************************************************/

#include "myinclude.h"


/*************************************************************
*Function Name  : UART3_Init
*Auther         : 张沁
*Version        : V2.0
*Date           : 2017-04-16
*Description    : 用于uart3的初始化
*Input          ：1. bound  串口通信的波特率
*Output         ：
*Return         ：
*************************************************************/
void UART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIOA_InitStructure;
	USART_InitTypeDef UART3_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*******初始化GPIOB和USART3的时钟************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/*****复用GPIOB10和GPIOB11为USART3***********/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART1);       //PB10:Tx
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART1);       //PB11:Rx
	
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOA_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIOA_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
    GPIO_Init(GPIOB, &GPIOA_InitStructure);
	
	UART3_InitStructure.USART_BaudRate = bound;                      //波特率
	UART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件流控制
	UART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //接收和发送模式
	UART3_InitStructure.USART_Parity = USART_Parity_No;              //无奇偶校验
	UART3_InitStructure.USART_StopBits = USART_StopBits_1;           //1个停止位
	UART3_InitStructure.USART_WordLength = USART_WordLength_8b;      //8个字长
	
	USART_Init(USART3, &UART3_InitStructure);                        //UART1初始化
	
	/*******UART3的接收中断配置************/
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3, ENABLE);  //使能UART3
	
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}


/*************************************************************
*Function Name  : USART3_printf
*Auther         : 张沁
*Version        : V1.0
*Date           : 2017-04-19
*Description    : 格式化输出，类似于C库中的printf，但这里没有用到C库
                  能够完成 "\r" "\n"   "%d"  "%s" 的解析
*Input          ：1. *USARTx 串口通道
                  2. *Data   要发送到串口的内容的指针
                  3. ...     其他参数
*Output         ：
*Return         ：
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
*Auther         : 张沁
*Version        : V1.0
*Date           : 2017-04-19
*Description    : UART3数据发送
*Input          ：1. *data 要发送到内容的指针
                  2. len   数据长度
*Output         ：
*Return         ：
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
*Description    : 发送指令给HMI，其中封装了HMI要求的指令格式
*Input          : *cmd 指令指针
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
	
	/* 发送指令结束符 0xff 0xff 0xff */
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
*Description    : USART3的中断服务例程
*Input          ：
*Output         ：
*Return         ：
*Histoty        :
*************************************************************/
void USART3_IRQHandler(void)
{	u8 rev;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)     //如果接收寄存器不为空
	{
		rev = USART_ReceiveData(USART3);
		if (rev == 0x55)  /* 接收到HMI发送的用户指令 */
		{
			point3 = 0;
		}
		
		UART3_RX_BUF[point3++] = rev;  /* 数据存入接收缓冲区 */
		
		if (point3 == 3)
		{
			point3 = 0;
			if (UART3_RX_BUF[0] == 0x55)/* 防止错误数据,再次检验 */
			{
				switch (UART3_RX_BUF[1])
				{
					/* 关于触摸屏发送给单片机的状态指令 */
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
					
					/* 关于触摸屏发送给单片机的控制指令 */
					case 0x02:
					{
						break;
					}
					
					/* 关于触摸屏发送给单片机的“测试组别”信息 */
					case 0x03:      
					{
						if (UART3_RX_BUF[2]==0x01)       headInfoFlag = 1; /* 1号模组 */
						else if (UART3_RX_BUF[2]==0x02)  headInfoFlag = 2; /* 2号模组 */
						else if (UART3_RX_BUF[2]==0x03)  headInfoFlag = 3; /* 3号模组 */
						else if (UART3_RX_BUF[2]==0x04)  headInfoFlag = 4; /* 4号模组 */
						else if (UART3_RX_BUF[2]==0x05)  headInfoFlag = 5; /* 5号模组 */
						break;
					}
					default:
						break;
				}
			}
		}
	}
}
