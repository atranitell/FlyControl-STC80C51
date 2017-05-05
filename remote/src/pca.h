
#ifndef	__PCA_H
#define	__PCA_H
#include	"config.h"


#define	PCA_Clock_1T	(4<<1)
#define	PCA_Clock_2T	(1<<1)
#define	PCA_Clock_4T	(5<<1)
#define	PCA_Clock_6T	(6<<1)
#define	PCA_Clock_8T	(7<<1)
#define	PCA_Clock_12T	(0<<1)
#define	PCA0			0
#define	PCA1			1
#define	PCA2			2
#define	PCA_Counter		3

#define	PCA_P12_P11_P10_P37	(0<<4)
#define	PCA_P34_P35_P36_P37	(1<<4)
#define	PCA_P24_P25_P26_P27	(2<<4)
#define	PCA_Mode_PWM				0x2	//B0100_0010
#define	PCA_Mode_Capture			0
#define	PCA_Mode_SoftTimer			0x8	//B0100_1000
#define	PCA_Mode_HighPulseOutput	0xc	//B0100_1100

#define	PCA_Clock_Timer0_OF	(2<<1)
#define	PCA_Clock_ECI	(3<<1)
#define	PCA_Rise_Active	(1<<5)
#define	PCA_Fall_Active	(1<<4)
#define	PCA_PWM_8bit	(0<<6)
#define	PCA_PWM_7bit	(1<<6)
#define	PCA_PWM_6bit	(2<<6)
 
extern	 volatile u8 pin_buff0;
extern	 volatile u8 pin_buff1;
extern	 volatile u8 pin_buff2;
extern	 volatile u16 PCA_Timer0;
extern	 volatile u16 PCA_Timer1;
extern	 volatile u16 PCA_Timer2;
extern	 volatile u16 PWM0_low;
extern	 volatile u16 PWM1_low;
extern	 volatile u16 PWM2_low;
void	UpdatePwm(u8 PCA_id, u8 pwm_value);
void  analogWrite(pin,u8 value);
void  Capturebegin(pin,pulsemode);
u8    Captureavailable();
u32   GetCapture();
void  CaptureEnd();
#endif

