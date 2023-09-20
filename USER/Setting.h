#ifndef __SETTING_H
#define __SETTING_H
#include "stm32f10x.h"

void Setting(void);
void Initi_Show_Setting_Menu(void);
void ChangeColor(void);
u8 use_Setting(short cs);
void Creat_Menu_String(u16 y,u8* target,char f);
#endif
