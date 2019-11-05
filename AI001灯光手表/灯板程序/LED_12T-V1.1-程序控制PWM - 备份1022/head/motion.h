

#ifndef _OS_MOTION_H_
#define _OS_MOTION_H_ 1

// Buffer size must be a power of two.
//�������Ĵ�С������2����
#define MOTION_BUFFER_SIZE       8
#define MOTION_BUFFER_MASK       (MOTION_BUFFER_SIZE - 1)

// �������
extern u8 motion_head;
extern u8 motion_tail;
extern u32 motion_counter;
extern u32 motion_duration;

// Motion functions.
void motion_init(void);
void motion_reset(int16 position);
void motion_registers_reset(void);
u8 motion_append(void);
void motion_next(u16 delta);
u8 motion_buffer_left(void);

// Motion inline functions.


 static u32 motion_time_left(void)
// Get the remaining time of the buffered curves.
// ��ȡ�������ߵ�ʣ��ʱ�䡣
{
    // The time left is the buffer duration minus the buffer counter.
//	ʣ�µ�ʱ���ǻ�����ʱ���ȥ��������������
    return motion_duration - motion_counter;
}
typedef struct motion_key
{
    u16 delta;
    float position;
    float in_velocity;
    float out_velocity;
} motion_key;
extern motion_key keys[MOTION_BUFFER_SIZE];
#endif // _OS_MOTION_H_

