#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "delay.h"
#include "pwm.h"
#include "ultrasonic.h"
#include "motor.h"


u32* peri_phy_base = (u32*)BCM2835_PERI_BASE;	/* peripheral physical address, default is 0x3f000000 for raspberryPi 3b */
u32 peri_size = BCM2835_PERI_SIZE;	/* peripheral area size*/

volatile u32* peri_base;	/* peripheral virtual address */
volatile u32* gpio_base;	/* virtual gpio address */
volatile u32* pwm_base;		/* pwm */
volatile u32* clock_base;	/* clock manager */
volatile u32* st_base;		/* system timer */

int base_setup(void);		/* find peripheral physical through "/proc/device-tree/soc/ranges" */
void set_flag(int fd, int flag);	/* make the stdin stream nonblock for convenience of set PID parameters while */
				/* no preventing the ultrasonic from working */
void clear_flag(int fd, int flag);/* make the stdin stream block for convenice of input PID parameters */

extern int P;
extern int I;
extern int D;

int main()
{
	int distance, distance_temp;
	int i;
	char ch;
	if (base_setup() == -1)	/* find peripheral physical base */
		return -1;
	set_flag(0, O_NONBLOCK);	/* set stdin nonblock */
	pwm_init(256, AR);	/* 19.2Mhz/256/100 = 75kHz/100 = 750Hz */
	ultrasonic_init();	/* set gpio of emiting and receive of the ultrasonic */
	motor_init();		/* set pwm duty ratio */
	while (1) {
		if ((ch = getchar()) == 's') {	/* if 's' is input,then set PID, else go on running for nonblock of stdin */
			clear_flag(0, O_NONBLOCK);
			printf("Input P:");
			scanf("%d", &P);
			printf("Input I:");
			scanf("%d", &I);
			printf("Input D:");
			scanf("%d", &D);
			set_flag(0, O_NONBLOCK);
		}
		else if (ch == 'e')	/* if 'e' is input, then end the motor's running */
			motor_disable();
		else if(ch == 'r')	/* if 'e' is input, then restart the motor's running */
			motor_enable();
		distance = 0;
		for(i = 0; i < 8; i++) {
			distance_temp = ultrasonic_getDistance();
			if (distance_temp < -25 || distance_temp > 100)
				continue;
			distance += distance_temp;
			delay_us(100);
		}
		distance /= 8;		/* get the distance by average the 8 results */
//		printf("D = %d,delt = %dcm\n", distance, distance - DISTANCE_CM);		
		motor_doPID(distance - DISTANCE_CM);	/* do motor PID */
		delay_ms(40);		/* delay a moment,then go next loop */
	}
	return 0;
}

int base_setup()
{
	FILE* fp;
	u32 fd;
	if ((fp = fopen("/proc/device-tree/soc/ranges", "rb"))){
		unsigned char buf[4];
		fseek(fp, 4, SEEK_SET);
		if (fread(buf, 1, sizeof(buf), fp) == sizeof(buf))
			peri_phy_base = (u32 *)((long)buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0);
		fseek(fp, 4, SEEK_SET);
		if (fread(buf, 1, sizeof(buf), fp) == sizeof(buf))
			peri_size = (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0);
		fclose(fp);
		
		if ((fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) {
			perror("Can not open /dev/mem");
			return -1;
		}
	}
	peri_base = mmap(NULL, peri_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (u32)peri_phy_base);
	gpio_base = peri_base + BCM2835_GPIO_OFFSET/4;
	pwm_base = peri_base + BCM2835_PWM_OFFSET/4;
	clock_base = peri_base + BCM2835_CLOCK_OFFSET/4;
	st_base = peri_base + BCM2835_ST_OFFSET/4;
	return 0;
}

void set_flag(int fd,int flag)
{
	int val = fcntl(fd, F_GETFL, 0);
	if (val < 0) {
		perror("fcntl GETFL error!");
		exit(1);
	}
	if (fcntl(0, F_SETFL, val | flag) < 0) {
		perror("fcntl SETFL error!");
		exit(2);
	}
}

void clear_flag(int fd,int flag)
{
	int val = fcntl(fd, F_GETFL, 0);
	if (val < 0) {
		perror("fcntl GETFL error!");
		exit(1);
	}
	if (fcntl(0, F_SETFL, val & ~flag) < 0) {
		perror("fcntl SETFL error!");
		exit(2);
	}
}
