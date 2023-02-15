#ifndef CLI_H_
#define CLI_H_

typedef struct {
	INT8* command_name;
	INT32 (*function)(INT8* opt);
} Command;

#define DBG_PRT
#define ARG_BUF_SIZE 12
#define BUF_SIZE 64

INT8* argv_buf[12];
INT8 test_buffer[BUF_SIZE];

#endif
