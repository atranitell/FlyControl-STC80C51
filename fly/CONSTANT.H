#ifndef _CONSTATN_H
#define _CONSTATN_H

// 主频
#define _MAIN_FREN		12 		// 主频 MHz
#define _TIME0_PEROID	20000	// time0 计时器时间 us
#define _TIME0_VALUE  	(65536-((int)(_TIME0_PEROID/12)*_MAIN_FREN))	// 计数初值

#define _EF_PROM	0xC800 // 0XC800

/***********需要自己修改的部分***********************/
#define _PWM_S_MAX		1000
#define _PWM_S_MIN		0
#define _YM_S_MAX		255
#define _YM_S_MIN		0
#define _PID_S_MUL		(_PWM_S_MAX/1000)

#define _PWM_B_POD	  	24000	// 最大计数周期 
#define _PWM_B_MAX 		12100   // 实际最大值 80%
#define _PWM_B_MIN		0		// 实际最小值 40%
#define _YM_B_MAX		255		// 有效控制YM最大值
#define _YM_B_MIN		100		// 有效控制YM最小值
#define _PID_B_MUL		(_PWM_B_MAX/1000)

/****************** 卡尔曼滤波参数 ******************/
// 定义高斯白噪声
#define _KM_Q_X		0.001
#define _KM_R_X		0.001
#define _KM_Q_Y		0.001
#define _KM_R_Y		0.001

/****************************************************/

// 传输模块
#define _TX_LENGTH	20
#define _RX_LENGTH  20
#define _RETR_NUM 	3

// PWM 模块
// 小四轴
#define _PWM_S_1H	0x03
#define _PWM_S_1L	0xe8
#define _PWM_S_2H	0x03
#define _PWM_S_2L	0xe9

// 大四轴
#define _PWM_PEROID		2									// 2ms
#define _PWM_HZ			(int)(1000/_PWM_PEROID) 			// PWM 频率
#define _PWM_B_PWM_MAX	(_PWM_B_POD)						// PWM最大值

#define _PWM_B_1H	(_PWM_B_PWM_MAX>>8 & 0x00FF)
#define _PWM_B_1L	(_PWM_B_PWM_MAX & 0x00FF)
#define _PWM_B_2H	(_PWM_B_PWM_MAX>>8 & 0x00FF)
#define _PWM_B_2L	(_PWM_B_PWM_MAX & 0x00FF + 1)

// 大四轴 PID
#define Big_Out_XP 12
#define Big_Out_XI 0.005f

#define Big_In_XP 0.3f
#define Big_In_XI 0.01f
#define Big_In_XD 2.85f

#define Big_Out_YP Big_Out_XP
#define Big_Out_YI Big_Out_XI

#define Big_In_YP Big_In_XP
#define Big_In_YI Big_In_XI
#define Big_In_YD Big_In_XD

#define Big_ZP 5.0f
#define Big_ZD 4.0f

#define	ERR_MAX	500

// 小四轴参数 PID
#define Small_Out_XP 35.0f
#define Small_Out_XI 0.005f

#define Small_In_XP 0.3f
#define Small_In_XI 0.06f
#define Small_In_XD 3.75f

#define Small_Out_YP Small_Out_XP
#define Small_Out_YI Small_Out_XI

#define Small_In_YP Small_In_XP
#define Small_In_YI Small_In_XI
#define Small_In_YD Small_In_XD

#define Small_ZP 5.0f
#define Small_ZD 4.0f

// 串口波特率
#define _UART_BAUD 	115200
#define _UART_T1	(int)(65536-_MAIN_FREN*250000/_UART_BAUD)	// 计数初值

// IMU 模块
#define	_IMU_PI		3.14159265f
#define	_IMU_Kp		0.8f
#define	_IMU_Ki		0.001f
#define	_IMU_Half_T	0.01f //(int)(_TIME0_PEROID/2000000) // time0计数器的一半

// 陀螺仪模块
#define	SMPLRT_DIV		0x19    // 0x19	// 陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	// 0x1A // 低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B    // 0x1B	// 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	// 加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	// 电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	// IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	// IIC写入时的地址字节数据，+1为读取
#define IICSPEED        0x24

#endif