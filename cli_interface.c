#include "inc.h"

static Command command_table[] =
{
		{"show", ds_show},
		{"mode", ds_mode},
		{"config", ds_config},
		{"exit", ds_exit_cli}
};

static INT32 table_size = sizeof(command_table) / sizeof(Command);

static void command_parser(INT32* argc)
{
	INT32 i = 1, cnt = 0;
	INT8* ptr;
	
	printf("DS75S> ");
#ifdef DBG_PRT
	memset(test_buffer, 0, BUF_SIZE);
	gets(test_buffer);
#endif	
	memset(argv_buf, 0, table_size);
	
	ptr = strtok(test_buffer, " ");
	argv_buf[0] = ptr;
	printf("input: %s \n", test_buffer);
	while(ptr != NULL)
	{
		ptr = strtok(NULL, " ");
		if (ptr != NULL)
		{
			cnt++;
		}
		argv_buf[i++] = ptr;
	}
	*argc = cnt;
}

static INT32 cli_logic(void)
{
	INT32 argc, i, ret_escape = 0;
	INT8* menuStr = 
			"Menu \n\
			show -temp: display temperature\n\
			     -setting: display thermometer setting\n\n\		
			     -repeat: repeat tmperature showing every 200ms\n\n\
			mode -alarm: set the thermometer to alarm mode \n\
			     -normal: set the thermometer to normal mode\n\n\
			config -hyst [INT]: set the hyst value to INT\n\
			       -os [INT]: set the O.S value to INT\n\
			       -res [\"high\\low\"]: set thermometer resolution \n\
			";
	INT32 compare_result;
	command_parser(&argc);
	
	if (argc == 0)
	{
		printf("%s\n", menuStr);
	}
	else
	{
		for (i = 0; i < table_size; i++)
		{
			compare_result = strcmp(command_table[i].command_name, argv_buf[0]);
			if (!compare_result)
			{
				ret_escape = command_table[i].function(argv_buf[1]);
			}
		}
	}
	return ret_escape;
}


INT32 CLImain(void)
{
	FOREVER
	{
		if (cli_logic())
		{
			cleanT();
			break;
		}
	}
	return(OK);
}

