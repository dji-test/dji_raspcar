#include "motor.h"
#include <stdio.h>

//extern volatile u32* gpio_base;

int P = 1;
int I = 0;
int D = 10;
int cmp[N_CTL];

void motor_init(void)
{
	int i;
	for (i = 0; i < N_CTL; i++)
		pwm_set_compare(i + 1, AR_DIV2);
	*(gpio_base + BCM2835_GPFSEL2) &= ~(7 << 3);
	*(gpio_base + BCM2835_GPFSEL2) |= (1 << 3);	//gpio21 output
}

void motor_enable(void)
{
	*(gpio_base + BCM2835_GPSET0) = (1 << 21);	//gpio21 output high
}

void motor_disable(void)
{
	*(gpio_base + BCM2835_GPCLR0) = (1 << 21);	//gpio21 output low
}

void motor_doPID(int delt)
{
	static int delt_last = 0;
	static int delt_sum = 0;
	int i;
	delt_sum += delt;
	int delt_cmp[N_CTL];
	for (i = 0; i < N_CTL; i++) {
		delt_cmp[i] = P * delt + D * (delt - delt_last) + I * delt_sum;
		delt_cmp[i] = delt_cmp[i] > AR ?  AR : delt_cmp[i];
		delt_cmp[i] = delt_cmp[i] < -AR ? -AR : delt_cmp[i];
	}
//	printf("d_c = %d", delt_cmp[0]);
	for (i = 0; i < N_CTL; i++) {
		cmp[i] += (int)(delt_cmp[i]);
		cmp[i] = cmp[i] > AR_HIGH ? AR_HIGH : cmp[i];
		cmp[i] = cmp[i] < AR_LOW  ? AR_LOW  : cmp[i];
		pwm_set_compare(i + 1, cmp[i]);
	}
//	printf("cmp = %d\n", cmp[0]);
	delt_last = delt;
}

