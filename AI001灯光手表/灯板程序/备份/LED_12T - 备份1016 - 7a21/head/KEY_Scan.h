#ifndef KEYSCAN_H
#define KEYSCAN_H
#define Long_Trg_Value    0xff
//#define KeyClean 		KEY1_Dev.Value=0;KEY2_Dev.Value=0;KEY3_Dev.Value=0;KEY4_Dev.Value=0;KEY5_Dev.Value=0;
//#define KeySecOn    KEY1_Dev.Value==1||KEY2_Dev.Value==1||KEY3_Dev.Value==1||KEY4_Dev.Value==1||KEY5_Dev.Value==1

enum Mode_Name{Single=0,Variety};
/*****************��������**********************/
typedef struct  
{	
	u8	 Mode;//����ģʽ��0�����´��� 1�����𴥷���֧�ֳ����������
	u8	 Lock;//������־
	u8   Trg;//�������
	u32  Up_Time;//����ʱ��
	u32  Down_Time;//�ϵ�ʱ��
  u8   Value;//����ֵ���������������Ϊoxff��	
}_KEY_Dev;
extern _KEY_Dev xdata	 KEY1_Dev;
 /*****************��������**********************/
void Key_Scan(void) ;
#endif


