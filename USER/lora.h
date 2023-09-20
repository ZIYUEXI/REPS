#ifndef __LORA_H
#define __LORA_H
#include "sys.h"
 
//�豸��������
 
#define LORA_AUX  PAin(4)    //LORAģ��״̬����
#define LORA_MD0  PAout(13)  //LORAģ���������
#define LORA_MD1  PAout(14)
 
//��������(��λ:Kbps)
#define  LORA_RATE_0K3  0 //0.3
#define  LORA_RATE_1K2  1 //1.2
#define  LORA_RATE_2K4  2 //2.4
#define  LORA_RATE_4K8  3 //4.8
#define  LORA_RATE_9K6  4 //9.6
#define  LORA_RATE_19K2 5 //19.2
 
//����ʱ��(��λ:��)
#define LORA_WLTIME_1S  0  //1��
#define LORA_WLTIME_2S  1  //2��
 
//����ģʽ
#define LORA_MODE_GEN   0   //һ��ģʽ
#define LORA_MODE_WK    1   //����ģʽ
#define LORA_MODE_SLEEP 2   //ʡ��ģʽ
 
//���书��  
#define LORA_PW_11dBm  0   //11dBm
#define LORA_PW_14Bbm  1   //14dBm
#define LORA_PW_17Bbm  2   //17dBm
#define LORA_PW_20Bbm  3   //20dBm
 
//����״̬
#define LORA_STA_Tran 0 //͸������
#define LORA_STA_Dire 1 //������
 
//���ڲ�����(��λ:bps)
#define LORA_TTLBPS_1200    0  //1200
#define LORA_TTLBPS_2400    1  //2400
#define LORA_TTLBPS_4800    2  //4800
#define LORA_TTLBPS_9600    3  //9600
#define LORA_TTLBPS_19200   4  //19200
#define LORA_TTLBPS_38400   5  //38400
#define LORA_TTLBPS_57600   6  //57600
#define LORA_TTLBPS_115200  7  //115200
 
//��������У��
#define LORA_TTLPAR_8N1  0 //8λ����
#define LORA_TTLPAR_8E1  1 //8λ����+1λżУ��
#define LORA_TTLPAR_8O1  2 //8λ����+1λ��У��
 
 
//�豸Ĭ�ϲ���
#define LORA_ADDR    1919                //�豸��ַ
#define LORA_CHN     17               //ͨ���ŵ�
#define LORA_POWER   LORA_PW_20Bbm    //���书��
#define LORA_RATE    LORA_RATE_2K4   //��������
#define LORA_WLTIME  LORA_WLTIME_1S   //����ʱ��
#define LORA_MODE    LORA_MODE_GEN    //����ģʽ
#define LORA_STA     LORA_STA_Tran    //����״̬
#define LORA_TTLBPS  LORA_TTLBPS_115200 //������
#define LORA_TTLPAR  LORA_TTLPAR_8N1  //У��λ 
 
#define LORA_AUX  PAin(4)    //LORAģ��״̬����
#define LORA_MD0  PAout(13)  //LORAģ���������
#define LORA_MD1  PAout(14)  //LORAģ���������
 
#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
 
extern u8 Lora_mode;

void lora_at_response(u8 mode);	
u8* lora_check_cmd(u8 *str);
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
 
 
u8 LoRa_Configure(void);
void LoRa_Init(void);
void Aux_Int(u8 mode);
void LoRa_Set(void);
void LoRa_SendData(u8 Data[]);
u8* LoRa_ReceData(void);
 
 
#endif
 