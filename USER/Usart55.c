#include "Usart55.h"
#include "stm32f10x.h"
#include "usart.h"

#define MAX_STRLEN 200 // 最大接收字符串长度

volatile char received_string[MAX_STRLEN+1]; // 用于存储接收到的字符串
uint32_t indexs = 0; // 用于记录接收字符串的索引位置
uint32_t get_T = 0;
uint32_t get_P = 0;
uint32_t OK_s = 0;
void USART3_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_INitstrute;      //串口使用中断
    // 使能USART3和GPIOB的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置GPIOB.10 (USART3_TX) 为复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 配置GPIOB.11 (USART3_RX) 为浮空输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 配置USART3的通信参数，如波特率、数据位数、奇偶校验位和停止位等
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStruct);

    // 启用USART3
    USART_Cmd(USART3, ENABLE);
    USART_ClearFlag(USART3,USART_FLAG_TC);
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//开启串口接受中断

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组
    NVIC_INitstrute.NVIC_IRQChannel= USART3_IRQn ;  //打开UART1的中断
    NVIC_INitstrute.NVIC_IRQChannelCmd=ENABLE;
    NVIC_INitstrute.NVIC_IRQChannelPreemptionPriority=0;  //抢占优先级
    NVIC_INitstrute.NVIC_IRQChannelSubPriority=2;          //响应优先级
    NVIC_Init(&NVIC_INitstrute);
}

void USART3_SenStr(USART_TypeDef* USARTx, u8 *str)
{
    //printf("发送了");
    int len = strlen(str);
    int i;
    for (i = 0; i < len; i++)
    {
        USART_SendData(USART3, (uint16_t) str[i]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
    printf("发送了");

}



void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // 判断是否是USART3接收中断
    {
        //static char buffer[100]; // 定义一个缓冲区用于存放接收到的数据
        //static int index = 0; // 定义一个变量用于记录缓冲区的下标

        char data = USART_ReceiveData(USART3); // 读取接收到的数据

        if (data == 'P') // 如果读取到T，则清空缓冲区
        {
            //printf("okok");
            indexs = 0;
            get_P = 1;
            get_T = 0;
            memset(received_string, 0, MAX_STRLEN);
        } else         if (data == 'T') // 如果读取到P，则对缓冲区的数据进行验证，并关闭接收中断
        {
            //printf("afafaf");
            get_T = 1;

        }
        else // 否则将数据存入缓冲区
        {
            if (get_P == 1 && get_T == 1 && data == '\n') // 验证成功
            {
							if(strlen(received_string) == 7){
								OK_s = 1;
                USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
							}
							else{
								OK_s = 0;
								get_P = 0;
								get_T = 0;
								memset(received_string, 0, MAX_STRLEN);
								indexs = 0;
								//printf("出错啦！");
							}

            } else {
                received_string[indexs] = data;
                indexs++;

            }
        } // 清空缓冲区
    }
}