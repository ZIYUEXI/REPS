#include "lora.h"
#include "sys.h"
#include "delay.h"
#include "usart3.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "Lcd_Driver.h"
#include "stm32f10x_exti.h"
 
//�豸������ʼ��(�����豸������lora_cfg.h����)

typedef struct
{
   u16 addr;//�豸��ַ
   u8 chn;//�ŵ�
   u8 power;//���书��
   u8 wlrate;//��������
   u8 wltime;//����ʱ��
   u8 mode;//����ģʽ
   u8 mode_sta;//����״̬
   u8 bps;//���ڲ�����
   u8 parity;//У��λ
}_LoRa_CFG;


_LoRa_CFG LoRa_CFG=
{
	LORA_ADDR,//�豸��ַ
	LORA_POWER,//�ŵ�
	LORA_CHN,//���书��
	LORA_RATE,//��������
	LORA_WLTIME,//����ʱ��
	LORA_MODE,//����ģʽ
	LORA_STA,//����״̬
	LORA_TTLBPS,//���ڲ�����
	LORA_TTLPAR//У��λ
};
 
 
 
//ȫ�ֲ���
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
 
//�豸����ģʽ(���ڼ�¼�豸״̬)
u8 Lora_mode=0;//0:����ģʽ 1:����ģʽ 2:����ģʽ
//��¼�ж�״̬
static u8 Int_mode=0;//0:�ر� 1:������ 2:�½���
 
 
//usmart֧�ֲ���
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART3_RX_STA;
//     1,����USART3_RX_STA;
void lora_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		//printf("%s",USART3_RX_BUF);	//���͵�����
		if(mode)USART3_RX_STA=0;
	} 
}
//lora���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)
u8* lora_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//lora��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART3->SR&0X40)==0);//�ȴ���һ�����ݷ������  
		USART3->DR=(u32)cmd;
	}else u3_printf("%s\r\n",cmd);//��������
	
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
	   while(--waittime)	//�ȴ�����ʱ
	   { 
		  delay_ms(10);
		  if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		  {
			  if(lora_check_cmd(ack))break;//�õ���Ч���� 
			  USART3_RX_STA=0;
		  } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
}
 
 
//AUX�ж�����
//mode:���õ�ģʽ 0:�ر� 1:������ 2:�½���
void Aux_Int(u8 mode)
{
    if(!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//�ر��ж�
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
	}else
	{
		if(mode==1)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //������
	    else if(mode==2)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½���
		
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	}
	Int_mode = mode;//��¼�ж�ģʽ
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  
    
}
 
//LoRaģ���ʼ��
//����ֵ: 0,���ɹ�
//        1,���ʧ��
u8 LoRa_Configure(void)
{
	 u8 retry=0;
	 u8 temp=1;
	
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��PA�˿�ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
 
     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//��ֹJTAG,�Ӷ�PA15��������ͨIOʹ��,����PA15��������ͨIO!!!	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LORA_MD0 //PA13
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	    		 //LORA_MD1 //PA14
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LORA_AUX
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //��������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.4
	
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	 EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
  	 EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //�ж��߹ر�(�ȹرպ����ٴ�)
  	 EXTI_Init(&EXTI_InitStructure);//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//LORA_AUX
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2�� 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//�����ȼ�3
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //�ر��ⲿ�ж�ͨ���������ٴ򿪣�
   	 NVIC_Init(&NVIC_InitStructure); 
	 
	 LORA_MD0=0;
	 LORA_MD1=0;
	 LORA_AUX=0;
	
	 while(LORA_AUX)//ȷ��LORAģ���ڿ���״̬��(LORA_AUX=0)
	 {
		 delay_ms(600); 
	 }
	 usart3_init(115200);//��ʼ������3
	 
	 //LORA_MD0=1;//����ATģʽ
	 return 1;
}
void LoRa_Init(void)
{
	printf("hihihihittttww");
	while(LoRa_Configure())//��ʼ��ATK-LORA-01ģ��
	{	
		delay_ms(300);
		printf("hihihihi");
	}
	//LoRa_Set();
	LORA_MD0=1;
	printf("hihihihisww");
	//usart3_set(LoRa_CFG.bps,LoRa_CFG.parity);
	usart3_rx(1);
}
 
