#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "driver/ledc.h"
#include "driver/gpio.h"

#define SERVO_PIN GPIO_NUM_4 // Servo motor GPIO
#define SERVO_MIN_PULSEWIDTH 500
#define SERVO_MAX_PULSEWIDTH 2500
#define SERVO_MAX_DEGREE 90

void servo_init(void);
void move_servo(int degree);

#endif // SERVO_CONTROL_H
