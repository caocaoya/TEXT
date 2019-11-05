#include "ALL_Includes.h"
#if CURVE_MOTION_ENABLED

// �������
u8 motion_head;//ͷ
u8 motion_tail;//β
u32 motion_counter;//�˶�������
u32 motion_duration;//�˶�ʱ����;

motion_key keys[MOTION_BUFFER_SIZE];
 
static float int_to_float(int16 a)
// 16bit unsigned integer to float.
{
    return (float) a;
}


static int16 float_to_int(float a)
// Float to 6:10 signed fixed.
{
    return (int16) (a + 0.5);
//	return (int16) (a );
}


static float fixed_to_float(int16 a)
// 6:10 signed fixed point to float.
{
    return ((float) a) / 1024.0;
}


#if 0
static int16 float_to_fixed(float a)
// Float to 6:10 signed fixed.
{
    return (int16) (a * 1024.0);
}
#endif


void motion_init(void)
// Initialize the curve buffer.
{
    // Initialize the counter.
    motion_counter = 0;

    // Initialize the duration.
    motion_duration = 0;

    // Initialize the queue.
    motion_head = 0;
    motion_tail = 0;

    // Initialize the keypoint.
    keys[0].delta = 0;
    keys[0].position = 512.0;
    keys[0].in_velocity = 0.0;
    keys[0].out_velocity = 0.0;

	
	  keys[1].delta = 0;
    keys[1].position = 512.0;
    keys[1].in_velocity = 0.0;
    keys[1].out_velocity = 0.0;
		
			
	  keys[2].delta = 0;
    keys[2].position = 512.0;
    keys[2].in_velocity = 0.0;
    keys[2].out_velocity = 0.0;
					
	  keys[3].delta = 0;
    keys[3].position = 512.0;
    keys[3].in_velocity = 0.0;
    keys[3].out_velocity = 0.0;
							
	  keys[4].delta = 0;
    keys[4].position = 512.0;
    keys[4].in_velocity = 0.0;
    keys[4].out_velocity = 0.0;
							
	  keys[5].delta = 0;
    keys[5].position = 512.0;
    keys[5].in_velocity = 0.0;
    keys[5].out_velocity = 0.0;	
							
	  keys[6].delta = 0;
    keys[6].position = 512.0;
    keys[6].in_velocity = 0.0;
    keys[6].out_velocity = 0.0;		
							
	  keys[7].delta = 0;
    keys[7].position = 512.0;
    keys[7].in_velocity = 0.0;
    keys[7].out_velocity = 0.0;			
    // Initialize an empty hermite curve at the center servo position.
	//���м��ŷ�λ�ó�ʼ��һ���յİ����������ߡ�
    curve_init(0, 0, 512.0, 512.0, 0.0, 0.0);
    // ���üĴ�����
    motion_registers_reset();
}


void motion_reset(int16 position)
// Reset the motion buffer to the specified position.  The enabled state is preserved.���˶�����������Ϊָ��λ�á�����״̬���档
{
    // Reset the counter.���ü�������
    motion_counter = 0;

    // Reset the duration.
    motion_duration = 0;

    // Reset the queue.
    motion_head = 0;
    motion_tail = 0;

    // Reset the keypoint.
    keys[0].delta = 0;
    keys[0].position = int_to_float(position);
    keys[0].in_velocity = 0.0;
    keys[0].out_velocity = 0.0;

    // Initialize an empty hermite curve.  This is a degenerate case for the hermite
    // curve that will always return the position of the curve without velocity.
//	��ʼ��һ���յİ����������ߡ����Ƕ����ص��˻����Ρ�
//  ���ߣ����Ƿ������ߵ�λ�ã�û���ٶ�
    curve_init(0, 0, keys[0].position, keys[0].position, 0.0, 0.0);

    //Reset the registers.
    motion_registers_reset();
}


