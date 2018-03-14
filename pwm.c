#include "pwm.h"
#include "delay.h"

void pwm_init(u32 psc, u32 arr)
{
	*(gpio_base + BCM2835_GPFSEL1/4) &= ~(7 << 24);	/* reset function register bits, gpio18 */
	*(gpio_base + BCM2835_GPFSEL1/4) |= (2 << 24);	/* set function register bits, altfun5 */
	*(gpio_base + BCM2835_GPFSEL1/4) &= ~(7 << 27);	/* reset function register bits, gpio19 */
	*(gpio_base + BCM2835_GPFSEL1/4) |= (2 << 27);	/* set function register bits, altfun5 */
	*(gpio_base + BCM2835_GPFSEL1/4) &=~ (7 << 6);
	*(gpio_base + BCM2835_GPFSEL1/4) |= (4 << 6);	/* gpio12 altfun0 */
	*(gpio_base + BCM2835_GPFSEL1/4) &= ~(7 << 9);
	*(gpio_base + BCM2835_GPFSEL1/4) |= (4 << 9);	/* gpio13 altfun0 */
	
	*(clock_base + BCM2835_PWMCLK_CNTL) = BCM2835_PWM_PASSWRD | 0x01;	/* use oscillator */
	delay_ms(110);	/* Prevents clock going slow */
	while ((*(clock_base + BCM2835_PWMCLK_CNTL) & 0x80) != 0);	/* Wait for the clock to be not busy */
	delay_ms(1);
	*(clock_base + BCM2835_PWMCLK_CNTL) = BCM2835_PWM_PASSWRD | 0x11; /* Source=osc and enable */
	*(clock_base + BCM2835_PWMCLK_DIV) = BCM2835_PWM_PASSWRD | (psc << 12);	/* Clock=19.2MHz/psc */
	
	*(pwm_base + BCM2835_PWM_CTL/4) |= 1 << 7;	/* channel1 M/S transmission is used */
	*(pwm_base + BCM2835_PWM_CTL/4) |= 1 << 15;	/* channel2 M/S transmission is used */
	*(pwm_base + BCM2835_PWM_RNG1/4) = arr;	/* 19.2MHz/(psc*arr) */
	*(pwm_base + BCM2835_PWM_RNG2/4) = arr;	/* 19.2MHz/(psc*arr) */
	*(pwm_base + BCM2835_PWM_CTL/4) |= 1 << 0;	/* channel1 is enabled */
	*(pwm_base + BCM2835_PWM_CTL/4) |= 1 << 8;	/* channel2 is enabled */
}

void pwm_set_compare(u32 channel, u32 cmp)
{
	if (channel == 0 || channel > 2) {
		perror("Invaild channel");
		return;
	}
	channel -= 1;
	*(pwm_base + BCM2835_PWM_DAT1/4 + channel) = cmp;
}
