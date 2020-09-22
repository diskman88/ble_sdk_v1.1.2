/*
 * Copyright (C) 2020 Alibaba Group Holding Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     app_main.c
 * @brief    the main function for the digitron driver
 * @version  V1.0
 * @date     07. June 2020
 ******************************************************************************/
#include <stdio.h>
#include <app_init.h>
#include "pin_name.h"
#include "pins.h"
#include <digitron.h>
#include "aos/kernel.h"

static uint8_t led_pin[7] = {18, 20, 23, 31, 32, 33, 34};
aos_timer_t timer2s;

void _timeout_hander()
{
    digitron_show_off();
}

int app_main(int argc, char *argv[])
{
    int ret = 0;
    board_yoc_init();

    ret = digitron_init(led_pin, sizeof(led_pin));
    if(ret) {
        printf("init digitron faild\r\n");
    }
    printf("digitron show\r\n");
    aos_timer_new(&timer2s, _timeout_hander, NULL, 2000, 0);
    aos_timer_start(&timer2s);
    digitron_show(1234);
    printf("end show\r\n");
    return 0;
}
