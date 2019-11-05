

#include "ALL_Includes.h"
//�������������������������������á���������������������������
#define SL_Sensor_Algo_Release_Enable 1
#if SL_Sensor_Algo_Release_Enable==0
extern signed short          SL_DEBUG_DATA[9][128];
extern unsigned char         SL_DEBUG_DATA_LEN;
#endif

static unsigned char   SL_SLEEP             = 0;
static unsigned short  sl_sleep_cnt         = 0;
static unsigned short  sl_sleep_flag        = 0;
static unsigned char   SL_DEBUG_SLEEP       = 0;
static unsigned char   SL_WRIST             = 0;

/**Please modify the following parameters according to the actual situation**/

signed char SL_SC7A21_INIT(void)
{
    unsigned char status=0;
//�������������������������������Ʋ��������á�������������������������������������	
    unsigned char pedo_para[7] ={//10h 18h 19h 1Ah 11h 12h 13h
		SL_SC7A21_STEP_EN|SL_SC7A21_STEP_MODE_25Hz|SL_SC7A21_STEP_INT1_MODE0|SL_SC7A21_STEP_INT2_MODE0|0x04,/*�ж�����*/
		SL_SC7A21_STEP_AXC_TH2|SL_SC7A21_STEP_ST_TH4/*����ֵԽ����ʼ�Ʋ��Ѷ�Խ��*/|SL_SC7A21_STEP_DIFF4,/*����ֵԽС�����˶��Ĺ�����Ҫ��Խ��*/
		SL_SC7A21_STEP_AXS_TH0|SL_SC7A21_STEP_BL_TH3/*����ֵԽС������Խ��ļƲ�����*/|SL_SC7A21_STEP_UL_TH7,/*����ֵԽ������Խ���ļƲ�����*/
		SL_SC7A21_STEP_SEN_TH3,/*����ֵԽ�󣬴����Ʋ��˶�����Խ�� ����ֵԽС�������Ʋ��˶�����ԽС*/
		SL_SC7A21_STEP_DELTA_5,/*�Ʋ���ֵ*/
		0xFF,/*�Ʋ���ֵ��λ*/
		0x00/*�Ʋ���ֵ��λ*/};
//������������������������������̧�ֲ������á�����������������������������������		
    unsigned char wrist_para[4]={//08h 09h 0Ah 0Bh
			SL_SC7A21_WRIST_EN|SL_SC7A21_WRIST_INT1_MODE3,
			SL_SC7A21_WRIST_A_SCALE3/*����Ƕ��������λ*/|SL_SC7A21_WRIST_SENSOR_POS6,
			SL_SC7A21_WRIST_ANGLE_5,/*̧��Ŀ��Ƕ�ֵ����λ*/
			SL_SC7A21_WRIST_SEN2/*̧��ʶ��������*/|SL_SC7A21_WRIST_TT_TH2/*̧��״̬�����������ʱ�������λ*/|SL_SC7A21_WRIST_FT_TH3/*�뿪̧��״̬�����������ʱ�������λ*/
//		   0x00,
//		   0x93
		};//̧���ж����ã�̧�ֲ������ã�08~0B��
//������������������������������˯�߲������á�����������������������������������		
    unsigned char sleep_para[2]={
    SL_SC7A21_SLEEP_EN|SL_SC7A21_SLEEP_INT_MODE1/*DRDY_S ӳ�䵽�ж� 1 ��*/,
		SL_SC7A21_SLEEP_BIT0|SL_SC7A21_SLEEP_TH2|SL_SC7A21_SLEEP_TIME1}; //˯��
//�������������������������������жϲ������á�����������������������������������			
    unsigned char int_para     =0x82;//default int level set high,int output mode set pushpull 
//����������������������������������������á�����������������������������������			
    unsigned char click_para[7]={
		0x01,/*ODR����Ƶ��*/
		SL_SC7A21_HPF_HPCF3|SL_SC7A21_HPF_OUT_EN|SL_SC7A21_HPF_CLICK_EN,
		SL_SC7A21_INT2_CLICK|SL_SC7A21_INT_ACT_L_LEVEL,
		0x15,
		0x48,/*��˫����������ֵ*/
		0x04,/*��˫�����ʱ�����ֵ*/
		0x17/*�ӳ�ʱ��*/
		};//int2 //20 21 25 38 3A 3B 3C

/*****if use spi mode please config first******/    
#if SL_SC7A21_SPI_IIC_MODE == 0//spi
    //disable SDO & SDA & SCL PULL-UP Resistance
    status=SL_SC7A21_Online_Test(SL_SC7A21_SDO_I2C_DIS);//pull up config
#else
    status=SL_SC7A21_Online_Test(SL_SC7A21_SDO_PU_DIS);//pull up config//�ر��ڲ�����
#endif
    /**********set sc7a21 full-scale*****************************/
    status=SL_SC7A21_FS_Config(SL_SC7A21_DATA_FS_4G);//��������������
    /**********set sc7a21 odr and power mode*********************/
    status=SL_SC7A21_Power_Config(SL_SC7A21_ODR_50HZ,SL_SC7A21_LOWER_POWER);//ODR���ã��͹��Ĺ���ģʽ����
    /**********set sc7a21 FIFO MODE and 8BITS FIFO MODE**********/ 
    status=SL_SC7A21_FIFO_Config(SL_SC7A21_STREAM_MODE_EN,SL_SC7A21_FIFO_8BITS_MODE);//FIFOģʽ��λ������
    /**********set sc7a21 INT LEVEL AND INT MODE*****************/ 
    status=SL_SC7A21_INT_Config(int_para);   //�жϵ�ƽ���ж�ģʽ����
    /**********10h 18h 19h 1Ah 11h 12h 13h  Total:7**************/
    SL_SC7A21_Pedo_Config(&pedo_para[0]);//�Ʋ���������
    /**********08h 09h 0Ah 0Bh  Total:4**************************/
    SL_SC7A21_Wrist_Config(&wrist_para[0]);//̧�ֲ�������
    /**********6Bh 6Ch   Total:2*********************************/
    //must open this function an hour early
    SL_SC7A21_Sleep_Config(&sleep_para[0]);//˯�߲�������
    sl_sleep_flag = 0;
    //if click function is open,it must open another timer
    SL_SC7A21_Single_Click_Config(&click_para[0]);
		
		//SL_SC7A21_Power_Config(SL_SC7A21_ODR_25HZ,SL_SC7A21_LOWER_POWER); 

	return status;
}
//signed char SL_SC7A21_INIT(void)
//{
//    unsigned char status=0;
//    unsigned char pedo_para[7] ={0x05,0xA4,0x1F,0x37,0x0F,0xFF,0x00};
//    unsigned char wrist_para[4]={0x31,0x31,0x00,0x93};//int1
//    unsigned char sleep_para[2]={0x11,0x09}; 
//    unsigned char int_para     =0x02;//default int level set high,int output mode set pushpull  
//    unsigned char click_para[7]={0x01,0x3C,0x82,0x15,0x48,0x04,0x17};//int2 //20 21 25 38 3A 3B 3C

