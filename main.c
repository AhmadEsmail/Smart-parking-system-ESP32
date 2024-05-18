#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "servo_control.h"
#include "traffic_light.h"

void app_main() {
    // Initialize the servo motor
    servo_init();

    // Create the traffic light task
    if (xTaskCreate(&traffic_light_task, "traffic_light_task", configMINIMAL_STACK_SIZE, NULL, 4, NULL) != pdPASS) {
        printf("Failed to create traffic light task!\n");
    }
}