void motion_registers_reset(void)
// Reset the motion registers to zero values.
{
    // Set the default position, velocity and delta data.����Ĭ��λ�á��ٶȺ�delta����
//    registers_write_word(REG_CURVE_POSITION_HI, REG_CURVE_POSITION_LO, 0);
//    registers_write_word(REG_CURVE_IN_VELOCITY_HI, REG_CURVE_IN_VELOCITY_LO, 0);
//    registers_write_word(REG_CURVE_OUT_VELOCITY_HI, REG_CURVE_OUT_VELOCITY_LO, 0);
    Curve_Position=0;
    Curve_In_Velocity=0;
    Curve_Out_Velocity=0;
    Curve_Delta=0;
    // ���»�����״̬��
//    registers_write_byte(REG_CURVE_RESERVED, 0);
	  Curve_Reserved=0;
//    registers_write_byte(REG_CURVE_BUFFER, motion_buffer_left());
}

u8 next;
u8 motion_append(void)
// Append a new curve keypoint from data stored in the curve registers.  The keypoint
// is offset from the previous curve by the specified delta.  An error is returned if
// there is no more room to store the new keypoint in the buffer or if the delta is
// less than one (a zero delta is not allowed).
//�Ӵ洢�ڼĴ��������������һ���µ����߹ؼ��㡣�ؼ�����ָ����������ǰ�������ƫ�ơ�������ش�����
//û�и���Ŀռ����洢�µĹؼ����ڻ������������С��1����������������
{
    int16 position;
    int16 in_velocity;
    int16 out_velocity;
//    u8 next;
    u16 delta;

    // Get the next index in the buffer.
    next = (motion_head + 1) & MOTION_BUFFER_MASK;//�ڻ������л�ȡ��һ������

    // Return error if we have looped the head to the tail and the buffer is filled.
    if (next == motion_tail) return 0;//������ǵ�ͷѭ����β�������������������򷵻ش���

    // Get the position, velocity and time delta values from the registers.
//    position = (int16) registers_read_word(REG_CURVE_POSITION_HI, REG_CURVE_POSITION_LO);
	  
//    in_velocity = (int16) registers_read_word(REG_CURVE_IN_VELOCITY_HI, REG_CURVE_IN_VELOCITY_LO);
//    out_velocity = (int16) registers_read_word(REG_CURVE_OUT_VELOCITY_HI, REG_CURVE_OUT_VELOCITY_LO);
//    delta = (u16) registers_read_word(REG_CURVE_DELTA_HI, REG_CURVE_DELTA_LO);
	  position =Curve_Position;
	  in_velocity = (int16)Curve_In_Velocity;
    out_velocity = (int16)Curve_Out_Velocity;
    delta = Curve_Delta;
    // Keypoint delta must be greater than zero.
    if (delta < 1) return 0;

    // Fill in the next keypoint.
    keys[next].delta = delta;
    keys[next].position = int_to_float(position);
    keys[next].in_velocity = fixed_to_float(in_velocity);
    keys[next].out_velocity = fixed_to_float(out_velocity);

    // Is this keypoint being added to an empty buffer?
//		�ؼ��㱻��ӵ�һ���ջ�������
    if (motion_tail == motion_head)
    {
        // Initialize a new hermite curve that gets us from the current position to the new position.
        // We use a velocity of zero at each end to smoothly transition from one to the other.
//			 ��ʼ��һ���µĶ������ߣ�ʹ���Ǵӵ�ǰλ�õ���λ�á�
//       ������ÿһ�˵����ٶ�ƽ�ȵش�һ�����ɵ���һ��
        curve_init(0, delta, curve_get_p1(), keys[next].position, 0.0, 0.0);
    }

    // Increase the duration of the buffer.���ӻ������ĳ���ʱ��
    motion_duration += delta;

    // Set the new head index.�����µ�ͷ������
    motion_head = next;

    // Reset the motion registers and update the buffer status.
   //		�����˶��Ĵ��������»�����״̬��
    motion_registers_reset();

    return 1;
}

    float fposition;//λ��
    float fvelocity;//�ٶ�
