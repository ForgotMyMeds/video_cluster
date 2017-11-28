//////////////////////////////////////////////////////////////////////////////////
// Company:  Jiangsu Variable Supercomputer Tech Co., Ltd
// Engineer: Gu Shubin
//
// Create Date		: 2017/08/16 10:43
// Design Name		:
// Module Name		: GP_OP
// Project Name		:
// Target Devices	: ZCU102 Evaluation Board
// Tool Versions	:
// Description		: CHN: gp�ڶ�д�����������ʽ./GP_OP addr data
//
// Dependencies		:
//
// Revision			: 0.01
// Revision 		  0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////

#include "include/GP_IO.h"

int char2int(char ch)
{
	//CHN: ������ASCII���ȥ48
	if (isdigit(ch))
		return ch - 48;

	//CHN: ��a~f��A~F�������ĸ������-1
	if ((ch<'A') || ((ch>'F') && (ch<'a')) || (ch>'f'))
		return -1;

	//CHN: ��д��ĸ��55��Сд��ĸ��87
	if (isalpha(ch))
		return isupper(ch) ? ch - 55 : ch - 87;

	return -1;
}

// =============================================================================
// CHN: ���ַ���ת��Ϊ��ֵ
// =============================================================================
int hex2dec(char *hex, uint64_t *O_DATA)
{
	int len;
	int i;
	int bits;

	uint64_t temp=0;
	uint64_t num=0;

	len = strlen(hex);
	//printf("strlen:%d\r\n",len);
	for (i=0; i<len; i++)
	{
		//printf("hex:%c\r\n",*(hex+i));
		temp	= char2int(*(hex+i));
		//printf("after char2int:%x\r\n",temp);
		bits	= (len-i-1) * 4;
		temp	= temp << bits;
		num		= num + temp;
	}

	*O_DATA = num;

	return 0;
}

// =============================================================================
// CHN: һЩ��ַӳ��֮��ģ��ݲ�ϸ����
// =============================================================================
void *mapdev(uint64_t offset, uint64_t size, int exflag)
{
    void *base;
    long long memfd_2;
    memfd_2 = open("/dev/mem", O_RDWR | exflag);
    if (memfd_2 == -1)
    {
        //printf("Can't open /dev/mem.\n");
        exit(0);
    }
   // printf("/dev/mem opened.\n");
    base = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_2, offset);
    if (base == (void *) -1)
    {
       // printf("Can't map the memory to user space.\n");
        exit(1);
    }
    close(memfd_2);
    return base;
}
void unmapdev(void *base, uint64_t size)
{
    if (munmap(base, size) == -1)
    {
        //printf("Can't unmap memory from user space.\n");
        exit(1);
    }
}


// =============================================================================
// main
// =============================================================================
int GP_IO(char* command, char* address, char* data)
{
	uint64_t ADDR;
	uint64_t DATA[2];

	hex2dec(address,&ADDR);
	//printf("ADDR=%lx\n", ADDR);

	void *gpbase;
	gpbase= mapdev( 0xA0000000UL, 0x20000UL, O_SYNC);

	uint64_t status[2];

	if (strcmp(command,"read") == 0)		//read
		{
			memcpy(status,gpbase+ADDR,16);
			//printf("read address=%lx, data[127:64]=%lx, data[63:0]=%lx\n", ADDR, status[1], status[0]);
		}
	else if (strcmp(command,"write") == 0)	//write
		{
			hex2dec(data,&DATA[0]);
			DATA[1] = 0;
			memcpy(gpbase+ADDR,DATA,16);
			//printf("write address=%lx, data[127:64]=%lx, data[63:0]=%lx\n", ADDR, DATA[1], DATA[0]);
		}

	unmapdev(gpbase,0x20000UL);

	return 0;
}

uint64_t _GP_IO(char* command, char* address)
{
	uint64_t ADDR;
	uint64_t DATA[2];

	hex2dec(address,&ADDR);
	//printf("ADDR=%lx\n", ADDR);

	void *gpbase;
	gpbase= mapdev( 0xA0000000UL, 0x20000UL, O_SYNC);

	uint64_t status[2];

	if (strcmp(command,"read") == 0)		//read
		{
			memcpy(status,gpbase+ADDR,16);
			ret = status[0];
			//printf("read address=%lx, data[127:64]=%lx, data[63:0]=%lx\n", ADDR, status[1], status[0]);
		}

	unmapdev(gpbase,0x20000UL);

	return ret;
}
