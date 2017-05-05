
/*********************************************************************
 *	四轴复合版，大四轴 & 小四轴
 *  时间：2015-8-19
 *  作者: JIN Kai & JIANGH He-jun
 *  算法：PID(内环PID, 外环PI) + 四元数解算 + 互补滤波

 // 程序架构方面
 *  1. 实现一键切换大小四轴
 *	2. 实现将需要修改的参数定位到同一个文件
 *  3. 实现参数关联修改(修改主频的同时将所有参数自动修改)
 
 // PWM电机驱动
 *	8. 动态大四轴PWM调节(自动进入正常模式)
 *	17. 动态小四轴PWM初始化计数初值
 
 // 飞机保护程序
 *  4. 实现飞行器失联保护
 *  5. 实现飞行器一键刹车

 // 通讯程序
 *  6. 实现无线通信的伪双工通讯(兼容单工)
 *  7. 实现uart串口的读写
 *	15. 定时器t0初始化过程消除uart串口中断

 // 飞控算法调整
 *	14. 对Speed的取值进行估计
 *  18. 重新调整了大小四轴的PID
 *  22. 将飞行器飞行方式由十字型转换为X型

 // 飞机调试模块
 *	13. 增加matlab对MPU6050的曲线实时显示

 // 遥控器模块
 *	12. 对遥控器程序进行了重构
 *	16. 解决遥控器控制不灵敏的问题(非线性函数)
 *  19. 遥控器部分实现了方向微调
 *	21. 实现LCD显示飞行器数据(PWM,MPU6050)
 *  25. 增加LCD大小四轴遥控器方式选择启动(Important)
 *  26. 增加LCD对油门杆误拉高启动的提醒

 // 辅助MCU模块(超声波程序, 灯光等)
 *  20. 增加超声波测高传感器
 *  24. 对超声波测距数据增加卡尔曼滤波

 // 陀螺仪校准方面
 *	9. 对陀螺仪进行校准
 *  23. 对加速度计进行了初始化过程的自校准
 *  24. 增加了卡尔曼滤波函数(针对加速度计)
 
 // 待实现
 *	2. 实现另一个MPU的传感器GPS/温度/灯光等
 *	7. 实现定高算法
 *********************************************************************/

#include <intrins.h>
#include <math.h>

#include "STC15W4K60S4.H"
#include "NRF24L01.H"
#include "MPU6050.H"
#include "STC15W4KPWM.H"
#include "Timer.h"
#include "EEPROM.h"
#include "USART.h"
#include "IMU.H"

#include "GLOBAL.H"
#include "CONSTANT.H"

#include <string.h>
#include <stdio.h>

// 姿态调整
void Angle_Calculate();
double KMfilter( int flag ) ;

// I/O部分
void initIOPort();

// 无线通讯部分
void initRemoteCtr();
void clearSendData();
void sendSensorData();
void recvRemoteData();

//  获取数据
void acqACCData();
void acqGYOData();
void acqPWMData();

// PWM相关
void initAllPWM( int _mode_ctr ) ;
int chooseMode();
void initFlyMode( int mode );
void ConfigPWM( int mode );
int changePWM(int MAX, int MIN, int CUR);
void selfStartForBig();

// 安全措施
void STOPonClick();
void lostProtect();

void main() {
	initAllPWM(0) ;

	initRemoteCtr();		// 初始化无线通讯
	_mode_ctr = chooseMode();			// 选择不同的四轴模式
	Usart_Init();			// 初始化串口

	initAllPWM(_mode_ctr) ;
	
	InitMPU6050();			// 初始化MPU-6050
	// IAPRead();			// 读取陀螺仪静差
	Time0_Init();			// 初始化定时器

	while (1) {

		recvRemoteData();
		Delay(10);

		// 右键 -- 刹车
		if ( RxBuf[5] == 1 ) {
			RxBuf[5] = 0;
			STOPonClick();
		}

		// 左键 -- 校准陀螺仪
		if ( RxBuf[6] == 1 ) {
			RxBuf[6] = 0;
			if (RxBuf[4] < 5) {
				IAP_Gyro();
				EA = 0;
				T2L = 0xE4;
				T2H = 0xFF;
				PWMCKS = 0x00;
				TR0 = 1;
				EA = 1;
			}
		}

		sendSensorData();
		clearSendData();
	}
}

