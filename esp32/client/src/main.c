#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <driver/gptimer.h>

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

    ESP_LOGI(TAG, "UART initialized");

    uint8_t data = 0;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));

        if (sizeof(data) != uart_write_bytes(UART, &data, sizeof(data)))
        {
            ESP_LOGE(TAG, "Failed to write");
        }
    }
}

/*
jag får detta felet: Configuring upload protocol... AVAILABLE: cmsis-dap, esp-bridge, esp-builtin, esp-prog, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa CURRENT: upload_protocol = esptool Looking for upload port... Auto-detected: /dev/ttyUSB0 Uploading .pio/build/esp32-c6-devkitc-1/firmware.bin esptool.py v4.5.1 Serial port /dev/ttyUSB0 A fatal error occurred: Could not open /dev/ttyUSB0, the port doesn't exist *** [upload] Error 2 ======================================================================== [FAILED] Took 3.71 seconds ======================================================================== * The terminal process "platformio 'run', '--target', 'upload', '--target', 'monitor', '--environment', 'esp32-c6-devkitc-1'" terminated with exit code: 1. * Terminal will be reused by tasks, press any key to close it. När jag kör denna koden: #include <stdio.h> #include <ctype.h> #include <string.h> #include <esp_log.h> #include <driver/gpio.h> #include <driver/uart.h> #include <driver/gptimer.h> #define UART UART_NUM_0 // Using UART1 #define TX_PIN GPIO_NUM_5 // Define TX pin #define RX_PIN GPIO_NUM_4 // Define RX pin #define BUF_SIZE (2 * SOC_UART_FIFO_LEN) // Buffer size shall be greater than SOC_UART_FIFO_LEN #define MSGLEN 8 // Message length #define BAUDRATE 1048576 static const char *TAG = "UART_Example"; void app_main() { uart_config_t config = { .baud_rate = BAUDRATE, .data_bits = UART_DATA_8_BITS, .parity = UART_PARITY_DISABLE, .stop_bits = UART_STOP_BITS_1, .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, .source_clk = UART_SCLK_DEFAULT, }; // Install driver and configure UART ESP_ERROR_CHECK(uart_driver_install(UART, BUF_SIZE, 0, 0, NULL, 0)); ESP_ERROR_CHECK(uart_param_config(UART, &config)); ESP_ERROR_CHECK(uart_set_pin(UART, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)); ESP_LOGI(TAG, "UART initialized"); uint8_t data = 0; while (1) { vTaskDelay(pdMS_TO_TICKS(100)); if (sizeof(data) != uart_write_bytes(UART, &data, sizeof(data))) { ESP_LOGE(TAG, "Failed to write"); } } } trots att jag har: alexander@school:~$ ls /dev/ttyUSB* /dev/ttyUSB0 i terminalen
*/