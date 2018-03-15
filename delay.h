#ifndef __DELAY_H
#define __DELAY_H

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "bcm2835register.h"

typedef unsigned int u32;
typedef unsigned long long u64;

#ifdef __cplusplus
extern "C" {
#endif

void delay_ms(u32 ms);
void delay_us(u64 us);

#ifdef __cplusplus
}
#endif

#endif
