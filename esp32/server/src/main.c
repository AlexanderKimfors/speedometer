#include <ctype.h>
#include "esp_bt.h"
#include <stdbool.h>
#include "nvs_flash.h"
#include "nimble/ble.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "nimble/nimble_port.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "nimble/nimble_port_freertos.h"

#include "setting.h"
#include "driver/uart.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//----------------------
// UART Config
#define UART_PORT UART_NUM_0
#define BUF_SIZE (2 * SOC_UART_FIFO_LEN)
//----------------------

#define TAG "SERVER"
#define DEVICE_NAME "BLE_SERVER"
// #define CLIENT_ADDRESS "c0:ff:ee:12:34:56"
static uint8_t client_addr[6] = {0xC0, 0xFF, 0xEE, 0x12, 0x34, 0x56};
//{0xC0, 0xFF, 0xEE, 0x12, 0x34, 0x56};

#define BLE_SVC_UUID16 0xABF0     /* 16 Bit Service UUID */
#define BLE_SVC_CHR_UUID16 0xABF1 /* 16 Bit Service Characteristic UUID */

int gatt_svr_register(void);
static int ble_server_gap_event(struct ble_gap_event *event, void *arg);
static int ble_svc_gatt_handler(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);

static bool notify_enable = false;
static uint8_t own_addr_type;
static uint16_t ble_svc_gatt_read_val_handle;
static uint16_t g_conn_handle = BLE_HS_CONN_HANDLE_NONE;

static const struct ble_gatt_svc_def new_ble_svc_gatt_defs[] = {
    {
        /* The Service */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(BLE_SVC_UUID16),
        .characteristics = (struct ble_gatt_chr_def[]){{
                                                           /* The characteristic */
                                                           .uuid = BLE_UUID16_DECLARE(BLE_SVC_CHR_UUID16),
                                                           .access_cb = ble_svc_gatt_handler,
                                                           .val_handle = &ble_svc_gatt_read_val_handle,
                                                           .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY,
                                                       },
                                                       {
                                                           0, /* No more characteristics */
                                                       }},
    },
    {
        0, /* No more services. */
    },
};

// static char *addr_str(const uint8_t *addr)
// {
//     static char buf[18]; // "xx:xx:xx:xx:xx:xx" + null
//     sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x",
//             addr[5], addr[4], addr[3],
//             addr[2], addr[1], addr[0]);
//     return buf;
// }

// Logs information about a connection to the console.
static void ble_server_print_conn_desc(struct ble_gap_conn_desc *desc)
{
    char addr[18];

    sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", desc->our_ota_addr.val[5], desc->our_ota_addr.val[4],
            desc->our_ota_addr.val[3], desc->our_ota_addr.val[2], desc->our_ota_addr.val[1], desc->our_ota_addr.val[0]);

    sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", desc->our_id_addr.val[5], desc->our_id_addr.val[4],
            desc->our_id_addr.val[3], desc->our_id_addr.val[2], desc->our_id_addr.val[1], desc->our_id_addr.val[0]);

    sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", desc->peer_ota_addr.val[5], desc->peer_ota_addr.val[4],
            desc->peer_ota_addr.val[3], desc->peer_ota_addr.val[2], desc->peer_ota_addr.val[1], desc->peer_ota_addr.val[0]);

    sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", desc->peer_id_addr.val[5], desc->peer_id_addr.val[4],
            desc->peer_id_addr.val[3], desc->peer_id_addr.val[2], desc->peer_id_addr.val[1], desc->peer_id_addr.val[0]);
}

static void ble_server_advertise(void)
{
    struct ble_hs_adv_fields fields = {0};
    const char *name = ble_svc_gap_device_name();

    // General discoverability and BLE-only (BR/EDR unsupported)
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

    /* Set device name */
    fields.name = (uint8_t *)name;
    fields.name_len = strlen(name);
    fields.name_is_complete = 1;

    /* Set device tx power */
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    fields.tx_pwr_lvl_is_present = 1;

    /* 16-bit service UUIDs (alert notifications) */
    fields.uuids16 = (ble_uuid16_t[]){BLE_UUID16_INIT(BLE_SVC_UUID16)};
    fields.uuids16_is_complete = 1;
    fields.num_uuids16 = 1;

    /* Set device LE role */
    fields.le_role = BLE_GAP_ROLE_MASTER;
    fields.le_role_is_present = 1;

    int status = ble_gap_adv_set_fields(&fields);
    if (status == 0)
    {
        // ble_hs_id_add_to_whitelist(BLE_ADDR_PUBLIC, client_addr);

        struct ble_gap_adv_params adv_params = {0};

        /* Set connetable and general discoverable mode */
        adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
        adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
        adv_params.filter_policy = BLE_HCI_ADV_FILT_NONE;

        /* Start advertising */
        status = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_server_gap_event, NULL);
    }
}

