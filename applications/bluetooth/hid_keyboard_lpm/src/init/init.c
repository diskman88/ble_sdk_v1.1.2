#include <yoc_config.h>
#include <stdbool.h>
#include <aos/aos.h>
#include <yoc/yoc.h>
#include <pinmux.h>
#include <mm.h>
#include <umm_heap.h>
#include <common.h>
#include <pwrmgr.h>
#include <yoc/init.h>
#include <pm.h>
#include <dw_gpio.h>
#include <drv_usart.h>
#include <pinmux.h>
#include "keyboard.h"



void at_server_init(utask_t *task);
void board_cli_init(utask_t *task);

#define TAG  "init"

#ifndef CONSOLE_ID
#define CONSOLE_ID 0
#endif

extern aos_sem_t g_wait_first_trigger;
extern keyboard g_key;

static utask_t cli_task;
static uint8_t cli_task_stack[2 * 1024];
UTASK_QUEUE_BUF_DEFINE(cli_task_queue, QUEUE_MSG_COUNT * 2);


static uint32_t mm_heap[2 * 1024 / 4]  __attribute__((section("noretention_mem_area0")));
static void mm_init()
{
    mm_initialize(&g_mmheap, mm_heap, sizeof(mm_heap));
}



int pm_prepare_sleep_action()
{
    hal_ret_sram_enable(RET_SRAM0 | RET_SRAM1 | RET_SRAM2);
    csi_gpio_prepare_sleep_action();
    csi_pinmux_prepare_sleep_action();
    csi_usart_prepare_sleep_action(0);
    return 0;
}



int pm_after_sleep_action()
{
    csi_gpio_wakeup_sleep_action();

    csi_pinmux_wakeup_sleep_action();

    csi_usart_wakeup_sleep_action(0);

    if(!g_key.press_num) {
        aos_sem_signal(&g_wait_first_trigger);
    }

    return 0;
}


void board_yoc_init(void)
{
    int ret = 0;

#ifdef CONFIG_WDT
    extern void wdt_init(void);
    wdt_init();
#endif

    /* disable low power mode when use console */

    drv_pinmux_config(P9, UART_TX);
    drv_pinmux_config(P10, UART_RX);

    console_init(CONSOLE_ID, 115200, 128);

    mm_init();

    LOGI(TAG, "Build:%s,%s", __DATE__, __TIME__);

    /* load partition */
    ret = partition_init();

    if (ret <= 0) {
        LOGE(TAG, "partition init failed");
    } else {
        LOGI(TAG, "find %d partitions", ret);
    }

    ret = aos_kv_init("kv");

    if (ret < 0) {
        LOGE(TAG, "KV init failed - %d", ret);
    }

    extern void board_ble_init(void);
    board_ble_init();

	pm_init();

    /* uService init */
    utask_new_ext(&cli_task, "at&cli", cli_task_stack, 2 * 1024, cli_task_queue, QUEUE_MSG_COUNT * 2, AOS_DEFAULT_APP_PRI);

    board_cli_init(&cli_task);
}
