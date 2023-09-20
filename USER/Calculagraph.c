#include "Calculagraph.h"
#include "led.h"
#include "sys.h"
#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "timer.h"
#include "stm32f10x_tim.h"
#include "usart.h"
#include "Usart55.h"
int time_sc_old = -1;
extern int total_times;
extern u16 Text_color;
extern u16 Back_color;
char str[20];
void getTime(void){
	if(time_sc_old!=total_times){
		//Lcd_Clear(Back_color);
		sprintf(str, "%d", total_times);
		Gui_DrawFont_GBK16(10,50,Text_color,Back_color,str);
		time_sc_old = total_times;
	}
}


void Calculagraph(void)
{
	u8 ts =0;
	short flag = 0;
	u8 tt = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//TIM3_Int_Init(10,36);
	Lcd_Clear(Back_color);
	USART3_Init();
	Gui_DrawFont_GBK16(10,10,Text_color,Back_color,"timeMaker");
	Gui_DrawFont_GBK16(10,90,Text_color,Back_color,"second");
	while(1){
		ts = 0;
		ts=KEY_Scan(0);
		if(ts == KEY1_PRES){
			Lcd_Clear(Back_color);
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//¿ª
			while(OK_s == 0){
				//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
				//delay_ms(1);
			}
			if(OK_s == 1){
				Gui_DrawFont_GBK16(10,10,Text_color,Back_color,"timeMaker");
				Gui_DrawFont_GBK16(10,90,Text_color,Back_color,"second");
				Gui_DrawFont_GBK16(10,50,Text_color,Back_color,received_string);
				memset(received_string, 0, MAX_STRLEN);
				get_P = 0;
				get_T = 0;
				indexs = 0;
				OK_s = 0;
				ts = 0;
			}
			//if(get_P != 0 && get_T != 0)
//			printf("%s\r\n",received_string);
//			printf("%d\r\n",strlen(received_string));
//			Gui_DrawFont_GBK16(10,50,Text_color,Back_color,received_string);
//			memset(received_string, 0, MAX_STRLEN);
//			get_P = 0;
//			get_T = 0;
//			indexs = 0;
//			OK_s = 0;
		}
//		if(indexs > 0)
//    {
//				//printf("%s",received_string);
//				Gui_DrawFont_GBK16(10,50,Text_color,Back_color,received_string);
//        memset(received_string, 0, MAX_STRLEN);
//        indexs=0;
//				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//			
//    }
//		ts = 0;
//		ts=KEY_Scan(0);
//		if(ts == KEY1_PRES){
//			TIM3_STOP(flag);
//			break;
//		}
//		if(ts == KEY0_PRES){
//			flag = flag + 1;
//			//total_times = 0;
//			//TIM3_Int_Init(999,35999);
//			TIM3_STOP(flag);
//		}
//		else{
//			getTime();
//		}
	}
}


