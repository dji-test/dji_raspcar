#include "uart.h"

extern volatile u32* uart_base;
extern volatile u32* gpio_base;

void uart_init(u32 baud_rate)
{
	int i;
	u16 mantissa;
	u16 fraction;
	float temp;

	*(gpio_base + BCM2835_GPFSEL1/4) &= ~(7 << 12);
	*(gpio_base + BCM2835_GPFSEL1/4) &= ~(7 << 15);
	*(gpio_base + BCM2835_GPFSEL1/4) |= (4 << 12);	/* gpio14 altfunc0 */
	*(gpio_base + BCM2835_GPFSEL1/4) |= (4 << 15);	/* gpio15 altfunc0 */

	*(gpio_base + BCM2835_GPPUD/4) = 0;
	for (i = 0; i < 150; i++)
		i = i;
	*(gpio_base + BCM2835_GPPUDCLK0/4) |= (3 << 14);	/* disable gpio14 and gpio15 Up and Down */
	for (i = 0; i < 150; i++)
		i = i;
	*(gpio_base + BCM2835_GPPUD/4) = 0;
	*(gpio_base + BCM2835_GPPUDCLK0/4) = 0;

	*(uart_base + BCM2835_UART0_CR/4) = 0;	/* disable UART */
	*(uart_base + BCM2835_UART0_LCRH/4) &= ~(1 << 4);
	*(uart_base + BCM2835_UART0_ICR/4) = 0x7ff;
	temp = 48000000.0/16/baud_rate;
	mantissa = (int)temp;
	fraction = (int)((temp - mantissa)*64 + 0.5);
	*(uart_base + BCM2835_UART0_IBRD/4) = mantissa;
	*(uart_base + BCM2835_UART0_FBRD/4) = fraction;
	*(uart_base + BCM2835_UART0_LCRH/4) |= (3 << 5);	/* 8 bits */
	*(uart_base + BCM2835_UART0_CR/4) |= (3 << 8);	/* enable TX and RX */
	*(uart_base + BCM2835_UART0_CR/4) |= 1;	/* enable UART */
}

void uart_putc(char ch)
{
	while( !(*(uart_base + BCM2835_UART0_FR/4) & (1 << 7)));	/* not TXFE */
	*(uart_base + BCM2835_UART0_DR/4) = 0xff & ch;
}

char uart_getc(){
	while( *(uart_base + BCM2835_UART0_FR/4) & (1 << 4));	/* RXFE */
	return (*(uart_base + BCM2835_UART0_DR/4) & 0xff);
}

int uart_isRXFE(){
	return (*(uart_base + BCM2835_UART0_FR/4) & (1 << 4));
}
