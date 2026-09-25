#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

static const char *TAG = "SPIKE_ESPIDF_NIMBLE";

// Mouse HID Report Map: 3 buttons (L/M/R) + X + Y + Wheel (Report ID 1)
// Functionally and structurally identical to the Arduino+NimBLE spike report map
static const uint8_t hid_report_map[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x01,                    //     REPORT_ID (1)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};

static uint8_t hid_info[] = {0x11, 0x01, 0x00, 0x01}; // bcdHID 1.11, bCountryCode 0, Flags 0x01 (RemoteWake)
static uint8_t hid_control_point = 0;
static uint8_t protocol_mode = 1; // 1 = Report Protocol Mode
static uint8_t mouse_input_report[4] = {0}; // [0]=buttons (bits 0..2), [1]=dx, [2]=dy, [3]=wheel
static uint8_t mouse_report_ref[] = {0x01, 0x01}; // Report ID 1, Input Report Type 1
static uint8_t pnp_id[] = {0x02, 0x5e, 0x04, 0x40, 0x00, 0x10, 0x01}; // USB/SIG, Microsoft VID, PID, Rev

static uint16_t mouse_input_handle;

static int gatt_svr_chr_access_hid(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt, void *arg) {
    uint16_t uuid16 = ble_uuid_u16(ctxt->chr->uuid);
    switch (uuid16) {
        case 0x2A4A: // HID Information
            return os_mbuf_append(ctxt->om, hid_info, sizeof(hid_info)) == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        case 0x2A4B: // Report Map
            return os_mbuf_append(ctxt->om, hid_report_map, sizeof(hid_report_map)) == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        case 0x2A4C: // HID Control Point
            if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
                if (OS_MBUF_PKTLEN(ctxt->om) > 0) {
                    hid_control_point = ctxt->om->om_data[0];
                }
                return 0;
            }
            return BLE_ATT_ERR_UNLIKELY;
        case 0x2A4E: // Protocol Mode
            if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
                return os_mbuf_append(ctxt->om, &protocol_mode, sizeof(protocol_mode)) == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
            } else if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
                if (OS_MBUF_PKTLEN(ctxt->om) > 0) {
                    protocol_mode = ctxt->om->om_data[0];
                }
                return 0;
            }
            return BLE_ATT_ERR_UNLIKELY;
        case 0x2A4D: // Report (Input)
            if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
                return os_mbuf_append(ctxt->om, mouse_input_report, sizeof(mouse_input_report)) == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
            }
            return BLE_ATT_ERR_UNLIKELY;
        default:
            return BLE_ATT_ERR_UNLIKELY;
    }
}

static int gatt_svr_dsc_access_report_ref(uint16_t conn_handle, uint16_t attr_handle,
                                          struct ble_gatt_access_ctxt *ctxt, void *arg) {
    return os_mbuf_append(ctxt->om, mouse_report_ref, sizeof(mouse_report_ref)) == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
}

static int gatt_svr_chr_access_dis(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt, void *arg) {
    uint16_t uuid16 = ble_uuid_u16(ctxt->chr->uuid);
    switch (uuid16) {
        case 0x2A29: // Manufacturer Name String
            return os_mbuf_append(ctxt->om, "Zephyr", strlen("Zephyr")) == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        case 0x2A50: // PnP ID
            return os_mbuf_append(ctxt->om, pnp_id, sizeof(pnp_id)) == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        default:
            return BLE_ATT_ERR_UNLIKELY;
    }
}

// Characteristic descriptors for Input Report
static const struct ble_gatt_dsc_def mouse_input_dscs[] = {
    {
        .uuid = BLE_UUID16_DECLARE(0x2908), // Report Reference Descriptor
        .att_flags = BLE_ATT_F_READ,
        .access_cb = gatt_svr_dsc_access_report_ref,
    },
    {0} // Terminator
};

