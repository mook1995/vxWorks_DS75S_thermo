#include "inc.h"
#include "taskLib.h"

//#define RESERVE_STDIO
#define TEST_EXP

static INT32 read_reg;
static TASK_ID readT = 0;
static TASK_ID regT = 0;

//typedef float	FLOAT;
typedef double FLOAT;

typedef struct two_byte_
{
	INT8 data;
	UINT8 belowZero;
//}__attribute__((packed))twoByte;
}twoByte;

static binaryPrt(INT8 raw)
{
	INT32 i;
	for (i = 0; i < 8; i++)
	{
		(raw & (128 >> i)) != 0 ? printf("1"):printf("0");
	}
}

static FLOAT shifter(INT32 count)
{
	return 1 << count;
}

static FLOAT binaryFloatP(UINT8 raw)
{
	INT32 i;
    FLOAT ret = 0.0;
	for (i = 0; i < 8; i++)
	{
		if((raw & (128 >> i)) != 0)
        {
            ret += 1.0 / shifter(i + 1);
        }
	}
    return ret;
}

static FLOAT binaryFloatN(UINT8 raw)
{
	INT32 i;
    FLOAT ret = 0.0;
	for (i = 0; i < 8; i++)
	{
		if((raw & (128 >> i)) != 0)
        {
            ret -= 1.0 / shifter(i + 1);
        }
	}
    return ret;
}


#ifdef ON_SIMT
#else
void readTempSetting()
{
	INT8 readData = 0;
	
#ifdef RESERVE_STDIO
	printf_reserve("Reading Thermometer...\n");
#else
	printf("Thermometer Setting Bits: ");
#endif
	vxbI2cDevRead(0x0033b678, (UINT8)1, &readData, 1);
#ifdef DBG_PRT_DEV
	binaryPrt(readData); putchar('\n');
#endif
#ifdef RESERVE_STDIO
	printf_reserve("Current Temperature: %d\n", readData);
#else
	//printf("Current Temperature: %d\n", readData);
	binaryPrt(readData);
	putchar('\n');
#endif
}
#endif

void readOs()
{
	twoByte buf = {0};
	INT16 twoByte = 0;
	UINT8 transF = 0;
	INT8 readData = 0;
	
#ifdef RESERVE_STDIO
	printf_reserve("Reading OS Thermometer...\n");
#else
	printf("Reading OS Temperature...\n");
#endif
	vxbI2cDevRead(0x0033b678, (UINT8)3, &buf, 2);
#ifdef DBG_PRT_DEV
	binaryPrt(readData); putchar('\n');
#endif
#ifdef RESERVE_STDIO
	printf_reserve("Current OS Temperature: %d\n", readData);
#else
	readData = buf.data;
	transF = buf.belowZero;
	printf("Current OS Temperature: ");
	if (readData > 0)
	{
		printf("%f¡É\n", (FLOAT)readData + binaryFloatP(transF));
	}
	else
	{
		printf("%f¡É\n", (FLOAT)readData + binaryFloatN(transF));
	}
#endif
}

void writeOs()
{
	twoByte buf = {0};
	INT32 d;
	INT32 b;
	printf("Os: ");
	scanf("%x %x", &d, &b);
	printf("result: %x.%x\n", d, b);
	
	buf.data = d;
	buf.belowZero = b;
	
	//buf.data = 0x1F;
	//buf.belowZero = 0x80;
	
	vxbI2cDevWrite(0x0033b678, (UINT8)3, &buf, 2);
	printf("Ok\n");
}


