#ifndef __PWM_H
#define __PWM_H
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "bcm2835register.h"

typedef unsigned int u32;


extern volatile u32* gpio_base;
extern volatile u32* pwm_base;
extern volatile u32* clock_base;

void pwm_init(u32 psc, u32 arr);
void pwm_set_compare(u32 channel, u32 cmp);

#endif
