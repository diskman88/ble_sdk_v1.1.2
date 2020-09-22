/*
 * Copyright (C) 2017-2019 C-SKY Microsystems Co., Ltd. All rights reserved.
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
 * @file     pin.h
 * @brief    header File for pin definition
 * @version  V1.0
 * @date     15. May 2019
 ******************************************************************************/
#ifndef _PM_H_
#define _PM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void enableSleepInPM(uint8_t flag);
void disableSleepInPM(uint8_t flag);
int pm_prepare_sleep_action();
int pm_after_sleep_action();

#ifdef __cplusplus
}
#endif

#endif /* _PIN_H_ */
