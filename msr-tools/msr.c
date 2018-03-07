/*
*	X86 MSR read/write options
*	author: grapebdy
*/
#include <stdio.h>
#include <sys/io.h>
#include <sys/ioctl.h>
#include <asm/msr.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MSR_DEVICE_NAME "/dev/cpu/0/msr"

void show_msr_addr()
{
	printf("Address MAP\n");
	printf("DST\t\t\t\tSRC\t\t\t\t\n");
	printf("\t\tCPU Core \t\t GLCP\n");
	printf("CPU \t\t 0x1000xxxx\t\t 0x2000xxxx\n");
	printf("GLIU0\t\t 0x1000xxxx\t\t 0x2000xxxx\n\n");
	printf("GLMC\t\t 0x1000xxxx\t\t 0x2000xxxx\n\n");
	printf("GLIU1\t\t 0x1000xxxx\t\t 0x2000xxxx\n\n");
	printf("GLCP\t\t 0x1000xxxx\t\t 0x2000xxxx\n\n");
	printf("GIO\t\t 0x1000xxxx\t\t 0x2000xxxx\n\n");
	printf("DC\t\t 0x1000xxxx\t\t 0x2000xxxx\n\n");
	printf("VG\t\t 0x1000xxxx\t\t 0x2000xxxx\n\n");
}

void do_usage()
{
        printf("Usage: msr cmd [OPERAND]...\n");
        printf("x86 msr option program\n");
        printf("Command:\n");
        printf("\t-r reg\t\t read msr reg for x86\n");
        printf("\t-w reg\t\t write msr reg for x86 (use with -l/-h)\n");
        printf("\t-l low\t\t msr low bit\n");
        printf("\t-h high\t\t msr high bit\n");
        printf("\t-s\t\t show msr regs address\n");
}

int msr_read(unsigned int reg, unsigned int *low, unsigned int *high)
{
	int fd, ret;
	unsigned int msr_ret[8];

	memset(msr_ret, 0, 8*4);

        fd = open(MSR_DEVICE_NAME, O_RDWR);
        if (fd < 0) {
		printf("open %s error\n", MSR_DEVICE_NAME);
		return fd;
	}

	lseek(fd, reg, SEEK_SET);
	ret = read(fd, msr_ret, 8);
	*low = msr_ret[0];
	*high = msr_ret[1];

        close(fd);
}
int msr_write(unsigned int reg, unsigned int low, unsigned int high)
{
	int fd, ret;
	int i;
	unsigned int msr_ret[2];
	memset(msr_ret, 0, 2*4);

        fd = open(MSR_DEVICE_NAME, O_RDWR);
        if (fd < 0) {
		printf("open %s error\n", MSR_DEVICE_NAME);
		return fd;
	}
	msr_ret[0] = low;
	msr_ret[1] = high;
	lseek(fd, reg, SEEK_SET);
	ret = write(fd, msr_ret, 8);
	if (ret == -1) {
		printf("write error \n");
	}

        close(fd);
}
int main(int argc, char *argv[])
{
	int opt;
	unsigned int reg = 1000;
	unsigned int command = -1;
	unsigned int high,low;

	if (argc < 2) {
		do_usage();
		return -1;
	}

	while ((opt = getopt(argc,argv,"w:r:l:h:s")) !=-1) {
		switch(opt) {
		case 'r':
			sscanf(optarg, "%x", &reg);
			printf("%x \n",reg);
			command = 1;
			break;
		case 'w':
			sscanf(optarg, "%x", &reg);
			printf("%x \n",reg);
			command = 2;
			break;
		case 'l':
			sscanf(optarg, "%x", &low);
			printf("low %.8x \n",low);
			break;
		case 'h':
			sscanf(optarg, "%x", &high);
			printf("high %.8x \n",high);
			break;
		case 's':
			show_msr_addr();
			return 0;
		default:
			do_usage();
			return -1;
		}

	}

	switch (command) {
	case 1:
		msr_read(reg, &low, &high);
		printf("%.8x %.8x\n", low, high);
		break;
	case 2:
		msr_write(reg, low, high);
		break;
	}

	return 0;
}

