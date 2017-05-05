#ifndef		__EPWM_H
#define		__EPWM_H
#include "config.h"
void epwmnTcolck(u8 nTclock);
void epwmTwide(u16 Twide);
void epwmStartADC(u8 StartADC_EN);
void epwmFaultSource(u8 FaultSource,u8 FaultCloseIO);
void epwmFirstSignalLevel(u8 pin,u8 value);
void epwmSetValue(u8 pin,u16 PWMT1,u16 PWMT2);
void epwmCRbegin();
#endif