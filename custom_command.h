#ifndef CCMD_H_
#define CCMD_H_

typedef struct
{
	INT8* opt1;
	void (*function)(void);
} ShowCommand;

typedef struct 
{
	INT8* opt1;
	void (*function)(void);
} ModeCommand;

typedef struct
{
	INT8* opt1;
	void (*function)(void);
} ConfigCommand;

extern void readTempFunc();

extern void readTempRepeat();

INT32 ds_exit_cli(INT8* opt);

INT32 ds_show(INT8* opt);

INT32 ds_mode(INT8* opt);

INT32 ds_config(INT8* opt);


#endif
