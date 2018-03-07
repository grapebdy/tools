#include <stdio.h>
#include <sys/io.h>
#include <unistd.h>
int mdelay(int delay)
{
	int i;
	int j;
	for (i = 0; i < delay * 0x100; i++)
		for (j = 0; j < 0x1000; j++);
}

void do_usage()
{
        printf("Usage: beep cmd [OPERAND]...\n");
        printf("x86 beep option program\n");
        printf("Command:\n");
        printf("\t-f freq\t\t set freq for beep\n");
        printf("\t-d time\t\t beep delay\n");
        printf("\t-o freq\t\t open beep with freq\n");
        printf("\t-c\t\t close beep\n");
        printf("\t-h\t\t help\n");
}

int speaker(unsigned int freq, unsigned int delay)
{
        int  bit;
        iopl(3);
	outb(0xb6,0x43);   //设置控制命令字
	outb((freq   &   0xff),0x42);  //设置定时器2的低8位
	outb((freq   >>   8),0x42);   //设置定时器2的高8位

        bit=inb(0x61);
        outb(3 | bit, 0x61);  //11使能峰鸣器输出
	mdelay(delay);
	outb(0xfc&bit, 0x61);   //00禁止峰鸣器输出
	return 0;
}

int speaker_close(void)
{
        int  bit;
        iopl(3);
	outb(0xb6,0x43);   //设置控制命令字

        bit=inb(0x61);
	outb(0xfc&bit, 0x61);   //00禁止峰鸣器输出
	return 0;
}

int speaker_open(unsigned int freq)
{
        int  bit;
        iopl(3);
	outb(0xb6,0x43);   //设置控制命令字
	outb((freq   &   0xff),0x42);  //设置定时器2的低8位
	outb((freq   >>   8),0x42);   //设置定时器2的高8位

        bit=inb(0x61);
        outb(3 | bit, 0x61);  //11使能峰鸣器输出
	return 0;
}
int main(int argc, char *argv[])
{
	int opt;
	unsigned int freq = 1000;
	unsigned int delay = 100;
	while ((opt = getopt(argc,argv,"f:d:o:ch")) !=-1) {
		switch(opt) {
		case 'f':
			freq = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'c':
			speaker_close();
			return 0;
		case 'o':
			freq = atoi(optarg);
			speaker_open(freq);
			return 0;
		case 'h':
		default:
			do_usage();
			return -1;
		}

	}
	//printf("freq: %d , delay: %d \n", freq, delay);
        speaker(freq,delay);
	return 0;
}

