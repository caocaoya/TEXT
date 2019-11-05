
#include "ALL_Includes.h"
#if CURVE_MOTION_ENABLED

// Hermite timing parameters.�����ض�ʱ����
 u16 curve_t0;
 u16 curve_t1;
 u16 curve_duration;
static float curve_duration_float;
 u16 Curve_Delta=0;//�����˶�ʱ��
 u16 Curve_Position=0;//�����˶�λ��
 u16 Curve_In_Velocity=0;//�����˶���ʼ�ٶ�
 u16 Curve_Out_Velocity=0;//�����˶������ٶ�
 
 u16 Curve_Reserved=0;
// Hermite curve parameters.�����������߲���
float curve_p0;
float curve_p1;
float curve_v0;
float curve_v1;

//Hermite curve cubic polynomial coefficients.�������ζ���ʽϵ��
static float curve_a;
static float curve_b;
static float curve_c;
static float curve_d;

void curve_init(u16 t0, u16 t1, float p0, float p1, float v0, float v1)
{
    // Set the time parameters.����ʱ�������
    curve_t0 = t0;
    curve_t1 = t1;
    curve_duration = t1 - t0;
    curve_duration_float = (float) curve_duration;

    // The tangents are expressed as slope of value/time.  The time span will
    // be normalized to 0.0 to 1.0 range so correct the tangents by scaling
    // them by the duration of the curve.
//	б�ʱ�ʾΪֵ/ʱ�䡣ʱ���Ƚ����鵽0��1�ķ�Χ�ڣ���ȷ������ͨ���������ߵĳ���ʱ�䡣
    v0 *= curve_duration_float;
    v1 *= curve_duration_float;

    // Set the curve parameters.
    curve_p0 = p0;
    curve_p1 = p1;
    curve_v0 = v0;
    curve_v1 = v1;

    // Set the cubic coefficients by multiplying the matrix form of
    // the Hermite curve by the curve parameters p0, p1, v0 and v1.
    //
    // | a |   |  2  -2   1   1 |   |       p0       |
    // | b |   | -3   3  -2  -1 |   |       p1       |
    // | c | = |  0   0   1   0 | . | (t1 - t0) * v0 |
    // | d |   |  1   0   0   0 |   | (t1 - t0) * v1 |
    //
    // a = 2p0 - 2p1 + v0 + v1
    // b = -3p0 + 3p1 -2v0 - v1
    // c = v0
    // d = p0

    curve_a = (2.0 * p0) - (2.0 * p1) + v0 + v1;
    curve_b = -(3.0 * p0) + (3.0 * p1) - (2.0 * v0) - v1;
    curve_c = v0;
    curve_d = p0;
}


void curve_solve(u16 t, float *x, float *dx)
{
    // Handle cases where t is outside and indise the curve.
    if (t <= curve_t0)
    {
        // Set x and in and out dx.
        *x = curve_p0;
        *dx = t < curve_t0 ? 0.0 : curve_v0;
    }
    else if (t >= curve_t1)
    {
        // Set x and in and out dx.
        *x = curve_p1;
        *dx = t > curve_t1 ? 0.0 : curve_v1;
    }
    else
    {
        // Subtract out the t0 value from t.
        float t1 = ((float) (t - curve_t0)) / curve_duration_float;
        float t2 = t1 * t1;
        float t3 = t2 * t1;

        // Determine the cubic polynomial.���ζ���ʽ��ȷ����
        // x = at^3 + bt^2 + ct + d
        *x = (curve_a * t3) + (curve_b * t2) + (curve_c * t1) + curve_d;

        // Determine the cubic polynomial derivative.�����ζ���ʽ����
        // dx = 3at^2 + 2bt + c
        *dx = (3.0 * curve_a * t2) + (2.0 * curve_b * t1) + curve_c;

        // The time span has been normalized to 0.0 to 1.0 range so correct
        // the derivative to the duration of the curve.
//			   ʱ���ȱ���׼����0��1��Χ�������������������ߵĳ���ʱ�䡣
        *dx /= curve_duration_float;
    }
}

#endif // CURVE_MOTION_ENABLED

