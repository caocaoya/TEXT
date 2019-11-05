/* 
Copyright (c) 2017 Silan MEMS. All Rights Reserved.
*/

#ifndef SL_Watch_PEDO_ALGO_DRV__H__
#define SL_Watch_PEDO_ALGO_DRV__H__

/***ʹ������ǰ�����ʵ�ʽ����������******/
/**SC7A20��SDO �Žӵأ�  0****************/
/**SC7A20��SDO �Žӵ�Դ��1****************/
#define SL_SC7A20_SDO_VDD_GND            0
/*****************************************/

/***ʹ������ǰ�����ʵ��IIC�����������***/
/**SC7A20��IIC �ӿڵ�ַ���� 7bits��  0****/
/**SC7A20��IIC �ӿڵ�ַ���� 8bits��  1****/
#define SL_SC7A20_IIC_7BITS_8BITS        1
/*****************************************/

#if SL_SC7A20_SDO_VDD_GND==0
#define SL_SC7A20_IIC_7BITS_ADDR        0x18
#define SL_SC7A20_IIC_8BITS_WRITE_ADDR  0x30
#define SL_SC7A20_IIC_8BITS_READ_ADDR   0x31
#else
#define SL_SC7A20_IIC_7BITS_ADDR        0x19
#define SL_SC7A20_IIC_8BITS_WRITE_ADDR  0x32
#define SL_SC7A20_IIC_8BITS_READ_ADDR   0x33
#endif

#if SL_SC7A20_IIC_7BITS_8BITS==0
#define SL_SC7A20_IIC_ADDRESS        SL_SC7A20_IIC_7BITS_ADDR
#else
#define SL_SC7A20_IIC_WRITE_ADDRESS  SL_SC7A20_IIC_8BITS_WRITE_ADDR
#define SL_SC7A20_IIC_READ_ADDRESS   SL_SC7A20_IIC_8BITS_READ_ADDR
#endif

/********�ͻ���Ҫ���е�IIC�ӿڷ������****************/
extern unsigned char SL_SC7A20_I2c_Spi_Write(unsigned char sl_spi_iic,unsigned char reg, unsigned char Data);
extern unsigned char SL_SC7A20_I2c_Spi_Read(unsigned char sl_spi_iic,unsigned char reg, unsigned char len, unsigned char *buf);
/**SL_SC7A20_I2c_Spi_Write �����У� sl_spi_iic:0=spi  1=i2c  Reg���Ĵ�����ַ   data���Ĵ���������ֵ******************/
/**SL_SC7A20_I2c_Spi_Write ���� ��һ������д�ĺ���*******************************************************************/
/***SL_SC7A20_I2c_Spi_Read �����У� sl_spi_iic:0=spi  1=i2c Reg ͬ�ϣ�len:��ȡ���ݳ��ȣ�buf:�洢�����׵�ַ��ָ�룩***/
/***SL_SC7A20_I2c_Spi_Read ���� �ǿ��Խ��е��ζ�����������ȡ�ĺ���*************************************************/

/*************������ʼ������**************/
signed char SL_SC7A20_Driver_Init(unsigned char Sl_spi_iic_init,unsigned char Sl_pull_up_mode);
/***�������:1,Sl_spi_iic_init:0-1***2,PULL_UP_MODE:0x00 0x08 0x04 0x0c********/
/****Sl_spi_iic_init=0��SPI MODE, Sl_pull_up_mode config failed****************/
/****Sl_spi_iic_init=1��IIC MODE***********************************************/
/****Sl_pull_up_mode=0x00: SDO  I2C  pull up***********************************/
/****Sl_pull_up_mode=0x08: I2C  pull up and  SDO  open drain*******************/
/****Sl_pull_up_mode=0x04: SDO  pull up and  I2C  open drain*******************/
/****Sl_pull_up_mode=0x0C: SDO  I2C  open drain********************************/
/****SDO�Žӵأ��Ƽ��ر�SDO�ڲ���������****************************************/
/****SPIͨѶ���Ƽ��ر�SDA��SCL�ڲ��������裬SDO�ڲ�����������Զ��ر�**********/

