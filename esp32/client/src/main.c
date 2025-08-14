/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of UART using polling.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c6/api-reference/peripherals/uart.html
 *
 * @version 0.1
 * @date 2025-04-01
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/uart.h>

#define UART UART_NUM_1   // Using UART1
#define TX_PIN GPIO_NUM_5 // Define TX pin
#define RX_PIN GPIO_NUM_4 /**                                                                                                                  \
                           * @file main.c                                                                                                      \
                           * @author Faroch Mehri (faroch.mehri@ya.se)                                                                         \
                           * @brief An example of UART using polling.                                                                          \
                           *        Look at: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c6/api-reference/peripherals/uart.html \
                           *                                                                                                                   \
                           * @version 0.1                                                                                                      \
                           * @date 2025-04-01                                                                                                  \
                           *                                                                                                                   \
                           * @copyright Copyright (c) 2025                                                                                     \
                           *                                                                                                                   \
                           */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/uart.h>

#define UART UART_NUM_0                  // Using UART1
#define TX_PIN GPIO_NUM_5                // Define TX pin
#define RX_PIN GPIO_NUM_4                // Define RX pin
#define BUF_SIZE (2 * SOC_UART_FIFO_LEN) // Buffer size shall be greater than SOC_UART_FIFO_LEN
#define MSGLEN 8                         // Message length

static const char *TAG = "UART_Example";

void app_main()
{
    uart_config_t config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Install driver and configure UART
    ESP_ERROR_CHECK(uart_driver_install(UART, BUF_SIZE, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART, &config));
    ESP_ERROR_CHECK(uart_set_pin(UART, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_LOGI(TAG, "UART initialized");

    uint8_t buffer[BUF_SIZE] = {0};

    while (1)
    {
        if (MSGLEN == uart_read_bytes(UART, buffer, MSGLEN, portMAX_DELAY))
        {
            for (size_t i = 0; i < MSGLEN; i++)
            {
                buffer[i] = toupper(buffer[i]);
            }

            if (MSGLEN != uart_write_bytes(UART, buffer, MSGLEN))
            {
                ESP_LOGE(TAG, "Failed to write");
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read");
        }
    }
}
// Define RX pin
#define BUF_SIZE (2 * SOC_UART_FIFO_LEN) // Buffer size shall be greater than SOC_UART_FIFO_LEN
#define MSGLEN 8                         // Message length

static const char *TAG = "UART_Example";

void app_main()
{
    uart_config_t config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Install driver and configure UART
    ESP_ERROR_CHECK(uart_driver_install(UART, BUF_SIZE, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART, &config));
    ESP_ERROR_CHECK(uart_set_pin(UART, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_LOGI(TAG, "UART initialized");

    uint8_t buffer[BUF_SIZE] = {0};

    while (1)
    {
        if (MSGLEN == uart_read_bytes(UART, buffer, MSGLEN, portMAX_DELAY))
        {
            for (size_t i = 0; i < MSGLEN; i++)
            {
                buffer[i] = toupper(buffer[i]);
            }

            if (MSGLEN != uart_write_bytes(UART, buffer, MSGLEN))
            {
                ESP_LOGE(TAG, "Failed to write");
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read");
        }
    }
}
