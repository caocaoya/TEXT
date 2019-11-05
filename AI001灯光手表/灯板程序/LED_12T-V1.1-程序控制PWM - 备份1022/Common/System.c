#include "ALL_Includes.h"
bit BIT_TMP;
u16  DelaymS=0;
u32  Timer_1s=0;
Timer_Struct gTimer= {0,0,0,0};
u16  System_Timer=0;
void Delay100us()		//@5.5296MHz
{
	unsigned char i, j;

	_nop_();
	i = 1;
	j = 134;
	do
	{
		while (--j);
	} while (--i);
}

/***************************************************************
* �������ܣ�ʱ���������
* ���������
* ���������
***************************************************************/
/***************************************************************
* �������ܣ�ʱ���������
* ���������
* ���������
***************************************************************/
void SysTick_Handler2(void)
{
    ++gTimer.Tick20Msec;
    if((gTimer.Tick20Msec%5)==0)
    {
        bTemp100Msec=TIMER_SET;
    }
    if((gTimer.Tick20Msec%25)==0)
    {
        bTemp500Msec=TIMER_SET;
    }
    if(gTimer.Tick20Msec==50)
    {   gTimer.Tick20Msec=0;
        bTemp1Sec=TIMER_SET;
    }
}
void SysTick_Handler(void)
{
    bTemp1Msec=TIMER_SET;
    ++gTimer.Tick1Msec;
    if((gTimer.Tick1Msec%2)==0)
    {
        bTemp2Msec=1;
    }
    if((gTimer.Tick1Msec%5)==0)
    {
        bTemp5Msec=TIMER_SET;
    }
    if((gTimer.Tick1Msec%10)==0)
    {
        bTemp10Msec=TIMER_SET;
    }
    if(gTimer.Tick1Msec==20)
    {
        gTimer.Tick1Msec=0;
        bTemp20Msec=TIMER_SET;
        SysTick_Handler2();
    }
    if(DelaymS>0)
    {
        DelaymS--;
    }

}

void SysTimer_Process(void)
{
   EA=0;
    gTimer.Timer_Status.byte=gTimer.SysTick_Status.byte;

    if(gTimer.SysTick_Status.byte)
    {
        gTimer.SysTick_Status.byte=0;

    }


//	gTimer.Timer_Status.byte=gTimer.SysTick_Status.byte;
//	gTimer.SysTick_Status.byte=0;

//    Clear_SysTick_Status
    EA=1;
}
/***************************************************************
* �������ܣ���ʱ��0��ʼ��
* ���������
* ���������
***************************************************************/
void Timer0Init(void)
{
    TMOD&=0xF0;
    TMOD|=0x01;//ģʽ1:16λ��ʱ��
 //   CKCON&=~0X08;//1/12ϵͳʱ��
	  set_T0M;//��ʱ�� 0 ��ʱ��Դѡ��Ϊϵͳʱ��
    TL0 =Timer0_Reload;
    TH0 =Timer0_Reload>>8;			//�ж�ʱ��Ϊ100us
    TR0=1;
    ET0= 1;
    EA = 1;
}
/***************************************************************
* �������ܣ���ʱ��1��ʼ��
* ���������
* ���������
***************************************************************/
void Timer1Init(void)
{
    TMOD&=0x0F;
    TMOD|=0x10;//ģʽ1:16λ��ʱ��
 //   CKCON&=~0X08;//1/12ϵͳʱ��
//    TL1 =Timer1_Reload(PWM_Cycle);
//    TH1 =Timer1_Reload(PWM_Cycle)>>8;			//�ж�ʱ��Ϊ100us
    TR1=1;
    ET1= 1;
    EA = 1;
}
/***************************************************************
* �������ܣ��ⲿ�жϷ�����
* ���������
* ���������
***************************************************************/
void EXT0_ISR (void) interrupt 0  //interrupt address is 0x000B
{
 clr_IE0;//����ⲿ�жϱ�־
 SL_SC7A21_CLICK_SERVICE();
}
/***************************************************************
* �������ܣ���ʱ��0�жϷ�����
* ���������
* ���������
***************************************************************/
void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    TL0 = Timer0_Reload;
    TH0 = Timer0_Reload>>8;
	
//	  TL0 = 0XF6;
//    TH0 = 0XFF;
    SysTick_Handler();
   System_Timer++;
   // LED_Sing=~LED_Sing;
	P16_NC=~P16_NC;
}
void EXT1_ISR (void) interrupt 2  //�ⲿ�жϷ�����
{
	clr_IE1;//����ⲿ�жϱ�־
//	Receive_Packet();
SL_SC7A21_WRIST_INT12_SERVICE_FUNCTION();

}
/***************************************************************
* �������ܣ���ʱ��1�жϷ�����
* ���������
* ���������
***************************************************************/

void Timer1_ISR (void) interrupt 3  //interrupt address is 0x001B
{
    u16 T1_Reload=0;

    TL1 =T1_Reload;
    TH1 =T1_Reload>>8;			//�ж�ʱ��Ϊ10ms

}
void Timer2_ISR (void) interrupt 5
{
    TF2=0;
}
u16 ADC_VAL;
u16 Light_Val;
void EXT_IO_ISR (void) interrupt 7
{
switch(PIF)
{
	case   0:
		break;
	case   2:
		     Lignt_VDD=1;
		     adc_init();
         Light_Val=ADC_Read(4)>>4;//��ȡ��ǿ��
	       Light_Val=0xff-Light_Val;
	       ADCCON1&=~0x01;//�ر�ADC��Դ
	       Lignt_VDD=0;
         LED_OPEN_Time=(Default_LED_OPEN_Time*Light_Val)>>8;
         LED_OPEN_T=(0xff-(127-LED_OPEN_Time));
         LED_CLOSE_T=(0xff-LED_OPEN_Time);
		     LED_Count=0;
	       LED_Cycle=0;
		     set_WKTR; //�������Ѷ�ʱ�� 
		break;

}
PIF=0;
}

