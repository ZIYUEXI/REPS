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
    
    // ����PC7Ϊ����͵�ƽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);

    // ����PC8Ϊ����ߵ�ƽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_8);
}


void USART2_Init(uint32_t bunld){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_INitstrute;      //����ʹ���ж�
  // ʹ��USART2��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  // ����USART2����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
		  //USART1_RX	  GPIOA.3��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//
  GPIO_Init(GPIOA, &GPIO_InitStructure);//

  // ����USART2����
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);
	

  // ʹ��USART2
  USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);  //�����־λ
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ���
	NVIC_INitstrute.NVIC_IRQChannel= USART2_IRQn ;  //��UART1���ж�
	NVIC_INitstrute.NVIC_IRQChannelCmd=ENABLE;
	NVIC_INitstrute.NVIC_IRQChannelPreemptionPriority=1;  //��ռ���ȼ�
	NVIC_INitstrute.NVIC_IRQChannelSubPriority=1;          //��Ӧ���ȼ�
	NVIC_Init(&NVIC_INitstrute); 
	Init_MD0_MD1();
}

void USART_Clear(USART_TypeDef* USARTx)
{
    uint16_t dummy = USARTx->DR;
    (void)dummy; // �����������ʾδʹ�õı���
    USART_ClearFlag(USARTx, USART_FLAG_RXNE);
}

// ���ͺ���
void USART_SenStr(USART_TypeDef* USARTx, u8 *str)
{
	int len = strlen(str);
	int i;
	for (i = 0; i < len; i++) 
	{
  USART_SendData(USART2, (uint16_t) str[i]);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
	printf("������\r\n");
}


void USART2_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			//printf("\r\n�����ݽ�����\r\n");
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		
		if((USART2_RX_STA&0x8000)==0)//����δ���
			{
				//printf("\r\n�ڶ���\r\n");
			if(USART2_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)
				{
					USART2_RX_STA=0;
					
					//printf("\r\n���󣡣�\r\n");
				}//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{
				//printf("\r\n������\r\n");					
				if(Res==0x0d){
					USART2_RX_STA|=0x4000;
				}
				else
					{
						//printf("\r\n�����\r\n");
					usart2_buffer[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					//printf("\r\n�����\r\n");
					if(USART2_RX_STA>(USART_REC_LEN-1)){
						//printf("������");
						USART2_RX_STA=0;
					}//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     }
	//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}