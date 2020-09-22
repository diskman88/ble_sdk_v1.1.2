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
#include <drv_adc.h>
#include <adc.h>
#include <pin_name.h>
#include "pins.h"
#include "pinmux.h"

int app_main(int argc, char *argv[])
{
    board_yoc_init();

    printf("diff adc read %d\r\n:", drv_adc_diff_read(ADC_CH3DIFF));

    return 0;
}
