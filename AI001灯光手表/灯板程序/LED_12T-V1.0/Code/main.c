
#include "ALL_Includes.h"
void main (void)
{	 
	InitSystem();
	while(1)
	{	if(LED_Cycle>=1)
		{ LED_0FF;
		  clr_WKTR;
			PIF=0;
		  set_EPI;}//���ڵ��˺������Զ�����
//		  BG_1_22V=ADC_Read(8);
//			VDD_VAL=(u32)4096*122/BG_1_22V;
//		  ANO_DT_Send_Version(0XF1, VDD_VAL, ADC_Read(4),0 , 0, 0);
		  set_PD;//��������	
		}
	
//  while(1)
//	{   
//    SysTimer_Process();
//			if(bsystem2Msec)
//			{
////				clr_HIRCST;		
//			}
//		if(bsystem5Msec)
//		{	
////			EA=0;
////			set_PD;//��������		
//			}
//		  if(bsystem10Msec)
//        {	
//                    }
//										
//		  if(bsystem1Sec)
//        {	
//					EA=0;
//					//set_PD;//��������	
//                    }

//  }		
}

