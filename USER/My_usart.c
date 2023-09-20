#include "stm32f10x.h"
#include "delay.h"

void My_USART1_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;// GPIO
	USART_InitTypeDef USART_InitStructure;// 串口
	NVIC_InitTypeDef NVIC_InitStructure;// 中断
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);//使能串口时钟和GPIO时钟
	USART_DeInit(USART1);//复位串口
	
}