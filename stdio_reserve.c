#include "stdio_reserve.h"

static char data[MAX_LEN];

int printf_reserve(char* str,...)
{
	va_list vl;
	int i = 0,j = 0;
	int fd_w, recvLen, ret = 0;

#ifdef SERIAL_RESERVE
	fd_w = open(SERIAL_RESERVE,O_WRONLY, 0);
#else
	//fd_w = open(FILENAME_CUSTOM, O_WRONLY, 777);
	fd_w = open(FILENAME_CUSTOM, O_WRONLY, 0);	
#endif
	
	char buff[100] = {0}, tmp[20] = {0};
	va_start(vl, str);
	
//	while(str && str[i]) 
//	{
//		if(str[i] == '%')
//		{
//			i++;
//			switch(str[i])
//			{
//				case 'c':
//				{
//					buff[j] = (char)va_arg(vl, int);
//					j++;
//					break;
//				}
//				case 'd':
//				{
//					itoa(tmp, va_arg(vl, int), 10);
//					strcpy(&buff[j], tmp);
//					j += strlen(tmp);
//					break;
//				}
//				case 'x':
//				{
//					itoa(tmp, va_arg(vl, int), 16);
//					strcpy(&buff[j], tmp);
//					j += strlen(tmp);
//					break;
//				}
//					
//			}
//		}
//		else 
//		{
//			buff[j] = str[i];
//			j++;
//		}
//		i++;
//	}
	recvLen = vsprintf(buff, str, vl);
#ifdef PRINT 
	printf("%s\n", buff);
#endif
	//lseek(fd_w, 0, SEEK_END);
	write(fd_w, buff, recvLen);
	write(fd_w, "\n\r", 2);
	close(fd_w);
	va_end(vl);
	return j;
}
	
