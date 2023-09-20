#include "led.h"
#include "sys.h"
#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "Main_Menu.h"
#include "timer.h"
#include "usart.h"
#include "stm32f10x_gpio.h"
#include "usart3.h"
#include "SysT.h"

//static char friend_code[] = "ACW1FAW21";
//char line[20];

 int main(void)
 {
	 u8 ts = 0;
	 u8 can = 0;
	SystemInit();
	delay_init();	    	 //延时函数初始化	  
	LED_Init();
	Lcd_Init();	 //初始化与LED连接的硬件接口
	KEY_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//delay_ms(2000);
	uart_init(9600);
	//USART2_Init(9600);
	//strcpy(line, friend_code);
	//strcat(line, "\r\n");
	//InitSysT();
	while(1){
		ts=KEY_Scan(0);
		if(ts == KEY0_PRES){
			break;
		}	
		//if(USART2_RX_STA&0x8000)
//		if(can == 25){
//			USART_SenStr(USART2,line);
//			//printf("已发送");
//			can = 0;
//		}
//		if(USART2_RX_STA> 0)
//		{
//			//printf("获取到数据了");
//			//printf("%s",usart2_buffer);
//			if(strcmp(usart2_buffer,friend_code) == 0) {
//				printf("匹配正确");
//			}
//			memset(usart2_buffer, 0, USART_REC_LEN);
//			USART2_RX_STA=0;
//		}
		//printf("%s",friend_code);
//		can = can + 1;
		delay_ms(50);
 }
	Main_Menu();
 }

