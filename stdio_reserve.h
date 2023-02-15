#ifndef __STDIO_RESERVE_H_
#define __STDIO_RESERVE_H_

#include <stdio.h>
#include <ioLib.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define MAX_LEN 100

/*/tyCo/1 : RESERVE SERIAL */
/*/tyCo/0 : DEBUG SERIAL */

#define SERIAL_RESERVE "/tyCo/0" 
#ifndef SERIAL_RESERVE
#define FILENAME_CUSTOM "/tffs0/testLog.txt"
#endif


int printf_reserve(char* str, ...);

#endif
