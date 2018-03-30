#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include "delay.h"
#include "pwm.h"
#include "ultrasonic.h"
#include "uart.h"
#include "motor.h"
#include "camera.h"
#define DATA_LEN 31

u32* peri_phy_base = (u32*)BCM2835_PERI_BASE;	/* peripheral physical address, default is 0x3f000000 for raspberryPi 3b */
u32 peri_size = BCM2835_PERI_SIZE;	/* peripheral area size*/

volatile u32* peri_base;	/* peripheral virtual address */
volatile u32* gpio_base;	/* virtual gpio address */
volatile u32* pwm_base;		/* pwm */
volatile u32* clock_base;	/* clock manager */
volatile u32* st_base;		/* system timer */
volatile u32* uart_base;	/* uart */

typedef struct{
	u8 can_read = 0;
	char data[DATA_LEN];
}data_struct;

int base_setup(void);		/* find peripheral physical through "/proc/device-tree/soc/ranges" */
void set_flag(int fd, int flag);	/* make the stdin stream nonblock for convenience of set PID parameters while */
				/* no preventing the ultrasonic from working */
void clear_flag(int fd, int flag);/* make the stdin stream block for convenice of input PID parameters */
void* receive_data(void* arg);
void str2num(char* str, int* P, int* I, int* D);

extern int P;
extern int I;
extern int D;

int main()
{
	int distance, distance_temp,angle=0;
	int i;
	char ch;
	if (base_setup() == -1)	/* find peripheral physical base */
		return -1;
	set_flag(0, O_NONBLOCK);	/* set stdin nonblock */
	pwm_init(256, AR);	/* 19.2Mhz/256/100 = 75kHz/100 = 750Hz */
	ultrasonic_init();	/* set gpio of emiting and receive of the ultrasonic */
	motor_init();		/* set pwm duty ratio */
	motor_enable();
	uart_init(9600);
	data_struct ds;
	ds.data[DATA_LEN - 1] = '\0';
	pthread_t uart_thread;
	if(pthread_create(&uart_thread, NULL, receive_data, (void*)&ds) != 0){
		perror("uart_thread create fail\n");
        	return 1;
	}
	printf("uart_thread create succeed\n");
	camera_init();
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
		else if (ch == 'e' || P == 0)	/* if 'e' is input, then end the motor's running */
			motor_disable();
		else if(ch == 'r')	/* if 'e' is input, then restart the motor's running */
			motor_enable();

		if(ds.can_read){
//			printf("%s", ds.data);
			ds.can_read = 0;
			str2num(ds.data, &P, &I, &D);
		}

		angle = camera_getAngle();
//		printf("angle = %d\n", angle);
		if(angle == 0){
			distance = 0;
			for(i=0; i < 8; i++){
				distance_temp = ultrasonic_getDistance();
				if(distance_temp < -25 || distance_temp > 100){
					continue;
				}
				distance += distance_temp;
				delay_us(100);
			}
			distance /= 8;
//			printf("dis = %d, delt = %dcm\n", distance, distance-DISTANCE_CM);
//			printf("P=%d, I=%d, D=%d\n", P, I, D);
			motor_doPID(distance - DISTANCE_CM);
		}
		else
			motor_turn(angle);
		delay_ms(30);
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
	peri_base = (u32 *)mmap(NULL, peri_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (u32)peri_phy_base);
	gpio_base = peri_base + BCM2835_GPIO_OFFSET/4;
	pwm_base = peri_base + BCM2835_PWM_OFFSET/4;
	clock_base = peri_base + BCM2835_CLOCK_OFFSET/4;
	st_base = peri_base + BCM2835_ST_OFFSET/4;
	uart_base = peri_base + BCM2835_UART0_OFFSET/4;
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

void* receive_data(void* arg)
{
	int i;char ch;
	data_struct* ds = (data_struct*)arg;
	while(1) {
		if(!uart_isRXFE() && !(ds->can_read)) {	//printf("*");
			if(uart_getc() != '^') {	/**/
				ds->can_read = 0;
				continue;
			}
			for(i = 0; i < DATA_LEN; i++) {
				if (ds->data[i] == '\0')
					break;
				ds->data[i] = '\0';
			}
			for(i = 0; i < DATA_LEN; i++){
				while(uart_isRXFE());
				ds->data[i] = uart_getc();
				if(ds->data[i] == '\r' || ds->data[i]== '\n'){
					ds->data[i] = '\0';
					ds->can_read = 1;
					break;
				}
				else if(ds->data[i] != ',' && ds->data[i] != '.' && ds->data[i] != ' ' && 
					!(ds->data[i] >= '0' && ds->data[i] <= '9')) {
					ds->can_read = 0;
					break;
				}
			}
			if(i == DATA_LEN)
				ds->can_read = 0;
		}
	}
	ds->can_read = 0;
	return NULL;
}

u32 _pow(u16 d,u16 z){
	unsigned int res=1;
	while(z){
		res*=d;
		z--;
	}
	return res;
}

void str2num(char* str, int* P, int* I, int* D)
{
	u16 len = strlen(str);
	u8 is_float = 0, n_float= 0;
	u16 i, n = 0;
	u16 mant = 0, frac = 0;
	for(i = 0; i < len; i++){
		if(str[i] >= '0' && str[i] <= '9'){
			if(!is_float){
				mant = mant * 10 + (str[i] - '0');
			}
			else{
				frac = frac * 10+(str[i] - '0');
				n_float++;
			}
		}
		else if(str[i] == '.'){	/* float number */
			is_float = 1;
		}
		else{	/* interval flag */
			n++;
			if(n == 1){
				*P = mant + (frac+0.0)/_pow(10, n_float);	/* P */
			}
			else if(n == 2){
				*I = mant + (frac+0.0)/_pow(10, n_float);	/* I */
			}
			mant = 0;
			frac = 0;
			n_float = 0;
			is_float = 0;
		}
	}
	if(n == 2){
		*D = mant + (frac + 0.0)/_pow(10, n_float);	/* D */
	}
}