// GATT Service Definitions
static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    // Device Information Service (0x180A)
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0x180A),
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                .uuid = BLE_UUID16_DECLARE(0x2A29), // Manufacturer Name
                .access_cb = gatt_svr_chr_access_dis,
                .flags = BLE_GATT_CHR_F_READ,
            },
            {
                .uuid = BLE_UUID16_DECLARE(0x2A50), // PnP ID
                .access_cb = gatt_svr_chr_access_dis,
                .flags = BLE_GATT_CHR_F_READ,
            },
            {0}
        },
    },
    // Human Interface Device Service (0x1812)
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0x1812),
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                .uuid = BLE_UUID16_DECLARE(0x2A4A), // HID Information
                .access_cb = gatt_svr_chr_access_hid,
                .flags = BLE_GATT_CHR_F_READ,
            },
            {
                .uuid = BLE_UUID16_DECLARE(0x2A4B), // Report Map
                .access_cb = gatt_svr_chr_access_hid,
                .flags = BLE_GATT_CHR_F_READ,
            },
            {
                .uuid = BLE_UUID16_DECLARE(0x2A4C), // HID Control Point
                .access_cb = gatt_svr_chr_access_hid,
                .flags = BLE_GATT_CHR_F_WRITE_NO_RSP,
            },
            {
                .uuid = BLE_UUID16_DECLARE(0x2A4E), // Protocol Mode
                .access_cb = gatt_svr_chr_access_hid,
                .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE_NO_RSP,
            },
            {
                .uuid = BLE_UUID16_DECLARE(0x2A4D), // Mouse Input Report (Report ID 1)
                .access_cb = gatt_svr_chr_access_hid,
                .val_handle = &mouse_input_handle,
                .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                .descriptors = (struct ble_gatt_dsc_def *)mouse_input_dscs,
            },
            {0}
        },
    },
    {0} // Terminator
};

static void start_advertising(void) {
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;

    memset(&fields, 0, sizeof(fields));
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.appearance = 0x03C2; // HID Mouse Appearance (identical to Arduino spike)
    fields.appearance_is_present = 1;

    const char *device_name = ble_svc_gap_device_name();
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;

    ble_uuid16_t hid_uuid = BLE_UUID16_INIT(0x1812);
    fields.uuids16 = &hid_uuid;
    fields.num_uuids16 = 1;
    fields.uuids16_is_complete = 1;

    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        ESP_LOGE(TAG, "Error setting adv fields: %d", rc);
        return;
    }

    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &adv_params, NULL, NULL);
    if (rc != 0) {
        ESP_LOGE(TAG, "Error starting advertising: %d", rc);
        return;
    }
    ESP_LOGI(TAG, "Advertising started as HID Mouse");
}

static void bleprph_on_sync(void) {
    ESP_LOGI(TAG, "NimBLE host synced, starting advertising");
    start_advertising();
}

static void bleprph_on_reset(int reason) {
    ESP_LOGE(TAG, "NimBLE host reset; reason = %d", reason);
}

void bleprph_host_task(void *param) {
    ESP_LOGI(TAG, "BLE Host Task Started");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Initializing NimBLE port");
    ESP_ERROR_CHECK(nimble_port_init());

    ble_hs_cfg.sync_cb = bleprph_on_sync;
    ble_hs_cfg.reset_cb = bleprph_on_reset;

    ble_svc_gap_init();
    ble_svc_gatt_init();

    ble_svc_gap_device_name_set("Zephyr-Spike-ESPIDF");

    int rc = ble_gatts_count_cfg(gatt_svr_svcs);
    ESP_ERROR_CHECK(rc == 0 ? ESP_OK : ESP_FAIL);

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    ESP_ERROR_CHECK(rc == 0 ? ESP_OK : ESP_FAIL);

    nimble_port_freertos_init(bleprph_host_task);

    ESP_LOGI(TAG, "Spike ESP-IDF + NimBLE HID Mouse initialized successfully");
}
