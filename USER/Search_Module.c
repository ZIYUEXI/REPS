#include "Search_Module.h"
#include "led.h"
#include "sys.h"
#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "math.h"
#include "usart.h"
#define PI_M 3.1415926
static u8 find_Flag = 1;
static u16 SCREEN_WIDTH = 128;
static u16 SCREEN_HEIGHT = 128;
static u16 remotion = 0;
static u16 x_cent;
static u16 y_cent;
static u16 r = 55;

struct coordinate{
	short x;
	short y;
};

struct real_Point{
	u16 x;
	u16 y;
};

void Change_u2s(u16 x,u16 y){
	
}

//x=33,y=44
struct real_Point Change_s2u(struct coordinate points){
	struct real_Point test;
	double x;
	double y;
	u16 ccs = points.x*points.x + points.y*points.y;
	if(ccs > r){
		double theta = atan(points.y/points.x);
		x = r * cos(theta);
		x = floor(x);
		y = r * sin(theta);
		y = floor(y);
		points.x = (short)x;
		printf("%d\n",points.x);
		points.y = (short)y;
		printf("%d\n",points.y);
}
	if(points.x < 0 && points.y < 0){
		test.x = points.x - x_cent;
		test.y = y_cent + points.y;
	}
	
		if(points.x > 0 && points.y <= 0){
			test.x = points.x + x_cent;
			test.y = y_cent + points.y;
	}
		
		if(points.x <= 0 && points.y > 0){
			test.x = x_cent - points.x;
			test.y = y_cent - points.y;		//第一
	}
		
		if(points.x > 0 && points.y > 0){
			test.x = x_cent + points.x;
			test.y = y_cent - points.y;		//第一
	}
	return test;
	
}

void infer(void){
	//Gui_DrawFont_GBK16(10,50,WHITE,BLACK,"No Sigal 11111 ");
	Gui_DrawFont_ASC12(10,50,WHITE,BLACK,"No Sigal 11111");
}

void draw_point(struct real_Point a){
	Gui_box(a.x,a.y,3,3,WHITE);
}
	
void Init_Paint(void){
	struct coordinate t1;
	struct real_Point t2;
	x_cent = (SCREEN_WIDTH / 2);
  y_cent = (SCREEN_HEIGHT / 2) - 7;
	Gui_Circle(x_cent, y_cent, r,GREEN);
	//Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)
	Gui_DrawLine(x_cent,y_cent,x_cent+r,y_cent,GREEN);
	Gui_DrawLine(x_cent,y_cent,x_cent-r,y_cent,GREEN);
	Gui_DrawLine(x_cent,y_cent,x_cent,y_cent+r,GREEN);
	Gui_DrawLine(x_cent,y_cent,x_cent,y_cent-r,GREEN);
	
	Gui_DrawFont_ASC12(10,120,GREEN,BLACK,"No Sigal 11111");
	
	//void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
	t1.x = 100;
	t1.y = 200;
	
	t2 = Change_s2u(t1);
	draw_point(t2);
}
	
void Search_Module(void)
{
	u8 ts =0;
	Lcd_Clear(BLACK);
	while(1){
		ts = 0;
		ts=KEY_Scan(0);
		if(find_Flag == 0){
			infer();
		}
		else{
			Init_Paint();
		}
		if(ts == KEY1_PRES){
			break;
		}
	}
}