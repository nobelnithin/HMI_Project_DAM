#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_NUM UART_NUM_1
#define BUF_SIZE 1024
#define LED_GPIO GPIO_NUM_18  // Define GPIO for the LED

void init_uart() {
    // Configure UART parameters
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, GPIO_NUM_16, GPIO_NUM_17, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // Change GPIO pins as needed
}

void toggle_led() {
    // Toggle the LED state
    gpio_set_level(LED_GPIO, !gpio_get_level(LED_GPIO));
}

void app_main() {
    init_uart();
    
    // Initialize GPIO for the LED
    // gpio_pad_select_gpio(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT); // Set GPIO as output

    uint8_t data[BUF_SIZE];

    while (1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, 20 / portTICK_PERIOD_MS); // 20ms timeout
        if (len > 0) {
            data[len] = '\0'; // Null-terminate the string

            // Check for specific command to toggle the LED
            if (strcmp((char *)data, "toggle") == 0) { // Example command to toggle LED
                toggle_led();
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to avoid busy-waiting
    }
}