/************************ Timer0 中断函数 大约 6ms ************************/
void Angle_Calculate(void) interrupt 1 		//8ms@28MHz
{
	int adj_flag = RxBuf[7] ;

	YM = RxBuf[4];	// 油门

	lostProtect() ; // 失联保护

	Read_MPU6050(tp); // 获取陀螺仪数据

	Angle_ax = ((float)(((int *)&tp)[0])) / 8192.0;	//加速度处理	结果单位是 +- g
	Angle_ay = ((float)(((int *)&tp)[1])) / 8192.0;	//转换关系	8192 LSB/g, 1g对应读数8192
	Angle_az = ((float)(((int *)&tp)[2])) / 8192.0;	//加速度量程 +-4g/S

    Angle_ax -= 0.07 ;
    Angle_az += 0.13 ; // 补偿

	Last_Angle_gx = Angle_gx;		//储存上一次角速度数据
	Last_Angle_gy = Angle_gy;
	Anglezlate = Angle_gz;

	Angle_gx = ((float)(((int *)&tp)[4])) / 65.5;	//陀螺仪处理	结果单位是 +-度
	Angle_gy = ((float)(((int *)&tp)[5])) / 65.5;	//陀螺仪量程 +-500度/S, 1度/秒 对应读数 65.536
	Angle_gz = ((float)(((int *)&tp)[6])) / 65.5;	//转换关系65.5 LSB/度

	if ( count_acc < 500 ) {
		Angle_gx = (Last_Angle_gx + Angle_gx)/2 ;
		Angle_gy = (Last_Angle_gy + Angle_gy)/2 ;
		Angle_gz = (Anglezlate + Angle_gz)/2 ;
		count_acc++ ;
		return ;
	}

	if ( count_acc == 500 ) {
		err_ax = Angle_gx ;
		err_ay = Angle_gy ;
		err_az = Angle_gz ;
		count_acc++ ;
	}
	
	Angle_gx -= err_ax ;
	Angle_gy -= err_ay ;
	Angle_gz -= err_az ;

	// Angle_gx = KMfilter(1) ;
	// Angle_gy = KMfilter(2) ;

//*********************************** 四元数解算 ***********************************
	IMUupdate(Angle_gx * 0.0174533f, Angle_gy * 0.0174533f, Angle_gz * 0.0174533f, Angle_ax, Angle_ay, Angle_az);
	//0.174533为PI/180 目的是将角度转弧度	四元数计算出 俯仰(X轴): Angle, 横滚(Y轴): Angley

//******************** 三角函数直接解算以供比较四元数解算精准度 ********************
//	AngleAx = atan(Angle_ax/sqrt(Angle_ay*Angle_ay+Angle_az*Angle_az)) * 57.2957795f; //后面的数字是180/PI 目的是弧度转角度
//	AngleAy = atan(Angle_ay/sqrt(Angle_ax*Angle_ax+Angle_az*Angle_az)) * 57.2957795f;
	Angle += a_x ;
	Angley += a_y ;
	Angle_gx += g_x ;
	Angle_gy += g_y ;

//***********************************************
	if ( adj_flag == 3 ) a_x += 0.1 ;
	else RxBuf[7] = 0 ;

	if ( adj_flag == 4 ) a_x -= 0.1 ;
	else RxBuf[7] = 0 ;

	if ( adj_flag == 5 ) a_y += 0.1 ;
	else RxBuf[7] = 0 ;

	if ( adj_flag == 6 ) a_y -= 0.1 ;
	else RxBuf[7] = 0 ;

	if ( adj_flag == 7 ) g_x += 0.002 ;
	else RxBuf[7] = 0 ;

	if ( adj_flag == 8 ) g_x -= 0.002 ;
	else RxBuf[7] = 0 ;

	if ( adj_flag == 9 ) g_y -= 0.002 ;
	else RxBuf[7] = 0 ;

	if ( adj_flag == 10 ) g_y += 0.002 ;
	else RxBuf[7] = 0 ;

//*******************X型油门转换*****************
	R[1] = RxBuf[1] - (RxBuf[1] - 128)/2;
	R[2] = RxBuf[2] + (RxBuf[1] - 128)/2;
	R[2] = R[2] - (RxBuf[2] - 128)/2;
	R[1] = R[1] - (RxBuf[2] - 128)/2;

//************** X轴指向 ***********************************************************
	Ax = Angle - (((float)R[1] - 128) / 7) - a_x;	//角度控制量加载至角度,  Angle: 当前角度, RxBuf[1]: 给定角度, a_x: 0点角度

	if (YM > 20)	ERRORX_Out += Ax;	//外环积分(油门小于某个值时不积分)
	else		ERRORX_Out = 0;		//油门小于定值时清除积分值

	if (ERRORX_Out >  ERR_MAX)	ERRORX_Out =  ERR_MAX;	//积分限幅
	else if (ERRORX_Out < -ERR_MAX)	ERRORX_Out = -ERR_MAX;	//积分限幅

	PID_Output = Ax * Out_XP + ERRORX_Out * Out_XI;	//外环PI

	if (YM > 20)	ERRORX_In += (Angle_gy - PID_Output);	//内环积分(油门小于某个值时不积分)
	else		ERRORX_In  = 0; //油门小于定值时清除积分值

	if (ERRORX_In >  ERR_MAX)	ERRORX_In =  ERR_MAX;
	else if (ERRORX_In < -ERR_MAX)	ERRORX_In = -ERR_MAX;	//积分限幅

	PID_Output = (Angle_gy + PID_Output) * In_XP + ERRORX_In * In_XI + (Angle_gy - Last_Angle_gy) * In_XD;	//内环PID

	if (PID_Output >  1000)	PID_Output =  1000; //输出量限幅
	if (PID_Output < -1000)	PID_Output = -1000;

	speed0 = 0 - PID_Output, speed2 = 0 + PID_Output;

//**************Y轴指向**************************************************
	Ay  = Angley + ((float)R[2] - 128) / 7 - a_y;			//角度控制量加载至角度

	if (YM > 20)		ERRORY_Out += Ay;				//外环积分(油门小于某个值时不积分)
	else			ERRORY_Out = 0;					//油门小于定值时清除积分值
	if (ERRORY_Out >  ERR_MAX)	ERRORY_Out =  ERR_MAX;
	else if (ERRORY_Out < -ERR_MAX)	ERRORY_Out = -ERR_MAX;	//积分限幅

	PID_Output = Ay * Out_YP + ERRORY_Out * Out_YI;	//外环PI

	if (YM > 20)		ERRORY_In += (Angle_gx - PID_Output);	//内环积分(油门小于某个值时不积分)
	else			ERRORY_In = 0;							//油门小于定值时清除积分值
	if (ERRORY_In >  ERR_MAX)	ERRORY_In =  ERR_MAX;
	else if (ERRORY_In < -ERR_MAX)	ERRORY_In = -ERR_MAX;	//积分限幅

	PID_Output = (Angle_gx + PID_Output) * In_YP + ERRORY_In * In_YI + (Angle_gx - Last_Angle_gx) * In_YD;	//内环PID

	if (PID_Output >  1000)	PID_Output =  1000; //输出量限幅
	if (PID_Output < -1000)	PID_Output = -1000;

	speed3 = 0 + PID_Output, speed1 = 0 - PID_Output;		//加载到速度参数

//************** Z轴指向(Z轴随便啦，自旋控制没必要上串级PID) *****************************
	Angle_gz = Angle_gz - ((float)RxBuf[3] - 128) * 1.0f;	//操作量

	if (YM > 20)		ERRORZ_Out += Angle_gz;
	else			ERRORZ_Out  = 0;
	if (ERRORZ_Out >  500)	ERRORZ_Out =  500;
	else if (ERRORZ_Out < -500)	ERRORZ_Out = -500;	//积分限幅
	PID_Output = (Angle_gz) * ZP + ERRORZ_Out * 0.2f + (Angle_gz - Anglezlate) * ZD;

	speed0 = speed0 + PID_Output, speed2 = speed2 + PID_Output;
	speed1 = speed1 - PID_Output, speed3 = speed3 - PID_Output;

//**************将速度参数加载至PWM模块*************************************************
	if (YM < 10) {
		PWM0 = pwm_max ;
		PWM1 = pwm_max ;
		PWM2 = pwm_max ;
		PWM3 = pwm_max ;
	}
	else {
		YM = changePWM(ym_max, ym_min, YM) ;
		// F
		PWM0 = pwm_max_pod - (int)YM * ym_mul + (int)(speed0*pid_mul);
		if (PWM0 > pwm_max)	PWM0 = pwm_max;
		else if (PWM0 < pwm_min)	PWM0 = pwm_min;
		// R
		PWM1 = pwm_max_pod - (int)YM * ym_mul + (int)(speed1*pid_mul);
		if (PWM1 > pwm_max)	PWM1 = pwm_max;
		else if (PWM1 < pwm_min )	PWM1 = pwm_min;
 		// B
		PWM2 = pwm_max_pod - (int)YM * ym_mul + (int)(speed2*pid_mul);
		if (PWM2 > pwm_max)	PWM2 = pwm_max;
		else if (PWM2 < pwm_min)	PWM2 = pwm_min;
		// L
		PWM3 = pwm_max_pod - (int)YM * ym_mul + (int)(speed3*pid_mul);
		if (PWM3 > pwm_max)	PWM3 = pwm_max;
		else if (PWM3 < pwm_min)	PWM3 = pwm_min;
	}

	SW2_tmp = P_SW2;
	EAXSFR();

	PWM2T1H = PWM1 >> 8;
	PWM2T1L = PWM1;

	PWM3T1H = PWM2 >> 8;
	PWM3T1L = PWM2;

	PWM4T1H = PWM0 >> 8;
	PWM4T1L = PWM0;

	PWM5T1H = PWM3 >> 8;
	PWM5T1L = PWM3;

	P_SW2 = SW2_tmp;
}

