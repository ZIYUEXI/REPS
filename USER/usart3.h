#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  	   
#include "usart.h"

extern u16 USART2_RX_STA;
extern u8 usart2_buffer[USART_REC_LEN];

void USART_SendString(USART_TypeDef* USARTx, char* str);
void USART2_Init(uint32_t bunld);
void USART_SenStr(USART_TypeDef* USARTx, u8 *str);

#endif