#include "ALL_Includes.h"
#define MAX_PACKET_LEN  32// max value is 32
u8 Rx_buf[MAX_PACKET_LEN];//���ջ���
u8 TX_buf[MAX_PACKET_LEN];//���ջ���
code unsigned char  Reg[][2] = {
	  {STEP_CTRL,    Bin(00000001)},		//B7~B6,�Ʋ��ж�2��B5~B4,�Ʋ��ж�1��B2:����Ʋ���B1:ODRƵ�ʣ�B0:�Ʋ����ܿ���
    {STEP_DELTA,   Bin(00000000)},		//�Ʋ���ֵ����
    {STEP_WTM0,    Bin(00000000)},		//�Ʋ���ֵ��λ���üĴ���
    {STEP_WTM,     Bin(00000000)},		//�Ʋ���ֵ��λ���üĴ���
    {STEP_CONFIG1, Bin(00000000)},		//�Ʋ�ֵ���ƼĴ���1
    {STEP_CONFIG2, Bin(00000000)},		//�Ʋ�ֵ���ƼĴ���2
    {CTRL_REG0,    Bin(00000000)},		//�Ʋ���������b6:�¶ȴ�������B4:�˲�����,B3:������,B2:��ͨ�˲���B1~B0:�˲�Ƶ��
    {CTRL_REG1,    Bin(01000111)},		//B7~B4:�����ʣ�B3:0������1�͹��ģ�B2:Z��ʹ�ܣ�B2:Y��ʹ�ܣ�B2:X��ʹ�ܣ�
    {CTRL_REG2,    Bin(00000000)},		//B7~B6:��ͨģʽѡ��,B5~B4:��ͨ��ֹƵ��ѡ��,B3:�����˲�ѡ��,B2:CLICK ���ܸ�ͨ�˲�ʹ��,B1�ж� 2 AOI ���ܸ�ͨ�˲�ʹ��,B0:�ж� 1 AOI ���ܸ�ͨ�˲�ʹ��
    {CTRL_REG3,    Bin(00000000)},		//
    {CTRL_REG4,    Bin(00000000)},		//
    {CTRL_REG5,    Bin(00000000)},   //
    {CTRL_REG6,    Bin(00000000)},   //
    {REFERENCE,    Bin(00000000)},   //
    {STATUS_REG,   Bin(00000000)},		//
    {FIFO_CTRL_REG,Bin(00000000)},		//
    {A0I1_CFG,     Bin(00000000)},   //
    {AOI_THS,      Bin(00000000)},   //
    {AOI1_DURATION,Bin(00000000)},   //
    {A012_CFG,     Bin(00000000)},   //
    {AO12_TSH,     Bin(00000000)},   //
    {AO12_DURATION,Bin(00000000)},	  //
    {CLICK_CFG,    Bin(00000000)},		//
		{CLICK_TSH,    Bin(00000000)},		//
    {TIME_LIMIT,   Bin(00000000)},		//
    {TIME_LATENCY, Bin(00000000)},   //
    {TIME_WINDOW,  Bin(00000000)},   //
    {ACT_THS,      Bin(00000000)},   //
    {ACT_DURATION, Bin(00000000)},   //
};
 /***************************************************************
* �������ܣ���ʼ��SC7A21
* ���������
* ���������
***************************************************************/
void INIT_SC7A21(void)
{
u8 i,j=sizeof(Reg)/2;
for(i=0;i<j;i++)
{	
i2cs_tx_data(SC7A21_ADDR,Reg[i][0],&(Reg[i][1]),1);
}
}
 /***************************************************************
* �������ܣ����Ʋ�ֵ
* ���������
* ���������
***************************************************************/
void Read_STEP_COUNTER(void)
{
i2cs_rx_data(SC7A21_ADDR,STEP_COUNTER0,Rx_buf,3);
}
 /***************************************************************
* �������ܣ�����Ʋ�ֵ
* ���������
* ���������
***************************************************************/
void Clean_STEP_COUNTER(void)
{u8 temp;
i2cs_rx_data(SC7A21_ADDR,STEP_CTRL,&temp,1);
temp=temp&(~0x04);
i2cs_tx_data(SC7A21_ADDR,STEP_CTRL,&temp,1);
}

//void INIT_SC7A21(void)
//{
//i2cs_tx_data(uint8_t devAddr7,STEP_CTRL,uint8_t*buf,uint8_t Size);



//}
//����Ʋ�ֵ�Ĵ�������
