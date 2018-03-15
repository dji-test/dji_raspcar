#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "bcm2835register.h"

#define SPEED_SOUND 340

#ifdef __cplusplus
extern "C" {
#endif

void ultrasonic_init(void);
float ultrasonic_getDistance(void);

#ifdef __cplusplus
}
#endif

#endif

