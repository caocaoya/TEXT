
#include "ALL_Includes.h"
// The minimum and maximum servo position as defined by 10-bit ADC values.
#define MIN_POSITION            (0)//��Сλ��ֵ
#define MAX_POSITION            0x1000//���λ��ֵ
// PID�������������Сֵ
#define MAX_OUTPUT              (255)//������
#define MIN_OUTPUT              (-MAX_OUTPUT)//��С���

// Values preserved across multiple PID iterations.
static int16 previous_seek=-1; // ע��:previous_seek = = 1��ʾ��ʼ�������
static int16 previous_position=0;//��һ��λ�ñ���

static int16 seek_delta=-1;//Ŀ��λ������
static int16 position_at_start_of_new_seek=-1;//����Ѱ�Ŀ�ʼλ��
static u8 previous_pwm_is_enabled=0;

     int16 seek_position;//Ŀ��λ��
     int16 seek_velocity;//Ѱ���ٶ�
     int16 minimum_position;//��Сλ��
     int16 maximum_position;//���λ��
 u8 Reverse_Seek=0;
 PID_Dir PID;




//
// Digital Lowpass Filter Implementation
//
// See: A Simple Software Lowpass Filter Suits Embedded-system Applications
// http://www.edn.com/article/CA6335310.html
//
// k    Bandwidth (Normalized to 1Hz)   Rise Time (samples)
// 1    0.1197                          3
// 2    0.0466                          8
// 3    0.0217                          16
// 4    0.0104                          34
// 5    0.0051                          69
// 6    0.0026                          140
// 7    0.0012                          280
// 8    0.0007                          561
//

#define FILTER_SHIFT 1

static int32 filter_reg = 0;

static int16 filter_update(int16 input)//�����˲�
{
#if 0
    // Update the filter with the current input.�õ�ǰ��������˲���
#if FULL_ROTATION_ENABLED
    filter_reg += normalize_position_difference(input - (filter_reg >> FILTER_SHIFT));
#else
    filter_reg = filter_reg - (filter_reg >> FILTER_SHIFT) + input;
#endif

    // Scale output for unity gain.
    return (int16) (filter_reg >> FILTER_SHIFT);
#else
    return input;
#endif
}

void pid_init(void)
// Initialize the PID algorithm module.��ʼ��pid�㷨ģ��
{
   previous_seek = -1;
}


void pid_registers_defaults(void)
{

	  PID.deadband=DEFAULT_PID_DEADBAND;
		PID.p_gain  =DEFAULT_PID_PGAIN;
		PID.i_gain  =DEFAULT_PID_IGAIN;
		PID.d_gain  =DEFAULT_PID_DGAIN;
	
    minimum_position=DEFAULT_MIN_SEEK;
	  maximum_position=DEFAULT_MAX_SEEK;
	  Reverse_Seek=0;//�������־
	
}


