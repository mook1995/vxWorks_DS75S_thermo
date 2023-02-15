#include "inc.h"

static void show_func2()
{
	printf("show func2 _setting\n");
}

static void mode_func1()
{
	printf("mode func1 _alarm\n");
}

static void mode_func2()
{
	printf("mode func2 _normal\n");
}

static void config_func1()
{
	printf("config func1 _hyst\n");
}

static void config_func2()
{
	printf("config func2 _os\n");
}

static void config_func3()
{
	if (!strcmp(argv_buf[2], "high" ))
	{
		setResolution_high();
		printf("Ok\n");
	}
	else if (!strcmp(argv_buf[2], "low"))
	{
		setResolution_low();
		printf("Ok\n");
	}
	else
	{
		printf("Wrong Command Argument...\n");
	}
}

static void exit_func1()
{
	printf("Exiting....\n");
}

ShowCommand ShowTable[] = 
{
		{"-temp", readTempFunc},
		{"-setting", readTempSetting},
		{"-repeat", readTempRepeat},
		{"-time", readClkFunc},
		{"-HYST"},
		{"alarm", mode_func1},
		{"normal", mode_func2},
		{"-now", exit_func1}
};

ConfigCommand ConfigTable[] = 
{
		{"-hyst", config_func1},
		{"-os", config_func2},
		{"-res", config_func3},
		{"-now", exit_func1}
};



static INT32 show_tbl_size = sizeof(ShowTable)/sizeof(ShowCommand);

static void opt_check_show(INT8* opt)
{
	int i; 
	
	for (i = 0; i < show_tbl_size; i++)
	{
		if (!strcmp(opt, ShowTable[i].opt1))
		{
			ShowTable[i].function();
		}
	}
}

static INT32 config_tbl_size = sizeof(ConfigTable)/sizeof(ConfigCommand);

static void opt_check_config(INT8* opt)
{
	int i;
	
	for (i = 0; i < config_tbl_size; i++)
	{
		if (!strcmp(opt, ConfigTable[i].opt1))
		{
			ConfigTable[i].function();
		}
	}
}

INT32 ds_exit_cli(INT8* opt)
{
	opt_check_show(opt);
	return 1;
}

INT32 ds_show(INT8* opt)
{
	opt_check_show(opt);
	return 0;
}

INT32 ds_mode(INT8* opt)
{
	//opt_check(opt);
	return 0;
}

INT32 ds_config(INT8* opt)
{
	opt_check_config(opt);
	return 0;
}
