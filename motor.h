#ifndef __MOTOR_H
#define __MOTOR_H

#include "pwm.h"
#define N_CTL 2
#define AR 100
#define AR_DIV2 50
#define AR_HIGH 90
#define AR_LOW  10
#define DISTANCE_CM 25

#ifdef __cplusplus
extern "C" {
#endif

void motor_init(void);
void motor_doPID(int delt);
void motor_enable(void);
void motor_disable(void);
void motor_turn(int angle);

#ifdef __cplusplus
}
#endif

#endif