/*************���������������*****************/
/**return : 0x11    ��ʾCHIP ID ����***********/
/**return : 0       ��ʾ��ȡ�쳣***************/
/**return :-1;      SPI ͨ������***************/
/**return :-2;      IIC ͨ������***************/
/**return :-3;      ������ȡ ͨ������***********/

/***************ִ���㷨ǰ��Ҫ��ȡFIFO����*****/
/***************FIFO���ݶ�ȡ��FIFO�����*****/
/***************����FIFOֻ�ܶ�ʱ��ȡһ��*******/
unsigned char SL_SC7A20_Read_FIFO(void);
/**return : FIFO_LEN    ��ʾ���鳤��***********/

/***************��ȡ����ĵ�ǰ�Ʋ�ֵ***************/
unsigned int SL_Watch_Pedo_Algo(void);
/**************�������Ϊ���Ʋ�ֵ(��)**************/

/***************��ȡ����ԭʼ����***************/
unsigned char SL_SC7A20_GET_FIFO_Buf(signed short *sl_x_buf,signed short *sl_y_buf,signed short *sl_z_buf);
/****************ִ���㷨��ִ�и�����**********/
/**x_buf  y_buf  z_buf : ����32�������׵�ַ****/
/****************���������������**************/
/**return : FIFO_LEN    ��ʾ���鳤��***********/


/******************��λ�Ʋ�ֵ************************/
void SL_Pedo_ResetStepCount(void);
/**********��������Ϊ����******�������Ϊ����********/
/*ʹ�÷���: ϵͳʱ�䵽�ڶ���ʱ�����øú�������Ʋ�ֵ*/

/******************���������üƲ�ֵ,����,����************************/
/******************���������ڳ�ʼ�����������Ӹú���*****************/
void SL_Pedo_StepCount_Set(unsigned int sl_pedo_value);
/**********��������Ϊ��sl_pedo_value  �Ʋ�ֵ*****/


/******************�Ʋ�����������********************/
void SL_PEDO_TH_SET(unsigned char sl_pedo_amp,unsigned char sl_pedo_th,unsigned char sl_pedo_weak);
/******sl_pedo_amp>10&&sl_pedo_amp<50**********************/
/******sl_pedo_amp:ԽС������Խ�ߣ�Խ���׼Ʋ�**************/
/******sl_pedo_amp:Խ��������Խ�ߣ�Խ�ѼƲ�****************/
/******sl_pedo_amp:Ĭ��ֵ26�������þ���26******************/

/******sl_pedo_th>5&&sl_pedo_th<50**********************/
/******sl_pedo_th:ԽС������Խ�ߣ�Խ���׼Ʋ�**************/
/******sl_pedo_th:Խ��������Խ�ߣ�Խ�ѼƲ�****************/
/******sl_pedo_th:Ĭ��ֵ10�������þ���10******************/

/******sl_pedo_weak>=0&&sl_pedo_weak<5**********************/
/******sl_pedo_th:0,������΢��·�Ʋ�����******************/
/******sl_pedo_th:1,����΢��·�Ʋ�����********************/
/******sl_pedo_th:2,����΢��·�Ʋ�����********************/
/******sl_pedo_th:3,����΢��·�Ʋ�����********************/
/******sl_pedo_th:4,����΢��·�Ʋ�����********************/
/******sl_pedo_th:Ĭ��ֵ0,���õ�ֵԽ�󣬼��Ʋ���Խ����****/

/*******�Ʋ���������****************/
void SL_PEDO_SET_AXIS(unsigned char sl_xyz);
/***sl_xyz:0  x��*******************/
/***sl_xyz:1  y��*******************/
/***sl_xyz:2  z��*******************/
/***sl_xyz:3  �㷨����**************/

/***********�ر�IIC�豸**************/
/***********Power down ����**********/
/********��ʹ�ø��豸�����**********/
/****����������ʹ��������ʼ������****/
signed char  SL_SC7A20_Power_Down(void);
/*************�������:��************/
/**********���ز������˵��**********/
/**return  1: Power Down Success*****/
/**return -1: Power Down Fail********/


#endif/****SL_Watch_ALGO_DRV__H__****/