int16 pid_position_to_pwm(int16 current_position, u8 tick)
// This is a modified pid algorithm by which the seek position and seek
// velocity are assumed to be a moving target.  The algorithm attempts to
// output a pwm value that will achieve a predicted position and velocity.
//����һ�ָĽ���PID�㷨��ͨ����������λ�ú�Ѱ�ҡ�
//�ٶȱ��ٶ�Ϊ�˶�Ŀ�ꡣ���㷨��ͼ���һ��PWMֵ���ԴﵽԤ���λ�ú��ٶȡ�
{
    // We declare these static to keep them off the stack.

    static int16 current_velocity;//�����ٶ�
    static int16 filtered_position;//�˲����λ��

    static u8    pwm_is_enabled;//PWMʹ�ܱ�־
	
// Get flag indicating if PWM is enabled or not. This saves fetching it each time
// we want to know.
//��ȡָʾ�Ƿ�������PWM�ı�־�����ʡÿ����ȡ����
//������֪��
//    pwm_is_enabled=registers_read_byte(REG_FLAGS_LO)&(1<<FLAGS_LO_PWM_ENABLED);

    // ͨ�����ֵ�ͨ�˲����Ե�ǰλ�ý����˲���
    filtered_position = filter_update(current_position);

    // ʹ�ù���λ��ȷ���ٶ�
#if FULL_ROTATION_ENABLED
    current_velocity = normalize_position_difference(filtered_position - previous_position);
#else
    current_velocity = filtered_position - previous_position;//��ǰ�ٶ�
#endif

    // ��ȡĿ��λ�ú��ٶ�
//    seek_position = (int16) registers_read_word(REG_SEEK_POSITION_HI, REG_SEEK_POSITION_LO);
//    seek_velocity = (int16) registers_read_word(REG_SEEK_VELOCITY_HI, REG_SEEK_VELOCITY_LO);
    // ��ȡ��С�����λ�á�
//    minimum_position = (int16) registers_read_word(REG_MIN_SEEK_HI, REG_MIN_SEEK_LO);
//    maximum_position = (int16) registers_read_word(REG_MAX_SEEK_HI, REG_MAX_SEEK_LO);

// Are we reversing the seek sense?�����Ƿ�ߵ���Ѱ�����壿
// TODO: What is the point of this? Surely it is better to correct the wires to the motor than����ʲô��˼����Ȼ����ðѵ���ĵ���У���á�
//       risk accidents?/�����¹ʣ�
    if (Reverse_Seek!= 0)
    {
        // Yes. Update the position and velocity using reverse sense.���÷���о�����λ�ú��ٶ�
//        registers_write_word(REG_POSITION_HI, REG_POSITION_LO, (u16) (MAX_POSITION - current_position));
//        registers_write_word(REG_VELOCITY_HI, REG_VELOCITY_LO, (u16) -current_velocity);
        // Reverse sense the seek and other position values.����Ѱ��λ�ú�����λ��ֵ
           seek_position = MAX_POSITION - seek_position;
        minimum_position = MAX_POSITION - minimum_position;
        maximum_position = MAX_POSITION - maximum_position;
			
    }
    else
    {
        // No. Update the position and velocity registers without change.�ޱ仯�ظ���λ�ú��ٶȼĴ���
//        registers_write_word(REG_POSITION_HI, REG_POSITION_LO, (u16) current_position);
//        registers_write_word(REG_VELOCITY_HI, REG_VELOCITY_LO, (u16) current_velocity);
    }

    // ��ȡ����
//    PID.deadband = (int16) registers_read_byte(REG_PID_DEADBAND);

// Keep the seek position bound within the set minimum and maximum position and the hardware
// defined limits.
// NOTE: Even when full rotation is enabled, the limit of the set min and max
//       positions are still applied: these must be set appropriately to allow
//       full rotation to occur.
//
//       TODO: When full rotation is enabled, it may be that the user desires the
//             range to be exclusive rather than inclusive.

    if (seek_position < minimum_position) seek_position = minimum_position;
    if (seek_position > maximum_position) seek_position = maximum_position;
    if (seek_position < MIN_POSITION)     seek_position = MIN_POSITION;
    if (seek_position > MAX_POSITION)     seek_position = MAX_POSITION;

// Check for new seek target
    if(previous_seek != seek_position )    // PWM enable state has changed...PWM����״̬�Ѹı�
    { 
       if(previous_seek == -1)                       // Initialisation��ʼ��
       {
          previous_position = current_position;
          PID.i_component = 0;
				  seek_delta = current_position;
       }
		  
     	 previous_seek = seek_position;  
			  seek_delta=previous_seek;
//     seek_delta = current_position;//???????
//		 seek_delta=seek_position;
       position_at_start_of_new_seek = current_position;//����Ŀ��λ��ʱ�����λ��
       previous_pwm_is_enabled = pwm_is_enabled;
			 
    }
//Send_Word_To_UART0 (current_position);
//
// Update seek target
//
    if(tick && seek_delta!=seek_position && seek_velocity>0) // Tick is our time constant
    {
       if(position_at_start_of_new_seek<seek_position)
       {
             seek_delta+=seek_velocity;
          if(seek_delta>=seek_position)
          {
             seek_delta=seek_position;
          }
       } else
       {
          if(position_at_start_of_new_seek>seek_position)
          {
             seek_delta-=seek_velocity;
             if(seek_delta<=seek_position)
             {
                seek_delta=seek_position;
             }
          }
       }
    }

    if(seek_delta==seek_position)
    {
       current_position = filtered_position;
    }

// Calculate PWM
#if FULL_ROTATION_ENABLED
    p_component = normalize_position_difference(seek_delta - current_position);
#else
    // The proportional component to the PID is the position error.PID�ı���������λ�����
    PID.p_component = seek_delta - current_position;
#endif

    // The integral component
    if(tick) // Tick is our time constant
    {
       PID.i_component += PID.p_component;
		
       if(PID.i_component<PID_I_MIN) // Somewhat arbitrary anti integral wind-up; we're experimenting�е���ϵķ����֣�������������
       {
          PID.i_component=PID_I_MIN;
       } else
       {
          if(PID.i_component>PID_I_MAX)
          {
             PID.i_component=PID_I_MAX;
          }
       }
    }

    // The derivative component to the PID is the change in position.PID�ĵ���������λ�õı仯
    PID.d_component = previous_position - current_position;
    previous_position = current_position;

    // Get the proportional, derivative and integral gains.
//    PID.p_gain = registers_read_word(REG_PID_PGAIN_HI, REG_PID_PGAIN_LO);
//    PID.i_gain = registers_read_word(REG_PID_IGAIN_HI, REG_PID_IGAIN_LO);
//    PID.d_gain = registers_read_word(REG_PID_DGAIN_HI, REG_PID_DGAIN_LO);

    // Start with zero PWM output.
    PID.pwm_output = 0;

    // Apply proportional component to the PWM output if outside the deadband.
    if ((PID.p_component > PID.deadband) || (PID.p_component < -PID.deadband))
    {
        // Apply the proportional component of the PWM output.
        PID.pwm_output += (int32) PID.p_component * (int32) PID.p_gain;

    // Apply the integral component of the PWM output.
        PID.pwm_output += (int32) PID.i_component * (int32) PID.i_gain;

    // Apply the derivative component of the PWM output.
        PID.pwm_output += (int32) PID.d_component * (int32) PID.d_gain;
    } else
    {
       PID.i_component = 0;
    }

    // Shift by 8 to account for the multiply by the 8:8 fixed point gain values.
    // NOTE: When OpenEncoder is enabled an extra 1 place of shift is applied
    //       to account for the increased precision (which is approximately a
    //       factor of 2) so that the magnitude of the gain values are similar
    //       across the two different platforms.
    //
    //       Factor of 2: pot measurements are typically approaching 180 degrees
    //       across the 0 to 1023 ADC range. OpenEncoder is 360 degrees across the
    //       0 to 4096 range.
    //
//��8ռ��8������ֵ��
//ע�⣺��OpenEncoder���ö����1λ��λ��Ӧ��
//���ǵ����ȵ���ߣ���Լ��
//����2��ʹ����ֵ�Ĵ�С���ơ�
//��Խ������ͬ��ƽ̨��

//2���أ��޲���ͨ���ӽ�180�ȡ�
//��Խ0��1023 ADC��Χ��openencoder��360����0��4096�ķ�Χ�ڡ�
#if ENCODER_ENABLED
    PID.pwm_output >>= 9;
#else
    PID.pwm_output >>= 8;//pwm_output/256
#endif

    // Check for output saturation.
    if (PID.pwm_output > MAX_OUTPUT)
    {
        // Can't go higher than the maximum output value.
        PID.pwm_output = MAX_OUTPUT;
    }
    else if (PID.pwm_output < MIN_OUTPUT)
    {
        // Can't go lower than the minimum output value.
        PID.pwm_output = MIN_OUTPUT;
    }

    return (int16) PID.pwm_output;
		
		
}