#ifdef ON_SIMT
#else
void readTempFunc()
{
	twoByte buf = {0};
	short test = 0;
	INT16 twoByte = 0;
	UINT8 transF = 0;
	INT8 readData = 0;
	
#ifdef RESERVE_STDIO
	printf_reserve("Reading Thermometer...\n");
#else
	printf("Reading Thermometer...\n");
#endif
	vxbI2cDevRead(0x0033b678, (UINT8)0, &buf, 2);
#ifdef DBG_PRT_DEV
	binaryPrt(readData); putchar('\n');
#endif
#ifdef RESERVE_STDIO
	printf_reserve("Current Temperature: %d\n", readData);
#else
	readData = buf.data;
	transF = buf.belowZero;
	printf("Current Temperature: ");
	if (readData > 0)
	{
		printf("%f¡É\n", (FLOAT)readData + binaryFloatP(transF));
	}
	else
	{
		printf("%f¡É\n", (FLOAT)readData + binaryFloatN(transF));
	}
	
#ifdef TEST_EXP
	vxbI2cDevRead(0x0033b678, (UINT8)0, &test, 2);
	printf("RAW: %d\n", test);
	printf("EXP: %f\n", (float)((test / 128)*625)/10000);
#endif
#endif
}
#endif

void controlTask(void)
{
	INT32 sock;
	struct sockaddr_in addr;
	struct sockaddr_in clnt_addr;
	static INT8 recv_buffer[32] = {0};
	INT32 recv_len;
	INT32 addr_len;
	INT32 i;
	struct timeval optVal = {60, 0}; /* {second, milli second} */
	INT32 optLen = sizeof(optVal);
	
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("socket error");
		return;
	}
	
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (INT8*)&optVal, optLen);
	
	memset(recv_buffer, 0, sizeof(recv_buffer));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9002);
	
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("bind error");
		close(sock);
		return;
	}
	
	memset(&clnt_addr, 0, sizeof(clnt_addr));
	addr_len = sizeof(clnt_addr);
	
	if ((recv_len = recvfrom(sock, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr*)&clnt_addr, &addr_len)) < 0)
	{
		if (recv_len == -1)
		{
			read_reg = 1;
			close(sock);
			return;
		}
		else
		{
			perror("recv error");
			close(sock);
			return;
		}
	} 
		
	if(!strcmp(recv_buffer, "exit"))
	{
		read_reg = 1;
	}
	
	if (regT != 0)
	{
		regT = taskIdSelf();
	}
	
	close(sock);
	return;
}

static void readTempTask(void)
{
	read_reg = 0;
	FOREVER
	{
		readTempFunc();
		taskDelay(1000);
		if (read_reg == 1)
		{
			break;
		}
	}
	
	if (readT != 0)
	{
		readT = taskIdSelf();
	}
}

void readTempRepeat()
{
	//taskSpawn("taskName_readTemp", 110, VX_FP_TASK | VX_SPE_TASK, 10240, (FUNCPTR)readTempTask,0,0,0,0,0,0,0,0,0,0);
	//taskSpawn("taskName_setReg", 110, VX_FP_TASK | VX_SPE_TASK, 10240, (FUNCPTR)controlTask,0,0,0,0,0,0,0,0,0,0);
	taskSpawn("taskName_readTemp", 110, VX_FP_TASK, 10240, (FUNCPTR)readTempTask,0,0,0,0,0,0,0,0,0,0);
	taskSpawn("taskName_setReg", 110, VX_FP_TASK, 10240, (FUNCPTR)controlTask,0,0,0,0,0,0,0,0,0,0);
	//taskSpawn("taskName_readTemp", 110, 0x0008, 10240, (FUNCPTR)readTempTask,0,0,0,0,0,0,0,0,0,0);
	//taskSpawn("taskName_setReg", 110, 0x0008, 10240, (FUNCPTR)controlTask,0,0,0,0,0,0,0,0,0,0);
}

STATUS cleanT(void)
{
	if (regT != 0)
	{
		taskDelete(regT);
		regT = 0;
	}
	if (readT != 0)
	{
		taskDelete(readT);		
		readT = 0;
	}
	return(OK);
}

void setResolution_low()
{
	UINT8 data = 0x00;
	vxbI2cDevWrite(0x0033b678, (UINT8)1, &data, 1);
}

void setResolution_high()
{
	UINT8 data = 0x60;	/* 0110 0000 */
	vxbI2cDevWrite(0x0033b678, (UINT8)1, &data, 1);
}
