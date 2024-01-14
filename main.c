// Copyright (c) 2024 Ahmad Esmail

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h" // Library for LEDC (PWM)

// Define the GPIO pins connected to the LEDs
#define LED1 GPIO_NUM_19  //RED
#define LED2 GPIO_NUM_18  //YELLOW
#define LED3 GPIO_NUM_5 //GREEN
#define BUTTON GPIO_NUM_17  //BUTTON
// Servo parameters
#define SERVO_PIN GPIO_NUM_4 //servo motor GPIO
#define SERVO_MIN_PULSEWIDTH 500
#define SERVO_MAX_PULSEWIDTH 2500
#define SERVO_MAX_DEGREE 90 // 0-> 180

// Define the delaies
#define delay_1 1000
#define delay_2 500
#define delay_3 2000
#define delay_4 4500

ledc_channel_config_t ledc_channel;  // Declare ledc_channel as a global variable
volatile bool buttonPressed = false; //Volatile flag to indicate button press

// function to Configure the GPIO pins as output
void Conf_pin_out(gpio_num_t led){    
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL<<led),
        .mode = GPIO_MODE_OUTPUT
    };
    gpio_config(&io_conf);
}

// function to Configure the GPIO pins as input
void conf_pin_in(gpio_num_t button){
  // Set up the button GPIO as input
    gpio_config_t button_conf = {
        .pin_bit_mask = (1ULL << button),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE, // Enable the internal pull-up resistor
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable the internal pull-down resistor
        .intr_type = GPIO_INTR_ANYEDGE // Trigger on any change (press or release)
    };
    gpio_config(&button_conf);
}
// there isn't a ready library
void servo_init() {
    // Setup PWM for servo
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 50,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel = (ledc_channel_config_t){
        .channel = LEDC_CHANNEL_0,
        .duty = 0,
        .gpio_num = SERVO_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);
}

void move_servo(int degree) {
    // Convert the degree to the corresponding PWM duty
    uint32_t duty = (uint32_t)(SERVO_MIN_PULSEWIDTH + degree * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) / SERVO_MAX_DEGREE);

    // Set the duty to the LEDC channel
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}

// function to toggel the led(set the level/delay)
void toggel_led(gpio_num_t led , int level , int delay_ms){
  gpio_set_level(led , level);
  vTaskDelay(delay_ms / portTICK_PERIOD_MS);
}

void IRAM_ATTR buttonInterruptHandler(void* arg) {
    // Set the buttonPressed flag in the interrupt service routine
    buttonPressed = true;
}
//Traffic light task
void traffic_light(void *pvParameter) {
  
  Conf_pin_out(LED1);
  Conf_pin_out(LED2);
  Conf_pin_out(LED3);

  // Set up the button GPIO as input
  conf_pin_in(BUTTON);

  // Attach the interrupt to the button pin
  // Install ISR service with default configuration
  gpio_install_isr_service(0);
  // Hook ISR handler for the button
  gpio_isr_handler_add(BUTTON, buttonInterruptHandler, (void*) BUTTON);

    while(1) {

    // int Bottun_state= gpio_get_level(BUTTON);
      // If someone presses the pedestrian button (waiting more time)
      if (buttonPressed) {
      // wait more for pedestrians

      // Clear the buttonPressed flag
        buttonPressed = false;
        toggel_led(LED1 , 1 ,delay_4);
        toggel_led(LED1 , 0 ,0);
        // close the Gate :Move the servo from 0 to 90 degrees
        for (int i = 0; i <= SERVO_MAX_DEGREE; i=i+5) {
            move_servo(i);
            vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay for smooth movement
        }
        toggel_led(LED2 , 1 ,delay_2);
      
        toggel_led(LED2 , 0 ,0);
        toggel_led(LED3 , 1 ,delay_3);
        toggel_led(LED3 , 0 ,0);
        // open the Gate : Move the servo from 90 to 0 degrees
        for (int i = SERVO_MAX_DEGREE; i >= 0; i=i-5) {
            move_servo(i);
            vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay for smooth movement
        }
        toggel_led(LED2 , 1 ,delay_2);
        toggel_led(LED2 , 0 ,0);
    }
    else{   // If no one presses the pedestrian button (no waiting)
        
        toggel_led(LED1 , 1 ,delay_1);
        toggel_led(LED1 , 0 ,0);
        // close the Gate :Move the servo from 0 to 90 degrees
        for (int i = 0; i <= SERVO_MAX_DEGREE; i=i+5) {
            move_servo(i);
            vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay for smooth movement
        }
        toggel_led(LED2 , 1 ,delay_2);

        toggel_led(LED2 , 0 ,0);
        toggel_led(LED3 , 1 ,delay_3);
        toggel_led(LED3 , 0 ,0);
        // open the Gate : Move the servo from 90 to 0 degrees
        for (int i = SERVO_MAX_DEGREE; i >= 0; i=i-5) {
            move_servo(i);
            vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay for smooth movement
        }
        toggel_led(LED2 , 1 ,delay_2);
        toggel_led(LED2 , 0 ,0);
    }
    
}
}

void app_main() {
   // Initialize servo motor
    servo_init();
    // Create a tasks 
    BaseType_t task_result;
    task_result = xTaskCreate(&traffic_light, "task1", configMINIMAL_STACK_SIZE, NULL, 4, NULL);
    if (task_result != pdPASS) {
        printf("Failed to create task!\n");
        // Handle the error (e.g., reset, log, etc.)
    }

}