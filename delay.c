#include "delay.h"

extern volatile u32* st_base;

void delay_ms(u32 ms)
{
	struct timespec ts;
	ts.tv_sec  = (time_t)(ms / 1000);
	ts.tv_nsec = (long)(ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

/* very not precision when us<100us~200us
void delay_us(u32 us)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = us;
	select(0, NULL, NULL, NULL, &tv);
}
*/

void delay_us(u64 us)
{
	struct timespec ts;
	u32 high, low;
	u64 start, end;

	/* Calling nanosleep() takes at least 100-200 us, so use it for
	long waits and use a busy wait on the System Timer for the rest.*/

   	high = *(st_base + BCM2835_ST_CHI/4);
	low = *(st_base + BCM2835_ST_CLO/4);
	
	start = *(st_base + BCM2835_ST_CHI/4);
	if (start != high)	/* low counter is overflow */
		low = *(st_base + BCM2835_ST_CLO/4);
	start = (start << 32) + low;

	
	if (start == 0) {	/* Not allowed to access timer registers (result is not as precise)*/
		ts.tv_sec = 0;
		ts.tv_nsec = 1000 * (long)(us);
		nanosleep(&ts, NULL);
		return;
	}

	if (us > 450) {
		ts.tv_sec = 0;
		ts.tv_nsec = 1000 * (long)(us - 200);	/* the start of nanosleep waste about 200us */
		nanosleep(&ts, NULL);
		return;
	}

	end = start + us;
	while (1) {
		high = *(st_base + BCM2835_ST_CHI/4);
		low = *(st_base + BCM2835_ST_CLO/4);

		start = *(st_base + BCM2835_ST_CHI/4);
		if(start != high){	/* low counter is overflow*/
			low = *(st_base + BCM2835_ST_CLO/4);
		}
		start = (start << 32) + low;

		if (start >= end) {
			break;
		}
	}
}
