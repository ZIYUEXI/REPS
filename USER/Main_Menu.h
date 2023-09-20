#ifndef __MAIN_MENU_H
#define __MAIN_MENU_H

#include "stm32f10x.h"
void Main_Menu(void);
void getColor(void);
void Creat_Menu_String(u16 y,u8* target,char f);
void Initi_Show_Main_Menu(void);
void Move_Show_Main_Menu(short cs);
void enter_Moudle(short cs);
#endif
