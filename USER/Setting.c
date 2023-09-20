#include "Setting.h"
#include "led.h"
#include "sys.h"
#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#include "Lcd_Driver.h"
#include "GUI.h"

u8* arrays_setting [5] = {"Change Color","Power","TIME","Exit"};
extern u16 Text_color;
extern u16 Back_color;
extern u16 Text_color_choose;
extern u16 Back_color_choose;
extern u8 colorIds;
void Initi_Show_Setting_Menu(void)
{
	short i = 0;
	u16 y = 5;
	Lcd_Clear(Back_color);
	Gui_DrawFont_GBK16(3,105,BLUE,BLACK,"Taihang Shield");	
	for(; i < 4;i++){
		if(i == 0){
			Creat_Menu_String(y,arrays_setting[i],'1');
		}
		else{
			Creat_Menu_String(y,arrays_setting[i],'0');
		}
		y = y + 18;
	}
}

void ChangeColor(void){
	if(colorIds + 1 == 2){
		colorIds = 0;
	}
	else{
		colorIds = colorIds + 1;
	}
	getColor();
	Initi_Show_Setting_Menu();
}

void Move_Show_Setting_Menu(short cs){
	short i = 0;
	u16 y = 5;
	Lcd_Clear(Back_color);
	Gui_DrawFont_GBK16(3,105,BLUE,BLACK,"Taihang Shield");	
	for(; i < 4;i++){
		if(i == cs){
			Creat_Menu_String(y,arrays_setting[i],'1');
		}
		else{
			Creat_Menu_String(y,arrays_setting[i],'0');
		}
		y = y + 18;
	}
}

u8 use_Setting(short cs){
	if(cs == 0){
		ChangeColor();
		}
	if(cs == 3){
		return 0;
	}
	return 1;
}

	
void Setting(void)
{
	u8 ts =0;
	short choose = 0;
	u8 flag;
	Initi_Show_Setting_Menu();
	while(1){
		ts = 0;
		ts=KEY_Scan(0);
		if(ts == KEY0_PRES){
			if(choose == 3){
					choose = 0;
				}
				else{
					choose = choose + 1;
				}
				Move_Show_Setting_Menu(choose);
		}
		if(ts == KEY1_PRES){
			if(use_Setting(choose) == 0){
				break;
			}
	}
}	
	}