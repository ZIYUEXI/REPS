#include "Friendly_Forces.h"
#include "sys.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "usart3.h"
#include "usart.h"
#include "stm32f10x.h"
#include "key.h"
#include "timer.h"
#include "Usart55.h"
#include <stdlib.h>
#include <string.h>


#define accws 3


static char friend_code[] = "AC14234234GG";
static char check_code[] = "faf2323FFQA";
static char friend_code_with_enter[8];
static char check_code_with_enter[8];
static int timessw[accws];
static u8 flag = 0;
static u8* arrays_setting [3] = {"GET","PUSH","Exit"};
static u8 Shou_Fa = 0;
static u8 far_ye = 0;
extern int total_times;
struct IntPair {
    int first;
    int second;
};

struct Times_ver{
	  int first;
    int second;
};

int GetTime(void){
			int result;
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开
			while(OK_s == 0){
				printf("循环\r\n");
			}
			if(OK_s == 1){
				result = atoi((const char*)received_string);
				memset(received_string, 0, MAX_STRLEN);
				get_P = 0;
				get_T = 0;
				indexs = 0;
				OK_s = 0;
			}
			
			printf("hello!");
			
			return result;
	
}
void Initi_Show_Friendly_Forces_Menu(void) {
    Lcd_Clear(BLACK);
    Gui_DrawFont_GBK16(3,10,BLUE,BLACK,"Friend 1");
}

void Initi_ChooseShoufa(void) {
    short i = 0;
    u16 y = 5;
    Lcd_Clear(BLACK);
    Gui_DrawFont_GBK16(3,105,BLUE,BLACK,"Taihang Shield");
    for(; i < 3; i++) {
        if(i == 0) {
            Creat_Menu_String(y,arrays_setting[i],'1');
        }
        else {
            Creat_Menu_String(y,arrays_setting[i],'0');
        }
        y = y + 18;
    }
}

//void Push_time(char* tims) {
//    static char lenthsw[20];
//		printf("%s",tims);
//		printf("\r\n");
//    strcpy(lenthsw, "");
//    strncat(lenthsw, "T", 1);
//    strcat(lenthsw, tims);
//    strcat(lenthsw, "P\r\n");
//		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    USART_SenStr(USART2,lenthsw);
//		printf("%s",lenthsw);
//		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//}

void showTime(char* tims) {
    Lcd_Clear(BLACK);
    Gui_DrawFont_GBK16(3,10,BLUE,BLACK,"Friend1");

    Gui_DrawFont_GBK16(3,70,BLUE,BLACK,tims);
	
		if(far_ye == 1){
			Gui_DrawFont_GBK16(80,80,RED,BLACK,"F");
		}
				if(far_ye == 2){
			Gui_DrawFont_GBK16(80,80,RED,BLACK,"C");
		}
						if(far_ye == 0){
			Gui_DrawFont_GBK16(80,80,RED,BLACK,"N");
		}
}

void Move_Choose_shouFa(short cs) {
    short i = 0;
    u16 y = 5;
    Lcd_Clear(BLACK);
    Gui_DrawFont_GBK16(3,105,BLUE,BLACK,"Taihang Shield");
    for(; i < 3; i++) {
        if(i == cs) {
            Creat_Menu_String(y,arrays_setting[i],'1');
        }
        else {
            Creat_Menu_String(y,arrays_setting[i],'0');
        }
        y = y + 18;
    }
}

void use_shouFa(short cs) {
    if(cs == 0) {
        Shou_Fa = 0;
    }
    if(cs == 1) {
        Shou_Fa = 1;
    }
}
void check_far(struct Times_ver test){
	if(test.first < test.second){
		far_ye = 1;
	}
	if(test.first > test.second){
		far_ye = 2;
	}
	else{
		far_ye = 0;
	}
}
void get_Number(size_t lens,char* a) {
    char buffer[20];
    memcpy(buffer, a + 1, lens);
    buffer[lens] = '\0';
		showTime(buffer);
    //printf("%s\r\n", buffer);
}

int get_Ver(int targetArry[10]){
	int sum = 0;  // 定义变量sum并初始化为0
	int average;  // 定义变量average，用于存放平均值
	int i;
	for (i = 0; i < accws; i++) {
    sum += targetArry[i];  // 累加数组中的每个元素
	}

	average = sum / accws;
	return average;// 计算平均值，需要将sum转换为浮点型
}
	
	
u8 find_sigal(void) {
    //printf("查验");
    char* p_start;
    char* p_end;
    size_t len;
    if(USART2_RX_STA&0X8000)
    {
        //printf("有东西");
        if(strcmp(usart2_buffer,friend_code) == 0) {
            memset(usart2_buffer, 0, USART_REC_LEN);
            USART2_RX_STA=0;
            return 1;
        }
        //printf("匹配到了");
        if(strcmp(usart2_buffer,check_code) == 0) {
            memset(usart2_buffer, 0, USART_REC_LEN);
            USART2_RX_STA=0;
            return 2;
        }
        printf("%s\r\n",usart2_buffer);
				
				if(strcmp(usart2_buffer,"TTACW") == 0) {
            memset(usart2_buffer, 0, USART_REC_LEN);
            USART2_RX_STA=0;
            return 3;
        }
//        p_start = (char*)strchr((char*)usart2_buffer, 'T');
//        p_end = (char*)strrchr((char*)usart2_buffer, 'P');
//        //printf("%s",usart2_buffer);
//        if(p_start != NULL && p_end != NULL) {
//            // 计算子字符串的长度
//            len = p_end - p_start - 1;
//            //printf("匹配到了");
//            get_Number(len,p_start);
//            // 定义一个缓冲区，用于存储子字符串
//            //char buffer[len + 1];
//            // 将子字符串拷贝到缓冲区
//            //memcpy(buffer, p_start + 1, len);
//            // 在缓冲区末尾添加'\0'，形成一个以'\0'结尾的字符串
//            //buffer[len] = '\0';
//            // 输出子字符串
//            //printf("%s\n", buffer);
//        }

        memset(usart2_buffer, 0, USART_REC_LEN);
        USART2_RX_STA=0;
        return 0;

    }

    return 0;
}


