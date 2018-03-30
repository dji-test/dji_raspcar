#ifndef __UART_H
#define __UART_H


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "bcm2835register.h"

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#ifdef __cplusplus
extern "C" {
#endif

void uart_init(u32 baud_rate);
void uart_putc(char ch);
char uart_getc(void);
int uart_isRXFE(void);

#ifdef __cplusplus
}
#endif

#endif
