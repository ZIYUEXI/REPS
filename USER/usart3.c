#include "stm32f10x.h"
#include "usart3.h"
#include "usart.h"

u16 USART2_RX_STA=0;
u8 usart2_buffer[USART_REC_LEN];

_ttywrch(int ch)
{
    ch = ch;
}


void USART_SendString(USART_TypeDef* USARTx, char* str)
{
    while(*str)
    {
        USART_SendData(USARTx, *str++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}

void Init_MD0_MD1(void){
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // 配置PC7为输出低电平
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);

    // 配置PC8为输出高电平
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_8);
}


void USART2_Init(uint32_t bunld){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_INitstrute;      //串口使用中断
  // 使能USART2和GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  // 配置USART2引脚
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
		  //USART1_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//
  GPIO_Init(GPIOA, &GPIO_InitStructure);//

  // 配置USART2参数
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure);
	

  // 使能USART2
  USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);  //清除标志位
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组
	NVIC_INitstrute.NVIC_IRQChannel= USART2_IRQn ;  //打开UART1的中断
	NVIC_INitstrute.NVIC_IRQChannelCmd=ENABLE;
	NVIC_INitstrute.NVIC_IRQChannelPreemptionPriority=1;  //抢占优先级
	NVIC_INitstrute.NVIC_IRQChannelSubPriority=1;          //响应优先级
	NVIC_Init(&NVIC_INitstrute); 
	Init_MD0_MD1();
}

void USART_Clear(USART_TypeDef* USARTx)
{
    uint16_t dummy = USARTx->DR;
    (void)dummy; // 避免编译器提示未使用的变量
    USART_ClearFlag(USARTx, USART_FLAG_RXNE);
}

// 发送函数
void USART_SenStr(USART_TypeDef* USARTx, u8 *str)
{
	int len = strlen(str);
	int i;
	for (i = 0; i < len; i++) 
	{
  USART_SendData(USART2, (uint16_t) str[i]);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
	printf("我用啦\r\n");
}


void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			//printf("\r\n有数据进来了\r\n");
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
			{
				//printf("\r\n第二捏\r\n");
			if(USART2_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)
				{
					USART2_RX_STA=0;
					
					//printf("\r\n错误！！\r\n");
				}//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{
				//printf("\r\n这里捏\r\n");					
				if(Res==0x0d){
					USART2_RX_STA|=0x4000;
				}
				else
					{
						//printf("\r\n最后捏\r\n");
					usart2_buffer[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					//printf("\r\n最后捏\r\n");
					if(USART2_RX_STA>(USART_REC_LEN-1)){
						//printf("出错啦");
						USART2_RX_STA=0;
					}//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     }
	//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}