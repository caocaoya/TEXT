   #ifndef SYSTEM_H
   #define SYSTEM_H
	 
	#define Software_NO		15//����汾��
	#define MAIN_Fosc		  16000000//������ʱ��, ģ�⴮�ںͺ�����ջ��Զ���Ӧ��5~36MHZ
	#define freq_base			12//12Tģʽ
	#define Timer0_base			1000//��ʱ����ʱʱ�䣬��λ΢��
	#define Timer1_base			10000//��ʱ����ʱʱ�䣬��λ΢��
	#define Timer2_base			100//��ʱ����ʱʱ�䣬��λ΢��
	#define Timer3_base			1000//��ʱ����ʱʱ�䣬��λ΢��
	#define T0F			(20000/Timer0_base)//��ʱ����ʱʱ�䣬��λ΢��
	#define T1F     (1000000/Timer1_base)//��ʱ����ʱʱ�䣬��λ΢��
	//#define T2F     (65536-FOSC/1000)      //1Tģʽ
	#define Timer0_Reload		(65536 - (((u32)Timer0_base*(MAIN_Fosc/freq_base/1000))/1000))
//	#define Timer1_Reload		(65536 - (((u32)Timer1_base*(MAIN_Fosc/freq_base/1000))/1000))
	#define Timer2_Reload		(65536 - (((u32)Timer2_base*(MAIN_Fosc/freq_base/1000))/1000))
	#define Timer3_Reload		(65536 - (((u32)Timer3_base*(MAIN_Fosc/1000))/1000))
  #define Timer1_Reload(Cycle)		(65536 - (((u32)Cycle*(MAIN_Fosc/freq_base/1000))/1000))
	extern  u16 Timer_Count[];
//#define  S_Conut   Timer_Count[0]
//#define  S_Delay   Timer_Count[1]
//#define  ADC_Conut   Timer_Count[2]
	 /*****************��������**********************/
	 void InitSystem(void);
   extern u16  System_Timer;
   extern bit BIT_TMP;


#define bsystem1Msec      gTimer.Timer_Status.field.bit0
#define bsystem2Msec      gTimer.Timer_Status.field.bit1
#define bsystem5Msec      gTimer.Timer_Status.field.bit2
#define bsystem10Msec     gTimer.Timer_Status.field.bit3
#define bsystem20Msec     gTimer.Timer_Status.field.bit4
#define bsystem100Msec    gTimer.Timer_Status.field.bit5
#define bsystem500Msec    gTimer.Timer_Status.field.bit6
#define bsystem1Sec       gTimer.Timer_Status.field.bit7
#define Clear_Timer_Status {gTimer.Timer_Status.byte=0;}

#define bTemp1Msec        gTimer.SysTick_Status.field.bit0
#define bTemp2Msec        gTimer.SysTick_Status.field.bit1
#define bTemp5Msec        gTimer.SysTick_Status.field.bit2
#define bTemp10Msec       gTimer.SysTick_Status.field.bit3
#define bTemp20Msec       gTimer.SysTick_Status.field.bit4
#define bTemp100Msec      gTimer.SysTick_Status.field.bit5
#define bTemp500Msec      gTimer.SysTick_Status.field.bit6
#define bTemp1Sec         gTimer.SysTick_Status.field.bit7
#define Clear_SysTick_Status {gTimer.SysTick_Status.byte=0;}

typedef struct
{   unsigned char bit0:1;
    unsigned char bit1:1;
    unsigned char bit2:1;
    unsigned char bit3:1;
    unsigned char bit4:1;
    unsigned char bit5:1;
    unsigned char bit6:1;
    unsigned char bit7:1;
} Timer_Bit;
typedef union
{
     unsigned char byte;
     Timer_Bit field;
} Int_Field;
typedef struct
{
    unsigned char Tick1Msec;
	  unsigned char Tick20Msec;
     Int_Field SysTick_Status;
	   Int_Field Timer_Status;
} Timer_Struct;
typedef enum
{
    TIMER_RESET=0,
    TIMER_SET=1,
} TimerStatus;
extern u16  DelaymS;
extern Timer_Struct gTimer;
void SysTimer_Process(void);

#endif