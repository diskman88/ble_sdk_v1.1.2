/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */

#include <yoc_config.h>
#include <aos/log.h>
#include <aos/kernel.h>
#include <aos/ble.h>
#include <app_init.h>
#include <yoc/partition.h>
#include <yoc/ota_server.h>

#define TAG "DEMO"
#define DEVICE_NAME "YoC OTA"
#define DEVICE_ADDR                                                                                \
    {                                                                                              \
        0xCC, 0x3B, 0x13, 0x88, 0xBA, 0xC0                                                         \
    }

#define EVENT_DISCONN 0x00000001
#define EVENT_OTA     0x00000002

static aos_event_t sync_event;

static int start_adv()
{
    ad_data_t ad[2] = {0};

    uint8_t flag = AD_FLAG_GENERAL | AD_FLAG_NO_BREDR;
    ad[0].type   = AD_DATA_TYPE_FLAGS;
    ad[0].data   = (uint8_t *)&flag;
    ad[0].len    = 1;

    ad[1].type = AD_DATA_TYPE_NAME_COMPLETE;
    ad[1].data = (uint8_t *)DEVICE_NAME;
    ad[1].len  = strlen(DEVICE_NAME);

    adv_param_t param = {
        ADV_IND, ad, NULL, BLE_ARRAY_NUM(ad), 0, ADV_FAST_INT_MIN_1, ADV_FAST_INT_MAX_1,
    };

    int ret = ble_stack_adv_start(&param);

    if (ret) {
        LOGE(TAG, "adv start fail %d!", ret);
    } else {
        LOGI(TAG, "adv start!");
        led_set_status(BLINK_FAST);
    }

    return 0;
}

void conn_change(ble_event_en event, void *event_data)
{
    evt_data_gap_conn_change_t *e = (evt_data_gap_conn_change_t *)event_data;

    if (e->connected == CONNECTED) {
        LOGI(TAG, "Connected");
        led_set_status(BLINK_SLOW);
    } else {
        LOGI(TAG, "Disconnected %d",e->err);
        led_set_status(BLINK_FAST);
        aos_event_set(&sync_event, EVENT_DISCONN, AOS_EVENT_OR);
    }
}

static int event_callback(ble_event_en event, void *event_data)
{
    switch (event) {
    case EVENT_GAP_CONN_CHANGE:
        conn_change(event, event_data);
        break;

    default:
        break;
    }

    return 0;
}

static ble_event_cb_t ble_cb = {
    .callback = event_callback,
};

static void ota_event(ota_state_en ota_state)
{
    aos_event_set(&sync_event, EVENT_OTA, AOS_EVENT_OR);
}

int app_main(int argc, char *argv[])
{
    int ret = 0;
    unsigned int event_flags;

    dev_addr_t   addr = {DEV_ADDR_LE_RANDOM, DEVICE_ADDR};
    init_param_t init = {
        .dev_name     = DEVICE_NAME,
        .dev_addr     = &addr,
        .conn_num_max = 1,
    };

    board_yoc_init();

    LOGI(TAG, "bluetooth YoC OTA demo!");

    ble_stack_init(&init);

    ble_ota_init(ota_event);

    ble_stack_setting_load();

    ble_stack_event_register(&ble_cb);

    start_adv();

    aos_event_new(&sync_event, 0);

    while (1) {

        ret = aos_event_get(&sync_event, 0xFFFFFFFF, AOS_EVENT_OR_CLEAR, &event_flags, AOS_WAIT_FOREVER);

        if (ret != 0) {
            LOGE(TAG, "aos_event_get error = %d", ret);
        }

        if ((event_flags & EVENT_DISCONN) == EVENT_DISCONN) {
            start_adv();
        }

        if ((event_flags & EVENT_OTA) == EVENT_OTA) {
            ble_ota_process();
        }

    }

    return 0;
}
