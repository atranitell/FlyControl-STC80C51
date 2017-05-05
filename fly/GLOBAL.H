#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "CONSTANT.H"

/********************************************************
 * 飞行器模式选择
 */
u8	_mode_ctr = 0 ;
u8	_if_finish_cfg_pwm = 0 ;
u8 	xdata R[3] = {0} ;

/********************************************************
 * 遥控器数据
 */
u8	xdata	TxBuf[_TX_LENGTH] = {0};
u8	xdata	RxBuf[_RX_LENGTH] = {0};

u8	xdata	tp[16];

uint t_while = 0;
uint t_tx = _RETR_NUM;
uint t_rx = _RETR_NUM;

int  xdata count_acc = 0 ;

/********************************************************
 * 高度控制
 */
unsigned int Height ;

/********************************************************
 * 油门相关
 */
// 指实际最大最小油门，以及对应的最大最小PWM输出
int pwm_max ;
int pwm_min ;
int ym_min ;
int ym_max ;
int ym_mul ; // 油门倍率
int pwm_max_pod ;
float pid_mul ;	// speed的乘子

// 油门变化速度控制
u8	YM = 0;

// 电机速度参数
int	data	speed0 = 0;
int	data	speed1 = 0;
int	data	speed2 = 0;
int	data	speed3 = 0;
//加载至PWM模块的参数
int	data	PWM0 = 0;
int	data	PWM1 = 0;
int	data	PWM2 = 0;
int	data	PWM3 = 0;
//陀螺仪矫正参数
float	xdata	g_x = 0;
float	xdata	g_y = 0;
float	xdata	g_z = 0;
//角度矫正参数
float	xdata	a_x;
float	xdata	a_y;

/********************************************************
 * 角度参数
 */

// 由加速度计算的加速度(弧度制)
float	xdata	Angle_ax = 0;
float	xdata	Angle_ay = 0;
float	xdata	Angle_az = 0;

// 由角速度计算的角速率(角度制)
float	idata	Angle_gy = 0;
float	idata	Angle_gx = 0;
float	idata	Angle_gz = 0;

// 加速度计静差参数
float	idata	err_ax ;
float	idata	err_ay ;
float 	idata	err_az ;
float 	idata	err_gx ;
float 	idata	err_gy ;
float 	idata	err_gz ;

// 加速度计卡尔曼滤波
float 	xdata	_km_bs_x = 0;		// 滤波后的值
float 	xdata 	_km_pnow_x = 0;	    // 本次测量值
float 	xdata	_km_bs_y = 0;		// 滤波后的值
float 	xdata 	_km_pnow_y = 0;	    // 本次测量值

// 三角函数解算出的欧拉角
// float idata AngleAx=0,AngleAy=0;

// 四元数解算出的欧拉角
float	data	Angle = 0;
float	data	Angley = 0;

// Z轴相关
float	xdata	Anglezlate = 0;

// 加入遥控器控制量后的角度
float	xdata	Ax = 0;
float	xdata	Ay = 0;

u8	data	lastR0 = 0;
// 上一次RxBuf[0]数据(RxBuf[0]数据在不断变动的)
u8	data	ZT = 0; // 状态标识

u8	SW2_tmp;

/********************************************************
 * 姿态处理和PID
 */
// PID最终输出量
float	data PID_Output;

// 外环PI输出量  上一次陀螺仪数据
float	idata	Last_Angle_gx = 0;
// 外环P  外环I  外环误差积分
float	idata	ERRORX_Out = 0;
// 内环P  内环I   内环D  内环误差积分
float	idata	ERRORX_In = 0;

float	idata	Last_Angle_gy = 0;
float	idata	ERRORY_Out = 0;
float	idata	ERRORY_In = 0;

float	idata	ERRORZ_Out = 0;

/********************************************************
 * 内环PID, 外环PI
 */
float 	Out_XP = 0;
float 	Out_XI = 0;

float 	In_XP = 0;
float 	In_XI = 0;
float 	In_XD = 0;

float 	In_YP = 0;
float 	In_YI = 0;
float 	In_YD = 0;

float 	Out_YP = 0;
float 	Out_YI = 0;

float 	ZP = 0;
float 	ZD = 0;

#endif
