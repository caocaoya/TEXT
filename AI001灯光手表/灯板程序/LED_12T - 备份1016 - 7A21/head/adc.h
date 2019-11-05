/*
    Copyright (c) 2006 Michael P. Thompson <mpthompson@gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

    $Id$
*/

#ifndef _OS_ADC_H_
#define _OS_ADC_H_ 

#define ADC_CHANNEL_CURRENT         1//����
//#define ADC_CHANNEL_Seek_POSITION2  0//���õ�λ��2
//#define ADC_CHANNEL_BATTERY         1//��Դ
//#define ADC_CHANNEL_POSITION2       4//λ��2
#define ADC_CHANNEL_POSITION        3//λ��1
//#define ADC_CHANNEL_SEEK_POSITION  0//���õ�λ��1
//#define ADC_CHANNEL_POSITION        5//λ��1
//#define ADC_CHANNEL_TEMPERATURE     3//�¶�
#define ADC_CHANNEL_VOLTAGE        7//��ѹ
 typedef struct  
{	
	u8  Count;//����
	u8  Depth;//�˲����
	u32 Date_Sun;
	u16  Buff[65];//�˲�����ָ��
}_Filt_Dev;
void adc_init(void);
void ADC_GO(void);

extern volatile u8 adc_position_ready;
extern volatile u16 adc_position_value;



extern u16 adc_seek_position_value;

 extern _Filt_Dev ADC_Filt;//�������綯���˲�
void INIT_Filter(_Filt_Dev* Filt_Dev,u8 Depth);

#endif // _OS_ADC_H_