void WKT_ISR (void) interrupt 17
{
  clr_WKTF;
	//LED_Sing=0;
  clr_EPI;//�رչܽ��ж�
	Scan_LED();
//	P16_NC=~P16_NC;
}
/************************************************************************************************************
*    Timer2 Capture interrupt subroutine
************************************************************************************************************/
//void Capture_ISR (void) interrupt 12
//{
//        clr_CAPF0;                          // clear capture0 interrupt flag
////        P1 = C0L;														// For capture mode CxL/CxH with data capture from I/O pin
////        P2 = C0H;
////        P12 = ~P12;													//toggle GPIO1 to show int
// if((CAPCON1&0x03)==0)
// {Rudder=MAKEWORD(C0H,C0L);
////	filter_Rudder = filter_Rudder - (filter_Rudder >> 1) + Rudder;
////    Rudder=(int16) (filter_Rudder >> 1);
////	 if(Rudder>filter_Rudder_MAX)
////	 {filter_Rudder_MAX=Rudder;}
////	 if(Rudder<filter_Rudder_MIN)
////	 {filter_Rudder_MIN=Rudder;}
////	 Sum_Rudder+=Rudder;
////	 Rudder_Sampling_Count++;
//	 filter_Rudder=Rudder>>4;
////	 if(Rudder_Sampling_Count>=4)
////	 {
////		 Sum_Rudder-=filter_Rudder_MAX;
////		 Sum_Rudder-=filter_Rudder_MIN;
////		 filter_Rudder=Sum_Rudder;
////		 filter_Rudder=filter_Rudder>>4;
////		 if(filter_Rudder>Rudder_MAX)
////		 {filter_Rudder=Rudder_MAX;}
////		 if(filter_Rudder<Rudder_MIN)
////		 {filter_Rudder=Rudder_MIN;}
////		 Rudder_Sampling_Count=0;
////		 Sum_Rudder=0;
////		 filter_Rudder_MAX=0;
////		 filter_Rudder_MIN=0xffff;
////	                            }
//     CAPCON1|=0x01;}
// else
// {CAPCON1&=~0x03;}
//}
/***************************************************************
* �������ܣ���ʼ��IO�˿�
* ���������
* ���������
***************************************************************/
void Init_IO(void)
{
    P0M1=P0M1_D;
    P0M2=P0M2_D;
    P0=P0_D;

    P1M1=P1M1_D;
    P1M2=P1M2_D;
    P1=P1_D;

    P2=P2_D;

    P3M1=P3M1_D;
    P3M2=P3M2_D;
    P3=P3_D;

}
void Set_System_Fre_10K(void)
{
set_LIRCST;//ʹ��10K IRC
set_OSC1;
clr_OSC0;

clr_BODEN;//�ر�Ƿѹ��⹦��
clr_IAPEN;//ʧ��IAP	
clr_HIRCEN;	//�ر�16M IRC
}
void Set_System_Fre_16M(void)
{
set_HIRCST;//ʹ��10K IRC
clr_OSC1;
clr_OSC0;
clr_BODEN;//�ر�Ƿѹ��⹦��
clr_IAPEN;//ʧ��IAP
clr_HIRCST;	//�ر�16M IRC	
	
}
/***************************************************************
* �������ܣ��Ի��Ѷ�ʱ��
* ���������
* ���������
***************************************************************/
void Init_WKT(void)
{
WKCON=Bin(00001011);//64��Ƶ
RWK=254;
set_EWKT;//�ж�ʹ��
EA = 1;
}
/***************************************************************
* �������ܣ���ʼ���ⲿ�ж�
* ���������
* ���������
***************************************************************/
void Init_EXT_IO_Interrupt(void)
{
PICON=Bin(00001000);
PINEN=Bin(00000010);
set_EPI;//ʹ�ܹܽ��ж�
EA = 1;
}
/***************************************************************
* �������ܣ���ʼ���ⲿ�ж�0
* ���������
* ���������
***************************************************************/
void Init_EXT0_Interrupt(void)
{
IT0=1;//�½��ش���
IE0=1;//���ж�
	EX0=1;
EA = 1;
}
/****************************************
***********************
* �������ܣ���ʼ���ⲿ�ж�1
* ���������
* ���������
***************************************************************/
void Init_EXT1_Interrupt(void)
{
	IT1=1;//�½��ش���
	IE1=1;//���ж�
	EX1=1;
	EA = 1;
}	
/***************************************************************
* �������ܣ�ϵͳ��ʼ��
* ���������
* ���������
***************************************************************/
void InitSystem(void)
{    
	  u16 BG_1_22V;
	  u16 VDD_VAL;
    Init_IO();
	 Delay100us();
	  LED_0FF;
//	  INIT_SC7A21();
	  SL_SC7A21_INIT();
    Timer0Init();
   // Timer1Init();
 // adc_init();
    InitialUART0_Timer3(115200);
		
    //ReadEeprom(); //��ȡ����
 Init_EXT0_Interrupt();
 Init_EXT1_Interrupt();
 Init_EXT_IO_Interrupt();
 Init_WKT();
//	Set_System_Fre_10K();
//  while(1)
//	{
//		if(LED_Cycle>=1)
//		{ LED_0FF;
//		  clr_WKTR;
//			PIF=0;
//		  set_EPI;}//���ڵ��˺������Զ�����
//		  set_PD;//��������	
//		set_PD;//��������	
//		};
	
}
