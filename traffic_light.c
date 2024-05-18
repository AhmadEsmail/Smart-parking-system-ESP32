#include "traffic_light.h"
#include "servo_control.h"

// Volatile flag to indicate button press
volatile bool buttonPressed = false;

void configure_pin_output(gpio_num_t pin) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT
    };
    gpio_config(&io_conf);
}

void configure_pin_input(gpio_num_t pin) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };
    gpio_config(&io_conf);
}

void toggle_led(gpio_num_t led, int level, int delay_ms) {
    gpio_set_level(led, level);
    vTaskDelay(delay_ms / portTICK_PERIOD_MS);
}

void IRAM_ATTR button_interrupt_handler(void* arg) {
    buttonPressed = true;
}

void traffic_light_task(void *pvParameter) {
    configure_pin_output(LED1);
    configure_pin_output(LED2);
    configure_pin_output(LED3);
    configure_pin_input(BUTTON);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON, button_interrupt_handler, (void*) BUTTON);

    while (1) {
        if (buttonPressed) {
            buttonPressed = false;
            toggle_led(LED1, 1, DELAY_PEDESTRIAN);
            toggle_led(LED1, 0, 0);

            for (int i = 0; i <= SERVO_MAX_DEGREE; i += 5) {
                move_servo(i);
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }

            toggle_led(LED2, 1, DELAY_YELLOW);
            toggle_led(LED2, 0, 0);
            toggle_led(LED3, 1, DELAY_GREEN);
            toggle_led(LED3, 0, 0);

            for (int i = SERVO_MAX_DEGREE; i >= 0; i -= 5) {
                move_servo(i);
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }

            toggle_led(LED2, 1, DELAY_YELLOW);
            toggle_led(LED2, 0, 0);
        } else {
            toggle_led(LED1, 1, DELAY_RED);
            toggle_led(LED1, 0, 0);

            for (int i = 0; i <= SERVO_MAX_DEGREE; i += 5) {
                move_servo(i);
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }

            toggle_led(LED2, 1, DELAY_YELLOW);
            toggle_led(LED2, 0, 0);
            toggle_led(LED3, 1, DELAY_GREEN);
            toggle_led(LED3, 0, 0);

            for (int i = SERVO_MAX_DEGREE; i >= 0; i -= 5) {
                move_servo(i);
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }

            toggle_led(LED2, 1, DELAY_YELLOW);
            toggle_led(LED2, 0, 0);
        }
    }
}
