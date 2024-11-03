#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include <string.h>
#include "driver/gpio.h"

// UART configuration
#define UART_NUM UART_NUM_0
#define TXD_PIN GPIO_NUM_17
#define RXD_PIN GPIO_NUM_18
#define BUF_SIZE (1024)

void init_uart() {
    // UART parameter configuration
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Install UART driver and set UART parameters
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void send_random_numbers_task(void *pvParameter) {
    char message[20];
    srand(time(NULL));  // Seed for random number generation

    while (1) {
        int random_num = rand() % 100;  // Generate a random number between 0 and 99
        snprintf(message, sizeof(message), "%d\n", random_num);
        uart_write_bytes(UART_NUM, message, strlen(message));

        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Send a number every second
    }
}

void app_main() {
    init_uart();
    xTaskCreate(send_random_numbers_task, "send_random_numbers_task", 2048, NULL, 1, NULL);
}
