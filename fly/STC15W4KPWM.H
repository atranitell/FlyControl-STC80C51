#ifndef _STC15W4K60S4_H_
#define _STC15W4K60S4_H_

void PWMGO(u8 _mode_ctr);
void PWM(unsigned int PWMa,unsigned int PWMb,unsigned int PWMc,unsigned int PWMd);
void initEachPWM( int PWM_1H, int PWM_1L, int PWM_2H, int PWM_2L);

#endif  