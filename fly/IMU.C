
#include <math.H>

#include "STC15W4K60S4.H"
#include "CONSTANT.H"
#include "IMU.H"

float idata q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float idata exInt = 0, eyInt = 0, ezInt = 0;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float idata norm;
	float idata vx, vy, vz;
	float idata ex, ey, ez;

	norm = sqrt(ax * ax + ay * ay + az * az);
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	vx = 2 * (q1 * q3 - q0 * q2);
	vy = 2 * (q0 * q1 + q2 * q3);
	vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3 ;

	ex = (ay * vz - az * vy) ;
	ey = (az * vx - ax * vz) ;
	ez = (ax * vy - ay * vx) ;

	exInt = exInt + ex * _IMU_Ki;
	eyInt = eyInt + ey * _IMU_Ki;
	ezInt = ezInt + ez * _IMU_Ki;

	gx = gx + _IMU_Kp * ex + exInt;
	gy = gy + _IMU_Kp * ey + eyInt;
	gz = gz + _IMU_Kp * ez + ezInt;

	q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * _IMU_Half_T;
	q1 = q1 + ( q0 * gx + q2 * gz - q3 * gy) * _IMU_Half_T;
	q2 = q2 + ( q0 * gy - q1 * gz + q3 * gx) * _IMU_Half_T;
	q3 = q3 + ( q0 * gz + q1 * gy - q2 * gx) * _IMU_Half_T;

	norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	Angle  = asin(2 * (q0 * q2 - q1 * q3 )) * 57.2957795f;
	Angley = asin(2 * (q0 * q1 + q2 * q3 )) * 57.2957795f;
}

