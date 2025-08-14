#include <stdio.h>
#include <ctype.h>
#include <string.h>
// #include <esp_log.h>
#include <driver/gpio.h>
#include <driver/uart.h>

#define UART UART_NUM_0                  // Using UART1
#define TX_PIN GPIO_NUM_5                // Define TX pin
#define RX_PIN GPIO_NUM_4                // Define RX pin
#define BUF_SIZE (2 * SOC_UART_FIFO_LEN) // Buffer size shall be greater than SOC_UART_FIFO_LEN
#define MSGLEN 8                         // Message length
#define BAUDRATE 1048576

static const char *TAG = "UART_Example";

void app_main()
{
    uart_config_t config = {
        .baud_rate = BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Install driver and configure UART
    ESP_ERROR_CHECK(uart_driver_install(UART, BUF_SIZE, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART, &config));
    ESP_ERROR_CHECK(uart_set_pin(UART, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // ESP_LOGI(TAG, "UART initialized");

    uint8_t data = 0;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));

        data++;

        if (sizeof(data) != uart_write_bytes(UART, &data, sizeof(data)))
        {
            // ESP_LOGE(TAG, "Failed to write");
        }
        fflush(stdout);
    }
}
