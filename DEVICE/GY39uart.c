/*************************************************************
*     Open source but not allowed to modify
*File name      : GY39uart.c
*Auther         : 张沁
*Version        : v1.0
*Date           : 2017-08-05
*Description    : UART4读取GY-39数据
                  包括光强、温度、湿度、气压、海拔
*Function List  : 
*************************************************************/

#include "myinclude.h"

/*************************************************************
*Function Name  : GY39UartInit
*Auther         : 张沁
*Vertion        : v1.0
*Date           : 2018-12-01
*Description    : 初始化UART4，用与GY-39通信    波特率:115200
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void GY39UartInit(void)
{
    GPIO_InitTypeDef GPIOC_InitStructure;
	USART_InitTypeDef UART4_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*******初始化GPIOC和UART4的时钟************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	/*****复用GPIOC10和GPIOC11为UART4***********/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
	
	GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOC_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIOC_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
    GPIO_Init(GPIOA, &GPIOC_InitStructure);
	
	UART4_InitStructure.USART_BaudRate = 115200;                   //波特率(GY-39为115200)
	UART4_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件流控制
	UART4_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收和发送模式
	UART4_InitStructure.USART_Parity = USART_Parity_No;            //无奇偶校验
	UART4_InitStructure.USART_StopBits = USART_StopBits_1;         //1个停止位
	UART4_InitStructure.USART_WordLength = USART_WordLength_8b;    //8个字长
	
	USART_Init(UART4, &UART4_InitStructure); 
	
	/*******UART1的接收中断配置************/
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;   //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;          //子优先级1
	
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART4, ENABLE);  //使能UART4
}

/*************************************************************
*Function Name  : UART4_IRQHandler
*Auther         : 张沁
*Vertion        : v2.0
*Date           : 2018-12-01
*Description    : UART4中断服务例程 
*                 用于接收GY-39发送的数据并解析
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
u8 point4 = 0;
void UART4_IRQHandler(void)
{
	int temp = 0;     //类型转换中间量
	
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)      /* 如果接收寄存器不为空 */
	{
		if (point4 == UART4_REC_LEN) point4 = 0;               /* 接收缓冲区溢出，则证明出错，重新接收 */
		UART4_RX_BUF[point4++] = USART_ReceiveData(UART4);     /* 接收数据 */
		
		if (UART4_RX_BUF[0] != 0x5a)                point4 = 0;/* 帧头错误 */
		if ((point4==2) && (UART4_RX_BUF[0]!=0x5a)) point4 = 0;/* 帧头错误 */
		
		 /* point4等于4时，已经接收到数据量字UART4_RX_BUF[3] */
		if (point4 > 3)  									  
		{
			if (point4 != (UART4_RX_BUF[3]+5)) return;	       /* 判断是否接收一帧数据完毕 */
			switch (UART4_RX_BUF[2])
			{
				case 0x15:
				{
					temp = UART4_RX_BUF[4];
					temp = (temp << 8) + UART4_RX_BUF[5];
					temp = (temp << 8) + UART4_RX_BUF[6];
					temp = (temp << 8) + UART4_RX_BUF[7];
					p_envirParam->lightIntensity = (float)temp / 100;
					
					break;
				}
				
				case 0x45:
				{
					temp = UART4_RX_BUF[4];
					temp = (temp << 8) + UART4_RX_BUF[5];
					p_envirParam->temp = (float)temp / 100;
					
					temp = UART4_RX_BUF[6];
					temp = (temp << 8) + UART4_RX_BUF[7];
					temp = (temp << 8) + UART4_RX_BUF[8];
					temp = (temp << 8) + UART4_RX_BUF[9];
					p_envirParam->pressure = (float)temp / 100;
					
					temp = UART4_RX_BUF[10];
					temp = (temp << 8) + UART4_RX_BUF[11];
					p_envirParam->humi = (float)temp / 100;
					
					temp = UART4_RX_BUF[12];
					temp = (temp << 8) + UART4_RX_BUF[13];
					p_envirParam->altitude = (short)temp;
					
					break;
				}
				
				default:
					break;
			}
			point4 = 0;
		}
	}
}