void motion_next(u16 delta)
// Increment the buffer counter by the indicated delta and return the position
// and velocity from the buffered curves.  If the delta is zero the current
// position and velocity is returned.
//ͨ��ָ���������������������������λ�á�
//�ͻ������ߵ��ٶȡ�����������������
//����λ�ú��ٶȡ�
{


    // ȷ���ڼĴ������Ƿ���������˶�
//    if (!(registers_read_byte(REG_FLAGS_LO) & (1<<FLAGS_LO_MOTION_ENABLED))) return;
    // ���Ǵ�����ǿ�������
    if (motion_tail == motion_head)
    {
        // Yes. ���ּ������ͳ���ʱ��Ϊ0��
        motion_counter = 0;
        motion_duration = 0;
    }
    else
    {
        // Increment the counter.���Ӽ�����
        motion_counter += delta;

        // Have we exceeded the duration of the currently buffered curve?
        while (motion_counter > curve_get_duration())
        {
            // Reduce the buffer counter by the currently buffered curve duration.
//					ͨ����ǰ��������߳���ʱ�������ٻ��������
            motion_counter -= curve_get_duration();

            // Reduce the buffer duration by the currently buffered curve duration.
//					   ͨ����ǰ��������߳���ʱ�������ٻ���������ʱ��
            motion_duration -= curve_get_duration();

            // Increment the tail to process the next buffered curve.
//					����β����������һ���������ߡ�motion_tail++;
            motion_tail = (motion_tail + 1) & MOTION_BUFFER_MASK;

            // Has the tail caught up with the head?
            if (motion_tail == motion_head)
            {
                // Initialize an empty hermite curve with a zero duration.  This is a degenerate case for
                // the hermite cuve that will always return the position of the curve without velocity.
//							��ʼ��һ����ʱ��Ŀհ����������ߡ�����Hermite�����������ߵ�λ��û���ٶȷ���һ���˻��������
                curve_init(0, 0, keys[motion_head].position, keys[motion_head].position, 0.0, 0.0);
                // Reset the buffer counter and duration to zero.
                motion_counter = 0;
                motion_duration = 0;
            }
            else
            {
                u8 curr_point;
                u8 next_point;
							//��ȡ���ߵĵ�ǰ�����һ��
                curr_point = motion_tail;
                next_point = (curr_point + 1) & MOTION_BUFFER_MASK;

                // �ӵ�ǰ����һ�����ʼ���������ߡ�
                curve_init(0,                             keys[next_point].delta,
                           keys[curr_point].position,     keys[next_point].position,
                           keys[curr_point].out_velocity, keys[next_point].in_velocity);
            }

            // Update the space available in the buffer.
//            registers_write_byte(REG_CURVE_BUFFER, motion_buffer_left());
        }
				
    }


		//��Hermite���ߵõ�λ�ú��ٶ�
    curve_solve(motion_counter, &fposition, &fvelocity);

    // The velocity is in position units a millisecond, but we really need the
    // velocity to be measured in position units every 10 milliseconds to match
    // the sample period of the ADC.
    fvelocity *=10.0;
   if(fvelocity<0)
	 { fvelocity=-fvelocity;}
    // Update the seek position register.
//    registers_write_word(REG_SEEK_POSITION_HI, REG_SEEK_POSITION_LO, );

//    // Update the seek velocity register.
//    registers_write_word(REG_SEEK_VELOCITY_HI, REG_SEEK_VELOCITY_LO, float_to_int(fvelocity));
		
	  seek_position=float_to_int(fposition);//Ŀ��λ��
    seek_velocity=float_to_int(fvelocity);//Ѱ���ٶ�
	 
}


u8 motion_buffer_left(void)
// The motion buffer can contain up to MOTION_BUFFER_SIZE keypoints.  The function
// returns how many keypoints remain free in the buffer for use.
//�˶��Ļ��������԰������motion_buffer_sizeҪ�㡣�������ض���Ҫ�㱣������ʹ�õĻ�����
{
    u8 space_left;

    // Determine the points left to store curve data.
   //	ȷ�������������ݵĵ�
    if (motion_head < motion_tail)
    {
        space_left = (MOTION_BUFFER_SIZE - 1) - (MOTION_BUFFER_SIZE + motion_head - motion_tail);
    }
    else
    {
        space_left = (MOTION_BUFFER_SIZE - 1) - (motion_head - motion_tail);
    }

    return space_left;
}


#endif // CURVE_MOTION_ENABLED

