#include "stm32f10x.h"
#include "delay.h"

void My_USART1_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;// GPIO
	USART_InitTypeDef USART_InitStructure;// ����
	NVIC_InitTypeDef NVIC_InitStructure;// �ж�
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);//ʹ�ܴ���ʱ�Ӻ�GPIOʱ��
	USART_DeInit(USART1);//��λ����
	
}