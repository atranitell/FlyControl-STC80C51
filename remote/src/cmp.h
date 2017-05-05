#ifndef	__CMP_H
#define	__CMP_H
#include"config.h"
#define CMPEN 0x80
#define CMPIF 0x40
#define PIE   0x20
#define NIE   0x10
#define PIS   0x08
#define NIS   0x04
#define CMPOE 0x02
#define CMPRES  0x01
#define INVCMPO 0x80
#define DISFLT  0x40
#define LCDTY   0x3f

#ifndef ADCIN
#define ADCIN 0x08//比较器正极输入端
#endif
#define P55IN  0x00//比较器正极输入端
#ifndef BandGap 
#define BandGap 0x00//比较器负极输入端
#endif
#define P54IN  0x04//比较器负极输入端
void CMPbegin(u8 CMP_P,u8 CMP_N,u8 CMP_OUT_EN);
u8 CMPresult();
#endif