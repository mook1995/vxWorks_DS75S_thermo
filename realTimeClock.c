#include "inc.h"

#define NOT_USED

#ifdef NOT_USED
static binaryPrt(INT8 raw)
{
	INT32 i;
	for (i = 0; i < 8; i++)
	{
		(raw & (128 >> i)) != 0 ? printf("1"):printf("0");
	}
}

void readClkSetting();

void readClkFunc(void)
{
	char buf = 0;
	STATUS ret;
	
	//vxbI2cDevWrite(0x0033b478, 0x0E, &buf, 1);
	ret = vxbI2cDevRead(0x0033b478, 0, &buf, 1);
	if (ret == OK)
	{
		printf("OK....");
	}
	else
	{
		perror("Err..");
	}
	
	
	printf("Clk: ");
	binaryPrt(buf);
	printf("(sec)\n");
}

void readClkFuncDBG(INT32 opt)
{
	char buf = 0;
	INT32 ret;
	
	
	//vxbI2cDevWrite(0x0033b478, 0x0E, &buf, 1);

	if ((ret = vxbI2cDevRead(0x0033b478, opt, &buf, 1)) < 0)
	{
		printf("N %d ", ret);
		strerror("Status");
	}
	else
	{
		strerror("Status");
		printf("P %d ", ret);
	}
	
	
	printf("Clk: ");
	binaryPrt(buf);
	printf("(sec)\n");
}

void writeClkFuncDBG(INT32 reg, UINT val)
{
	INT32 ret = 0;
	if((ret = vxbI2cDevWrite(0x0033b478, reg, &val, 1)) < 0)
	{
		strerror("Status");
		printf("N %d ", ret);
	}
	else
	{
		strerror("Status");
		printf("P &d ", ret);
	}
}

void readClkRepeat(void);
#endif