/********************************************************
 * 飞行模式设置
 * 0 - 小四轴
 * 1 - 大四轴
 */

void initFlyMode( int mode ) {
	if ( mode == 0 ) {
		Out_XP = Small_Out_XP;
		Out_XI = Small_Out_XI;
		In_XP = Small_In_XP;
		In_XI = Small_In_XI;
		In_XD = Small_In_XD;

		Out_YP = Small_Out_YP;
		Out_YI = Small_Out_YI;
		In_YP = Small_In_YP;
		In_YI = Small_In_YI;
		In_YD = Small_In_YD;

		ZP = Small_ZP;
		ZD = Small_ZD;
	}

	if ( mode == 1 ) {
		Out_XP = Big_Out_XP;
		Out_XI = Big_Out_XI;
		In_XP = Big_In_XP;
		In_XI = Big_In_XI;
		In_XD = Big_In_XD;

		Out_YP = Big_Out_YP;
		Out_YI = Big_Out_YI;
		In_YP = Big_In_YP;
		In_YI = Big_In_YI;
		In_YD = Big_In_YD;

		ZP = Big_ZP;
		ZD = Big_ZD;
	}
}

// 所有I/O口全设为准双向，弱上拉模式
void initIOPort() {
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	P6M0 = 0x00;
	P6M1 = 0x00;
	P7M0 = 0x00;
	P7M1 = 0x00;
}

