
// traffic_light.h

#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED1 GPIO_NUM_19  // RED
#define LED2 GPIO_NUM_18  // YELLOW
#define LED3 GPIO_NUM_5   // GREEN
#define BUTTON GPIO_NUM_17 // BUTTON

#define DELAY_RED 1000
#define DELAY_YELLOW 500
#define DELAY_GREEN 2000
#define DELAY_PEDESTRIAN 4500

void traffic_light_task(void *pvParameter);

#endif // TRAFFIC_LIGHT_H