static int ble_server_gap_event(struct ble_gap_event *event, void *)
{
    struct ble_gap_conn_desc desc;

    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT: /* A new connection was established or a connection attempt failed. */
        if (event->connect.status == 0)
        {
            g_conn_handle = event->connect.conn_handle;
            assert(0 == ble_gap_conn_find(event->connect.conn_handle, &desc));
            ble_server_print_conn_desc(&desc);
        }
        else
        {
            /* Connection failed; resume advertising. */
            ble_server_advertise();
        }
        break;

    case BLE_GAP_EVENT_DISCONNECT:
        g_conn_handle = BLE_HS_CONN_HANDLE_NONE;
        notify_enable = false;
        ble_server_print_conn_desc(&event->disconnect.conn);
        ble_server_advertise(); /* Connection terminated; resume advertising. */
        break;

    case BLE_GAP_EVENT_CONN_UPDATE: /* The central has updated the connection parameters. */
        assert(0 == ble_gap_conn_find(event->conn_update.conn_handle, &desc));
        ble_server_print_conn_desc(&desc);
        break;

    case BLE_GAP_EVENT_ADV_COMPLETE:
        ble_server_advertise();
        break;

    case BLE_GAP_EVENT_MTU:
        /* Maximum Transmission Unit defines the maximum size of a single ATT (Attribute Protocol) payload,
         i.e., how much data can be sent in a single BLE GATT read/write/notify/indication operation. */
        break;

    case BLE_GAP_EVENT_SUBSCRIBE:
        notify_enable = true;
        break;

    default:
        break;
    }

    return 0;
}

static void ble_server_on_reset(int reason)
{
}

static void ble_server_on_sync(void)
{
    // Din egen hårdkodade random static address (byt värden som du vill)
    // Viktigt: MSB i första byte (custom_addr[5]) måste ha bitarna 11 (0xC0).
    uint8_t custom_addr[6] = {0x9A, 0x78, 0x56, 0x34, 0x12, 0xC0};
    custom_addr[5] |= 0xC0; // säkerställ random static enligt spec

    int rc = ble_hs_id_set_rnd(custom_addr);
    if (rc != 0)
    {
        return;
    }

    // Tala om för stacken vilken typ av address vi använder (random)
    rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if (rc != 0)
    {
        return;
    }

    // Logga den address som används
    uint8_t addr_val[6];
    ble_hs_id_copy_addr(own_addr_type, addr_val, NULL);

    // Starta advertising med den addressen
    ble_server_advertise();
}

/* Callback function for custom service */
static int ble_svc_gatt_handler(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *)
{
    switch (ctxt->op)
    {
    case BLE_GATT_ACCESS_OP_READ_CHR:
        break;

    case BLE_GATT_ACCESS_OP_WRITE_CHR:
    {

        char buffer[OS_MBUF_PKTLEN(ctxt->om)];
        memset(buffer, 0, sizeof(buffer));

        // Attribute data is contained in ctxt.om.
        assert(0 == os_mbuf_copydata(ctxt->om, 0, sizeof(buffer), buffer));
        for (int i = 0; i < sizeof(buffer); i++)
        {
            buffer[i] = toupper(buffer[i]);
        }

        struct os_mbuf *txom = ble_hs_mbuf_from_flat(buffer, sizeof(buffer));
        ble_gatts_notify_custom(conn_handle, ble_svc_gatt_read_val_handle, txom);
    }
    break;
    }

    return 0;
}

static void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *)
{
    // char buf[BLE_UUID_STR_LEN] = {0};

    switch (ctxt->op)
    {
    case BLE_GATT_REGISTER_OP_SVC:
        break;

    case BLE_GATT_REGISTER_OP_CHR:
        break;

    case BLE_GATT_REGISTER_OP_DSC:
        break;

    default:
        assert(0);
        break;
    }
}

int gatt_svr_init(void)
{
    ble_svc_gap_init();
    ble_svc_gatt_init();

    int status = ble_gatts_count_cfg(new_ble_svc_gatt_defs);
    if (status == 0)
    {
        status = ble_gatts_add_svcs(new_ble_svc_gatt_defs);
    }

    return status;
}

void uart_task(void *pvParameters)
{
    uint8_t data[BUFFLEN];

    while (1)
    {
        int len = uart_read_bytes(UART_PORT, data, BUFFLEN, portMAX_DELAY); // 1s timeout
        if (len == BUFFLEN)
        {
            if (g_conn_handle != BLE_HS_CONN_HANDLE_NONE && notify_enable)
            {
                struct os_mbuf *txom = ble_hs_mbuf_from_flat(data, len);
                ble_gatts_notify_custom(g_conn_handle, ble_svc_gatt_read_val_handle, txom);
            }
        }
    }
}

void app_main(void)
{
    uart_config_t uart_config = {
        .baud_rate = BAUDRATE, // Samma som du använder på PC-sidan
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    // Installera UART-drivrutin
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT, BUF_SIZE, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    xTaskCreate(uart_task, "MyTask", 2048, NULL, 5, NULL);

    esp_err_t status = nvs_flash_init();
    if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        status = nvs_flash_init();
    }
    ESP_ERROR_CHECK(status);

    ESP_ERROR_CHECK(nimble_port_init());
    ESP_ERROR_CHECK(esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P20));

    /* Initialize the NimBLE host configuration. */
    ble_hs_cfg.reset_cb = ble_server_on_reset;
    ble_hs_cfg.sync_cb = ble_server_on_sync;
    ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    /* Register custom service */
    assert(0 == gatt_svr_init());

    /* Set the default device name. */
    assert(0 == ble_svc_gap_device_name_set(DEVICE_NAME));

    nimble_port_run(); /* This function will return only when nimble_port_stop() is executed */
    nimble_port_freertos_deinit();
}