// 初始化遥控器
void initRemoteCtr() {
	init_NRF24L01();
	RxBuf[0] = 0;
	RxBuf[1] = 128;
	RxBuf[2] = 128;
	RxBuf[3] = 128;
	RxBuf[4] = 0;
	RxBuf[5] = 0;
	RxBuf[6] = 0;
}

void acqPWMData() {
	TxBuf[0] = 0xAA;
	// 电机数据
	TxBuf[1] = PWM0;
	TxBuf[2] = PWM0 >> 8;
	TxBuf[3] = PWM1;
	TxBuf[4] = PWM1 >> 8;
	TxBuf[5] = PWM2;
	TxBuf[6] = PWM2 >> 8;
	TxBuf[7] = PWM3;
	TxBuf[8] = PWM3 >> 8;
	TxBuf[9] = 0xFF;
}

void clearSendData() {
	int i;
	for ( i = 0; i < _TX_LENGTH; i++ ) TxBuf[0] = 0;
}

void acqACCData() {
	int t1,t2,t3;
	// t1 = Angle_ax*100 ;
	// t2 = Angle_ay*100 ;
	t1 = Angle*10 ;
	t2 = Angley*10 ;
	t3 = Angle_az*10 ;
	TxBuf[0] = 0xAB;
	// 电机数据
	TxBuf[1] = t1;
	TxBuf[2] = t1 >> 8;
	TxBuf[3] = t2;
	TxBuf[4] = t2 >> 8;
	TxBuf[5] = t3;
	TxBuf[6] = t3 >> 8;
	TxBuf[7] = Height;
	TxBuf[11] = 0xFF;
}

void acqGYOData() {
	int t1,t2,t3;
	t1 = Angle_gx*100 ;
	t2 = Angle_gy*100 ;
	t3 = Angle_gz*100 ;
	TxBuf[0] = 0xAC;
	// 电机数据
	TxBuf[1] = t1;
	TxBuf[2] = t1 >> 8;
	TxBuf[3] = t2;
	TxBuf[4] = t2 >> 8;
	TxBuf[5] = t3;
	TxBuf[6] = t3 >> 8;
	TxBuf[11] = 0xFF;	
}

