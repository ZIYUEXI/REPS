#ifndef __USART55_H
#define __USART55_H
#include"sys.h"

#define MAX_STRLEN 200
extern uint32_t indexs;
extern uint32_t get_T;
extern uint32_t get_P;
extern uint32_t OK_s;
extern volatile char received_string[MAX_STRLEN+1];
void USART3_Init(void);
void USART3_SenStr(USART_TypeDef* USARTx, u8 *str);

#endif