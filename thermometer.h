#ifndef THERMO_H_
#define THERMO_H_

#include "stdio_reserve.h"

//#define DBG_PRT
//#define CLI_BUFF_SIZE 128


void readTempSetting();

void readTempFunc(void);

void readTempRepeat(void);

STATUS cleanT(void);

void setResolution_low();

void setResolution_high();

#endif