///*****if use spi mode please config first******/    
//#if SL_SC7A21_SPI_IIC_MODE == 0//spi
//    //disable SDO & SDA & SCL PULL-UP Resistance
//    status=SL_SC7A21_Online_Test(SL_SC7A21_SDO_I2C_DIS);//pull up config
//#else
//    status=SL_SC7A21_Online_Test(SL_SC7A21_SDO_PU_DIS);//pull up config
//#endif
//    /**********set sc7a21 full-scale*****************************/
//    status=SL_SC7A21_FS_Config(SL_SC7A21_DATA_FS_4G);
//    /**********set sc7a21 odr and power mode*********************/
//    status=SL_SC7A21_Power_Config(SL_SC7A21_ODR_50HZ,SL_SC7A21_LOWER_POWER);
//    /**********set sc7a21 FIFO MODE and 8BITS FIFO MODE**********/ 
//    status=SL_SC7A21_FIFO_Config(SL_SC7A21_STREAM_MODE_EN,SL_SC7A21_FIFO_8BITS_MODE);
//    /**********set sc7a21 INT LEVEL AND INT MODE*****************/ 
//    status=SL_SC7A21_INT_Config(int_para);   
//    /**********10h 18h 19h 1Ah 11h 12h 13h  Total:7**************/
//    SL_SC7A21_Pedo_Config(&pedo_para[0]);
//    /**********08h 09h 0Ah 0Bh  Total:4**************************/
//    SL_SC7A21_Wrist_Config(&wrist_para[0]);
//    /**********6Bh 6Ch   Total:2*********************************/
//    //must open this function an hour early
//    SL_SC7A21_Sleep_Config(&sleep_para[0]);
//    sl_sleep_flag = 0;
//    //if click function is open,it must open another timer
//  SL_SC7A21_Single_Click_Config(&click_para[0]);
////SL_SC7A21_Power_Config(SL_SC7A21_ODR_400HZ,0);//400Hz
//	return status;
//}
//��ʱ1min ����һ��
#define SL_SC7A21_SLEEP_WAKE_TH    2//0-1-2
#define SL_SC7A21_SLEEP_LIGHT_TH   5//3-4-5//�ж�˯��
#define SL_SC7A21_SLEEP_DEEP_TH    7//6-7//���˯��
unsigned char  SL_SC7A21_GET_SLEEP_VALUE(void)
{
    unsigned char   sl_sleep_status      = 0;
    
    sl_sleep_status =  SL_SC7A21_Get_Sleep_Status();
    SL_DEBUG_SLEEP  =  sl_sleep_status;
    if(sl_sleep_flag==0)//pre-judge
    {
        if(sl_sleep_status>1)
        {
            sl_sleep_cnt++;
            if(sl_sleep_cnt>20)//already 20min
            {
                sl_sleep_cnt=0;
                sl_sleep_flag=1;
                return  sl_sleep_status/2;           
            }
            else
                return 0;//wake status
        }
        else
        {
            sl_sleep_cnt =0;
            sl_sleep_flag=1;
        }
    }

    if(sl_sleep_status<=SL_SC7A21_SLEEP_WAKE_TH)
    {
        return 0;//wake status
    }
    else if(sl_sleep_status<=SL_SC7A21_SLEEP_LIGHT_TH)
    {
        return 1;//light status
    }
    else
    {
        return 2;//deep status
    }

    return sl_sleep_status;
}