void Friendly_Forces(void) {
    struct IntPair pair;
		struct Times_ver ver_group;
		short varNamber = 0;
    short can = 0;
    short baka = 0;
    short count = 0;
    u8 ts =0;
		u8 ta = 0;
    short choose = 0;
    int timesw;
    char str[20];
    strcpy(friend_code_with_enter, friend_code);
    strcat(friend_code_with_enter, "\r\n");
    strcpy(check_code_with_enter, check_code);
    strcat(check_code_with_enter, "\r\n");
    Initi_ChooseShoufa();
		USART3_Init();

    //选择手法模式
    while(1) {
        ts = 0;
        ts=KEY_Scan(0);
        if(ts == KEY0_PRES) {
            if(choose == 2) {
                choose = 0;
            }
            else {
                choose = choose + 1;
            }
            Move_Choose_shouFa(choose);
        }
        if(ts == KEY1_PRES) {
            use_shouFa(choose);
            break;
        }
    }


    Lcd_Clear(BLACK);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    USART2_Init(9600);
    Gui_DrawFont_GBK16(50,50,RED,BLACK,"NO SIGNAL");
    while(1) {
        if(flag == 0) {
            if(Shou_Fa == 1) {
                printf("发送模式");
                while(1) {

                    if(can == 1000) {
                        USART_SenStr(USART2,friend_code_with_enter);
                        //printf("发送了");
                        can = 0;
                    }
                    if(can == 100) {
                        if(find_sigal() == 2) {
                            flag = 2;
                            break;
                        }
                    }
                    can = can + 1;
                    delay_ms(1);
                }
            }

            if(Shou_Fa == 0) {
                printf("接收模式");
                while(1) {
                    if(find_sigal() == 1) {
                        flag = 1;
                        printf("find!!");
                        break;
                    }
                    delay_ms(100);
                }
            }
        }
        //printf("OKS111");
        Lcd_Clear(BLACK);
        Gui_DrawFont_GBK16(3,10,BLUE,BLACK,"Friend1");

        if(flag == 1) {
						//TIM3_Int_Init(1,36);
            //printf("OKS");
            while(1) {

                if(baka == 1000) {
										USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
										USART_ClearITPendingBit(USART2, USART_IT_RXNE);
                    USART_SenStr(USART2,check_code_with_enter);
                    printf("发送了");
                    baka = 0;
										USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
                }
                if(baka == 100) {
                    if(find_sigal() == 3) {
											count = count + 1;
                     if(count == 1) {
                            printf("搜寻信号2");
                            pair.first = GetTime();
                        }
                        if(count == 2) {
                            printf("搜寻信号3");
                            pair.second = GetTime();

                            timesw  = (pair.second - pair.first);
														
                            sprintf(str, "%d", timesw);
														timessw[varNamber] = timesw;
														varNamber = varNamber + 1;
														if(varNamber + 1 == accws){
															varNamber = 0;
															if(ta == 0){
																ver_group.first = get_Ver(timessw);
																ta = 1;
															}
															if(ta == 1){
																ver_group.second = get_Ver(timessw);
																check_far(ver_group);
																ta = 0;
															}
															
														}
                            showTime(str);
                            //Push_time(str);
                            count = 0;
														
                        }
												

                    }
                }

                baka = baka + 1;
                delay_ms(1);
            }
        }
        if(flag == 2) {
            printf("开始循环");
            //TIM3_Int_Init(1,36);
            while(1) {
                printf("开始循环");
                if(baka == 100) {
                    printf("搜寻信号");
                    if(find_sigal() == 2) {
                        printf("搜寻信号1");
                        count = count + 1;
                        if(count == 1) {
                            printf("搜寻信号2");
                            pair.first = GetTime();
                        }
                        if(count == 2) {
                            printf("搜寻信号3");
                            pair.second = GetTime();

                            timesw  = (pair.second - pair.first);
														
                            sprintf(str, "%d", timesw);
														timessw[varNamber] = timesw;
														varNamber = varNamber + 1;
														if(varNamber + 1 == accws){
															varNamber = 0;
															if(ta == 0){
																ver_group.first = get_Ver(timessw);
																printf("%d",ver_group.first);
																ta = 1;
															}
															if(ta == 1){
																ver_group.second = get_Ver(timessw);
																check_far(ver_group);
																printf("%d",ver_group.second);
																ta = 0;
															}
															
															//ta = ta + 1;
															
														}
                            showTime(str);
                            //Push_time(str);
                            count = 0;
														
                        }
												
												USART_SenStr(USART2,"TTACW\r\n");

                    }
                    baka = 0;
                }

                baka = baka + 1;
                delay_ms(1);
            }


        }
    }
}