int count = 0 ;
void sendSensorData() {
	if ( count == 0 )
		acqPWMData();
	else if ( count == 1 )
		acqACCData() ;
	else if ( count == 2 ) {
		acqGYOData() ;
		count = -1 ;
	}
	t_while = t_tx;
	while (t_while--) {
		nrf_RxTx('t', TxBuf);
		if (getTxDS() == 1) break;
	}
	count++ ;
}

void recvRemoteData() {
	t_while = t_rx;
	while (t_while--) {
		nrf_RxTx('r', RxBuf);
		if (getRxDR() == 1) break;
	}
}

int chooseMode() {
	SetRX_Mode();
	while ( 1 ) {
		PWM(pwm_max,pwm_max,pwm_max,pwm_max) ;
		nRF24L01_RxPacket(RxBuf);
		// 右键 -- 小四轴模式
		if ( RxBuf[7] == 2 ) {
			Delay(500) ;
			RxBuf[7] = 0;
			return _mode_ctr;	
		}
		// 左键 -- 大四轴模式
		if ( RxBuf[7] == 1 ) {
			Delay(500) ;
			RxBuf[7] = 0;
			_mode_ctr = 1;
			return _mode_ctr;
		}
	}
}

void ConfigPWM( int mode ) {
	// 小四轴
	if ( mode == 0 ) {
		pwm_max = _PWM_S_MAX;
		pwm_min = _PWM_S_MIN;
		ym_max = _YM_S_MAX;
		ym_min = _YM_S_MIN;
		pwm_max_pod = _PWM_S_MAX;
		ym_mul = _PWM_S_MAX/255;
		pid_mul = _PID_S_MUL;
	}
	// 大四轴
	if ( mode == 1 ) {
		pwm_max = _PWM_B_MAX;
		pwm_min = _PWM_B_MIN;
		ym_max = _YM_B_MAX;
		ym_min = _YM_B_MIN;
		pwm_max_pod = _PWM_B_POD;
		ym_mul = _PWM_B_POD/255;
		pid_mul = _PID_B_MUL;
	}
}

int changePWM(int MAX, int MIN, int CUR) {
	return (CUR * (MAX - MIN) / 255 + MIN);
}

void STOPonClick() {
	while (1) {
		EA = 0;
		T2L = 0xE4;
		T2H = 0xFF;
		PWM(pwm_max, pwm_max, pwm_max, pwm_max);
		Delay(1000);
		PWMCKS = 0x00;
	}
}

void selfStartForBig() {
	int interval, i;
	EA = 0;
	interval = (pwm_max - pwm_min) / 50;
	for ( i = 1; i < 3; i += interval ) {
		PWM(pwm_max - i, pwm_max - i, pwm_max - i, pwm_max - i);
		Delay(3000);
	}
	EA = 1;
}

void lostProtect() {
	if (RxBuf[0] == lastR0) {
		if ( ++ZT >= 100) {
			ZT = 0;
			RxBuf[1] = 128;
			RxBuf[2] = 128;
			RxBuf[3] = 128;
		    RxBuf[4] = 0 ;
		}
	} else ZT = 0;
	lastR0 = RxBuf[0];
}

void initAllPWM( int _mode_ctr ) {
	initIOPort();			// 初始化IO口
	PWMGO(_mode_ctr);		// 初始化PWM
	initFlyMode(_mode_ctr);	// 初始化飞行模式
	ConfigPWM(_mode_ctr);	// 配置PWM设置
	if ( _mode_ctr ) 		// 大四轴电机初始化
		selfStartForBig();
	PWMCKS = 0x00;
}

double KMfilter( int flag ) {
	double Kg_now ;
	// x 轴
	if ( 1 == flag ) {
		_km_pnow_x = _km_pnow_x + _KM_Q_X ;
		Kg_now = _km_pnow_x / ( _km_pnow_x + _KM_R_X) ;
		_km_bs_x = _km_bs_x + Kg_now * (Angle_gx - _km_bs_x) ;
		_km_pnow_x = _km_pnow_x * (1 - Kg_now) ;
		return _km_bs_x ;
	}
	// y 轴
	if ( 2 == flag ) {
		_km_pnow_y = _km_pnow_y + _KM_Q_Y ;
		Kg_now = _km_pnow_y / ( _km_pnow_y + _KM_R_Y) ;
		_km_bs_y = _km_bs_y + Kg_now * (Angle_gy - _km_bs_y) ;
		_km_pnow_y = _km_pnow_y * (1 - Kg_now) ;
		return _km_bs_y ;
	}
	return 0;
}