//GPIO�ж��е���
/***SC7A21 INT1 INT2�жϷ������У���ȡ̧��״̬***/
/***�Ƽ� INT1**************************************/
//return 0: no action
//return 1: turn on   success
//return 2: turn down success
unsigned char  SL_SC7A21_WRIST_INT12_SERVICE_FUNCTION(void)
{
    SL_WRIST=SL_SC7A21_Get_Wrist_Status();
    return   SL_WRIST;
}

//GPIO�ж��е��ã��Ƽ�INT2
//Service function in Int Functi  on �жϺ����еķ�����
unsigned char  sl_click_timer_en     =0;
unsigned char  sl_click_status       =0;
unsigned short click_timer_cnt       =0;
unsigned short click_timer_total_cnt =0;
unsigned char  click_click_final_cnt =0;
unsigned char  SL_SC7A21_CLICK_SERVICE(void)
{
	unsigned char click_status=0;

	click_status=SL_SC7A21_Click_Algo(70,60);
	
	if(click_status==1) //���ֵ��� 
	{
		if(sl_click_timer_en==0)
		{
			//set click timer flag
			sl_click_timer_en    =1;
			//open click timer
			SL_Open_Sc7a21_Click_Timer();
			//clear click timer cnt value
			click_timer_cnt      =0;
			click_timer_total_cnt=0;
			click_click_final_cnt=0;
		}
		sl_click_status=1;
	}

	return click_status;
}

