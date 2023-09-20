#include "Main_Menu.h"
#include "led.h"
#include "sys.h"
#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "Search_Module.h"
#include "Calculagraph.h"
#include "Setting.h"
#include "Friendly_Forces.h"
u8* arrays [5] = {"Search Module","Friendly Forces","Calculagraph","Setting","ReBoot"};

u16 Text_color;
u16 Back_color;
u16 Text_color_choose;
u16 Back_color_choose;
u8 colorIds;
typedef struct {
    void (*task_func)(void);    // 任务函数指针
    uint32_t delay_ticks;       // 延迟tick数
    uint32_t delay_count;       // 已经延迟的tick数
    uint32_t is_valid;          // 是否有效标志
} tcb_t;

void getColor(void){
	if(colorIds == 0){
		Text_color = WHITE;
		Back_color = BLACK;
		Text_color_choose = RED;
		Back_color_choose = WHITE;
	}
		if(colorIds == 1){
		Text_color = BLACK;
		Back_color = WHITE;
		Text_color_choose = WHITE;
		Back_color_choose = RED;
	}
}
void Creat_Menu_String(u16 y,u8* target,char f){
	getColor();
	if(f == '0'){
		Gui_DrawFont_GBK16(3,y,Text_color,Back_color,target);
}
	else{
		Gui_DrawFont_GBK16(3,y,Text_color_choose,Back_color_choose,target);
	}
}

void Initi_Show_Main_Menu(void)
{
	short i = 0;
	u16 y = 5;
	Gui_DrawFont_GBK16(3,105,BLUE,Back_color,"Taihang Shield");	
	for(; i < 5;i++){
		if(i == 0){
			Creat_Menu_String(y,arrays[i],'1');
		}
		else{
			Creat_Menu_String(y,arrays[i],'0');
		}
		y = y + 18;
	}
}

void Move_Show_Main_Menu(short cs){
	short i = 0;
	u16 y = 5;
	Lcd_Clear(Back_color);
	Gui_DrawFont_GBK16(3,105,BLUE,Back_color,"Taihang Shield");	
	for(; i < 5;i++){
		if(i == cs){
			Creat_Menu_String(y,arrays[i],'1');
		}
		else{
			Creat_Menu_String(y,arrays[i],'0');
		}
		y = y + 18;
	}
}

void enter_Moudle(short cs){
	if(cs == 2){
		Calculagraph();
	}
	if(cs == 3){
		Setting();
	}
	if(cs == 0){
		Search_Module();
	}
	if(cs == 1){
		Friendly_Forces();
	}
	Lcd_Clear(Back_color);
	Move_Show_Main_Menu(cs);
}

void Main_Menu(void)
{
	u8 ts =0;
	short choose = 0;
	colorIds = 0;
	getColor();
	LCD_LED_SET;//通过IO控制背光亮
	Lcd_Clear(Back_color);
	Initi_Show_Main_Menu();
	while(1){
		ts = 0;
		ts=KEY_Scan(0);
		if(ts == KEY0_PRES){
			if(choose == 4){
					choose = 0;
				}
				else{
					choose = choose + 1;
				}
				Move_Show_Main_Menu(choose);
		}
		if(ts == KEY1_PRES){
			enter_Moudle(choose);
		}
	}
	LCD_LED_CLR;
}
