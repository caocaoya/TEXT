
#ifndef _SC7A21_H_
#define _SC7A21_H_
////////////////////SC7A21��ַ����///////////////
#define SC7A21_ADDR    0X18   //SDO�Žӵ�Ϊ0x18,���ջ��߽Ӹ�Ϊ0x19
////////////////////�Ĵ�����ַ����///////////////
#define	The_OUT_TEMP_L	0X0C//�¶ȵ�λ
#define	The_OUT_TEMP_H	0X0D//�¶ȸ�λ

#define	WHO_AM_I	      0X0F
#define	STEP_CTRL	      0X10//�Ʋ����ƼĴ���

#define	STEP_DELTA	    0X11//�Ʋ���ֵ���ƼĴ���

#define	STEP_WTM0	      0X12//�Ʋ���ֵ��λ
#define	STEP_WTM	      0X13//�Ʋ���ֵ��λ

#define	STEP_SRC	      0X14//�Ʋ�״̬

#define	STEP_COUNTER0	  0X15//�Ʋ�ֵ�Ĵ���1
#define	STEP_COUNTER1	  0X16//�Ʋ�ֵ�Ĵ���1
#define	STEP_COUNTER2	  0X17//�Ʋ�ֵ�Ĵ���2

#define	STEP_CONFIG1	  0X18//�Ʋ�ֵ���ƼĴ���0
#define	STEP_CONFIG2	  0X19//�Ʋ�ֵ���ƼĴ���1 

#define	CTRL_REG0	      0X1F//���ƼĴ���0
#define	CTRL_REG1	      0X20//���ƼĴ���1
#define	CTRL_REG2	      0X21//���ƼĴ���2
#define	CTRL_REG3	      0X22//���ƼĴ���3
#define	CTRL_REG4	      0X23//���ƼĴ���4
#define	CTRL_REG5	      0X24//���ƼĴ���5
#define	CTRL_REG6	      0X25//���ƼĴ���6

#define	REFERENCE	      0X26//�ο�

#define	STATUS_REG	    0X27//����״̬�Ĵ���
#define	OUT_X_L	        0X28//X ����ٶȼ�ֵ�� ���ֵ�� 2 �Ĳ������ʽ�����
#define	OUT_X_H	        0X29
#define	OUT_Y_L	        0X2A//Y ����ٶȼ�ֵ�� ���ֵ�� 2 �Ĳ������ʽ�����
#define	OUT_Y_H	        0X2B
#define	OUT_Z_L	        0X2C//Z ����ٶȼ�ֵ�� ���ֵ�� 2 �Ĳ������ʽ�����
#define	OUT_Z_H	        0X2D

#define	FIFO_CTRL_REG	  0X2E//FIFO ���ƼĴ���
#define	FIFO_SRC_REG	  0X2F//FIFO ״̬�Ĵ���

#define	A0I1_CFG	      0X30//AOI1 ���üĴ���
#define	AOI1_SOURCE	    0X31//AOI1 ״̬�Ĵ���
#define	AOI_THS	        0X32//AOI1 ��ֵ�Ĵ���
#define	AOI1_DURATION	  0X33//AOI1 ����ʱ��

#define	A012_CFG	      0X34//AOI2 ���üĴ���
#define	A012_SOURCE	    0X35//AOI2 ״̬�Ĵ���
#define	AO12_TSH	      0X36//AOI2 ��ֵ�Ĵ���
#define	AO12_DURATION	  0X37//AOI2 ����ʱ��

#define	CLICK_CFG	      0X38
#define	CLICK_the_SRC	  0X39
#define	CLICK_TSH	      0X3A

#define	TIME_LIMIT	    0X3B
#define	TIME_LATENCY	  0X3C
#define	TIME_WINDOW	    0X3D



#define	ACT_THS	        0X3E
#define	ACT_DURATION	  0X3F


void INIT_SC7A21(void);
void Read_STEP_COUNTER(void);
void Clean_STEP_COUNTER(void);
#endif 