//Loraģ���������
void LoRa_Set(void)
{
	u8 sendbuf[20];
	u8 lora_addrh,lora_addrl=0;
	
	//usart3_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//��������ģʽǰ����ͨ�Ų����ʺ�У��λ(115200 8λ���� 1λֹͣ ������У�飩
	usart3_rx(1);//��������3����
	
	while(LORA_AUX);//�ȴ�ģ�����
	//LORA_MD0=1; //��������ģʽ
	delay_ms(40);
	Lora_mode=0;//���"����ģʽ"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//�����豸��ַ
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//�����ŵ��Ϳ�������
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//���÷��书��
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//���ù���ģʽ
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//���÷���״̬
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//����˯��ʱ��
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LoRa_CFG.bps,LoRa_CFG.parity);//���ô��ڲ����ʡ�����У��λ
	lora_send_cmd(sendbuf,"OK",50);
 
	LORA_MD0=1;//�˳�����,����ͨ��
	delay_ms(40);
	while(LORA_AUX);//�ж��Ƿ����(ģ����������ò���)
	USART3_RX_STA=0;
	Lora_mode=1;//���"����ģʽ"
	//usart3_set(LoRa_CFG.bps,LoRa_CFG.parity);//����ͨ��,����ͨ�Ŵ�������(�����ʡ�����У��λ)
	Aux_Int(1);//����LORA_AUX�������ж�	
	
}
 
u8 Dire_Date[]={0x11,0x22,0x33,0x44,0x55};//����������
u8 date[30]={0};//��������
 
 
 
u32 obj_addr = 0;//��¼�û�����Ŀ���ַ
u8 obj_chn = 0;//��¼�û�����Ŀ���ŵ�
 
u8 wlcd_buff[10]={0}; //LCD��ʾ�ַ���������
//Loraģ�鷢������
void LoRa_SendData(u8 Data[])
{ 
	static u8 num=0;
    u16 addr;
	u8 chn;
	u16 i=0; 
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//͸������
	{
		u3_printf("%s\r\n",Data);
		printf("LoRa_SendData");
		num++;
		if(num==255) num=0;
		
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//������
	{
		
		addr = (u16)obj_addr;//Ŀ���ַ
		chn = obj_chn;//Ŀ���ŵ�
		
		date[i++] =(addr>>8)&0xff;//��λ��ַ
		date[i++] = addr&0xff;//��λ��ַ
		date[i] = chn;//�����ŵ�
		
		for(i=0;i<Dire_DateLen;i++)//����д������BUFF
		{
			date[3+i] = Dire_Date[i];
		}	
		for(i=0;i<(Dire_DateLen+3);i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������   
			USART_SendData(USART3,date[i]); 
		}	
		
        //��ʮ�����Ƶ�����ת��Ϊ�ַ�����ӡ��lcd_buff����
		sprintf((char*)wlcd_buff,"%x %x %x %x %x %x %x %x",
				date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7]);
	    Dire_Date[4]++;//Dire_Date[4]���ݸ���		
	}
			
}
 
u8 rlcd_buff[10]={0}; //LCD��ʾ�ַ���������
//Loraģ���������
u8* LoRa_ReceData(void)
{
 
    u16 len=0;
		printf("save221");
	//����������
	if(USART3_RX_STA&0x8000)
	{
		//printf("save");
		LED1 =0;
		memset((char*)USART3_RX_BUF,0x00,len);//���ڽ��ջ�������0
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//��ӽ�����
		USART3_RX_STA=0;
 
		if(LoRa_CFG.mode_sta==LORA_STA_Tran)//͸������
		{	
 
		}else if(LoRa_CFG.mode_sta==LORA_STA_Dire)//������
		{
			//��ʮ�����Ƶ�����ת��Ϊ�ַ�����ӡ��lcd_buff����
			sprintf((char*)rlcd_buff,"%x %x %x %x %x",
			USART3_RX_BUF[0],USART3_RX_BUF[1],USART3_RX_BUF[2],USART3_RX_BUF[3],USART3_RX_BUF[4]);
		}
		
	}
	return USART3_RX_BUF;
 
}
 
 
 
//LORA_AUX�жϷ�����
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
	{  
	   if(Int_mode==1)//������(����:��ʼ�������� ����:���ݿ�ʼ���)     
	   {
		  if(Lora_mode==1)//����ģʽ
		  {
			 USART3_RX_STA=0;//���ݼ�����0
		  }
		  Int_mode=2;//�����½��ش���
		  LED0=0;//DS0��
	   }
       else if(Int_mode==2)//�½���(����:�����ѷ����� ����:�����������)	
	   {
		  if(Lora_mode==1)//����ģʽ
		  {
			 USART3_RX_STA|=1<<15;//���ݼ���������
		  }else if(Lora_mode==2)//����ģʽ(�������ݷ������)
		  {
			 Lora_mode=1;//�������ģʽ
		  }
		  Int_mode=1;//���������ش���
          LED0=1;//DS0��		   
	   }
       Aux_Int(Int_mode);//���������жϱ���
	   EXTI_ClearITPendingBit(EXTI_Line4); //���LINE4�ϵ��жϱ�־λ  
	}	
}
 
 
 
 
 