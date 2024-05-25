# smart parking system
---


## Description

This project implements a traffic light system with pedestrian crossing using ESP32 bord. The system controls the traffic lights and a servo motor to simulate pedestrian crossing. It utilizes GPIO pins and interrupts to manage the traffic light states and servo motor movement.

## schematic diagram

<img src="https://github.com/AhmadEsmail/Smart-parking-system-ESP32/blob/main/simulation.PNG" alt="Project Logo" width="500" height="400">

## Prerequisites

- ESP32 development board
- ESP-IDF development framework
- FreeRTOS
- LED components for traffic lights
- Servo motor

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/yourproject.git
   ```

2. Set up the ESP-IDF environment according to the [ESP-IDF documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html).

3. Configure the GPIO pins for the traffic lights and the servo motor according to your hardware setup.

## Usage

1. Include the necessary header files in your main program:

   ```c
   #include "traffic_light.h"
   #include "servo_control.h"
   ```

2. Initialize the servo motor and create the traffic light task in your `app_main` function:

   ```c
   void app_main() {
       // Initialize the servo motor
       servo_init();

       // Create the traffic light task
       if (xTaskCreate(&traffic_light_task, "traffic_light_task", configMINIMAL_STACK_SIZE, NULL, 4, NULL) != pdPASS) {
           printf("Failed to create traffic light task!\n");
       }
   }
   ```

3. Compile and flash the program to your ESP32 board.

4. Observe the traffic light behavior and servo motor movement.

## Components

### 1. `main.c`

This file contains the `app_main` function where the servo motor is initialized and the traffic light task is created.

### 2. `servo_control.h` and `servo_control.c`

These files define functions to initialize the servo motor and control its movement.

### 3. `traffic_light.h` and `traffic_light.c`

These files define functions to control the traffic lights and implement the traffic light task.

## GPIO Pin Configuration

- **Traffic Lights**
  - LED1: Red light
  - LED2: Yellow light
  - LED3: Green light
  - BUTTON: Pedestrian crossing button

- **Servo Motor**
  - SERVO_PIN: GPIO pin connected to the servo motor

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request.

