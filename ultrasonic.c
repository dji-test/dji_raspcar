#include "ultrasonic.h"
#include "delay.h"

extern volatile u32* gpio_base;

void ultrasonic_init()
{
	int i;
	*(gpio_base + BCM2835_GPFSEL0/4) &= ~(7 << 15);
	*(gpio_base + BCM2835_GPFSEL0/4) |= (1 << 15);	/* gpio5 output not 21! */
	*(gpio_base + BCM2835_GPFSEL0/4) &= ~(7 << 18);	/* gpio6 input not 22! */
	*(gpio_base + BCM2835_GPPUD/4) = 1;	/* pull down */
	for (i = 0; i < 150; i++)
		i = i;
	*(gpio_base + BCM2835_GPPUDCLK0/4) |= (1 << 6);	/* enable gpio6 pull down */
	for (i = 0; i < 150; i++)
		i = i;
	*(gpio_base + BCM2835_GPPUD/4) = 0;
	*(gpio_base + BCM2835_GPPUDCLK0/4) = 0;
}

float ultrasonic_getDistance()
{
	unsigned int t=0;
	*(gpio_base + BCM2835_GPSET0/4) |= (1 << 5);	/* gpio5 output high */
	delay_us(12);
	*(gpio_base + BCM2835_GPCLR0/4) |= (1 << 5);/* gpio5 output low */

	while (!(*(gpio_base + BCM2835_GPLEV0/4) & (1 << 6)));
	while (*(gpio_base + BCM2835_GPLEV0/4) & (1 << 6)) {
		delay_us(2);
		t += 2;
	}
//	printf("t = %d\n", t);
	return (t/2/1000000.0 * SPEED_SOUND * 100);
}