#define       click_pp_num    (unsigned short)10  //0.5s
#define       click_max_num   (unsigned short)100 //5s
//This fuction is execute in 50ms timer when the timer is opened
unsigned char SL_SC7A21_Click_Timer_Cnt(void)//50ms
{
	unsigned char click_e_cnt=0;
		
	if(sl_click_timer_en==1)
	{
		click_timer_cnt++;
		if((click_timer_cnt<click_pp_num)&&(sl_click_status==1))
		{
			sl_click_status=0;
			click_timer_total_cnt=click_timer_total_cnt+click_timer_cnt;
			click_timer_cnt=0;
			click_click_final_cnt++;
		}
		
		click_e_cnt=SL_Get_CLICK_PP_CNT(1);
		if((((click_timer_cnt>=click_pp_num)||(click_timer_total_cnt>=click_max_num))&&(click_e_cnt<2))||
		   ((click_timer_cnt>=click_pp_num)&&(click_e_cnt>1)))
		{
			//close click timer
			SL_Close_Sc7a21_Click_Timer();//�رջ���ʱ��
			//clear click timer flag
			sl_click_timer_en=0;
			//clear click timer cnt value
			click_timer_cnt      =0;
			click_timer_total_cnt=0;
			
			if(click_e_cnt>0)
			{
				click_e_cnt=SL_Get_CLICK_PP_CNT(0);
				return 0;
			}
			else
			{
				return click_click_final_cnt;
			}
		}
	}
	return 0;
}

//��ʱ1s��0.5s����һ��
//#define SL_SC7A21_TIMER_INTERVAL   1000
#define SL_SC7A21_TIMER_INTERVAL   500//ʱ����
static unsigned char sc7a21_timer_counter=0;   
static unsigned int  SL_STEP            = 0;
unsigned int  SL_SC7A21_GET_PEDO_VALUE(void)
{
#if SL_Sensor_Algo_Release_Enable==0
	unsigned char         sl_i;
#endif
  
#if   SL_SC7A21_FIFO_16BIT_8BIT==0
	signed char  SC7A21_XYZ_Buf[3][64];
#elif SL_SC7A21_16BIT_8BIT==1
	signed short SC7A21_XYZ_Buf[3][32];
#endif
	unsigned char         SC7A21_FIFO_NUM;
    
    /*******get accel orginal fifo data and length*******/
#if   SL_SC7A21_FIFO_16BIT_8BIT==0
    SC7A21_FIFO_NUM=SL_SC7A21_Read_XYZ_FIFO_Data(&SC7A21_XYZ_Buf[0][0],&SC7A21_XYZ_Buf[1][0],&SC7A21_XYZ_Buf[2][0]);
#elif SL_SC7A21_16BIT_8BIT==1
    SC7A21_FIFO_NUM=SL_SC7A21_Read_XYZ_FIFO_Data(&SC7A21_XYZ_Buf[0][0],&SC7A21_XYZ_Buf[1][0],&SC7A21_XYZ_Buf[2][0]);
#endif

    /*******get pedo value******/
    SL_STEP= SL_SC7A21_Get_Pedo_Count();

    if(SL_STEP>200)
    {
        /**reset pedo value,just for example**/
        SL_SC7A21_Pedo_Reset();
    }
  //  ANO_DT_Send_Version(0XF1, SL_STEP, 0,0 , 0, 0);
    sc7a21_timer_counter++; 
#if   SL_SC7A21_TIMER_INTERVAL==500//1���Ӽ��һ��˯��״̬
    if(sc7a21_timer_counter>=120)
    {
        SL_SLEEP=SL_SC7A21_GET_SLEEP_VALUE();
        sc7a21_timer_counter=0;
    }
#elif SL_SC7A21_TIMER_INTERVAL==1000
    if(sc7a21_timer_counter>=60)
    {
        SL_SLEEP=SL_SC7A21_GET_SLEEP_VALUE();
        sc7a21_timer_counter=0;
    } 
#endif
    
#if SL_Sensor_Algo_Release_Enable==0
    for(sl_i=0;sl_i<SC7A21_FIFO_NUM;sl_i++)
    {       
        SL_DEBUG_DATA[0][sl_i]=SC7A21_XYZ_Buf[0][sl_i];
        SL_DEBUG_DATA[1][sl_i]=SC7A21_XYZ_Buf[1][sl_i];
        SL_DEBUG_DATA[2][sl_i]=SC7A21_XYZ_Buf[2][sl_i];
        SL_DEBUG_DATA[3][sl_i]=SL_SLEEP;
        SL_DEBUG_DATA[4][sl_i]=SL_DEBUG_SLEEP;
        SL_DEBUG_DATA[5][sl_i]=SL_STEP;
        SL_DEBUG_DATA[6][sl_i]=SL_WRIST;
    }
    SL_DEBUG_DATA_LEN=SC7A21_FIFO_NUM;
    SL_WRIST = 0;//Clear this para
#endif

    return SL_STEP;
}

