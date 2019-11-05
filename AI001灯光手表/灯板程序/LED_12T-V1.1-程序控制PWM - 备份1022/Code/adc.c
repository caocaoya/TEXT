

#include "ALL_Includes.h"




 u8 adc_position_ready;
 u16 adc_position_value;
// u8 adc_voltage_needed;
u16 adc_seek_position_value;



void adc_init(void)
{

//    adc_position_ready = 0;
//    adc_position_value = 0;
    ADCCON1|=0x01;//��ADC��Դ
    AINDIDS=Bin(00010000);
	  ADCF=0;	
   // EADC=1;//ʹ��ADC�ж�
}

u16 ADC_Read(u8 CHS)
{ u16 new_value;
	set_BODEN;//��Ƿѹ��⹦�ܣ��ſ���ʹ��1.2V��׼��ѹ
  ADCCON0=(CHS&0x0f);
	ADCF=0;
	ADCS=1;
	while(ADCS==1);
	ADCF=0;
  new_value =(ADCRH<<4)+(ADCRL&0x0f);
	clr_BODEN;//�ر�Ƿѹ��⹦��
  return new_value;						 			
}
/***************************************************************
* �������ܣ�ADC��ʼ������10ms����һ�Σ�
* ���������
* ���������
***************************************************************/
void ADC_GO(void)
{
  ADCCON0=(ADC_CHANNEL_CURRENT&0x0f);
	ADCF=0;
	ADCS=1;
}
//bit full=0;

//void INIT_Filter(_Filt_Dev* Filt_Dev,u8 Depth)
//{   u8 i = 0;

//    Filt_Dev->Count = 0;
//    Filt_Dev->Depth = Depth;
//    for(i = 0; i < Depth; i++)
//    {
//        Filt_Dev->Buff[i] = 0;
//    }
//		Filt_Dev->Date_Sun=0;

//}

/***************************************************************
* �������ܣ��˲���
* ���������
* ���������
***************************************************************/

//u16 Filter(_Filt_Dev* Filt_Dev, u16 date)
//{       u16 Aver_date = 0;
//				Filt_Dev->Date_Sun -= Filt_Dev->Buff[Filt_Dev->Count];
//				Filt_Dev->Buff[Filt_Dev->Count]=date;
//				Filt_Dev->Date_Sun += date;
//        Filt_Dev->Count++;
//        if(Filt_Dev->Count == Filt_Dev->Depth)
//        {   Filt_Dev->Count = 0;
//            full=1;
//        }
//        if(full)
//        Aver_date = Filt_Dev->Date_Sun / Filt_Dev->Depth;
//				else
//				{Aver_date=date;}
//			 return 	Aver_date;
//}
/***************************************************************
* �������ܣ�ADC�жϷ�����
* ���������
* ���������
***************************************************************/

void ADC_ISR (void) interrupt 11  
{
    u16 new_value =(ADCRH<<4)+(ADCRL&0x0f);
						 			
		ADCF=0;
//    switch (ADCCON0 & 0x0f)
//    {

//        case ADC_CHANNEL_POSITION:

//            break;



//        case ADC_CHANNEL_CURRENT:

//             break;
//	      case ADC_CHANNEL_VOLTAGE:
//			
//             ADCCON0=(ADC_CHANNEL_POSITION&0x0f);
//	           ADCS=1;				
//             break;		
//			
//    }
		 
}
