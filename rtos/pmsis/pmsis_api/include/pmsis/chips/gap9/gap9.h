/*
 * Copyright (C) 2020 GreenWaves Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __PMSIS_CHIPS_GAP9_GAP9_H__
#define __PMSIS_CHIPS_GAP9_GAP9_H__

typedef enum {
  PI_FREQ_DOMAIN_FC     = 1,
  PI_FREQ_DOMAIN_CL     = 2,
  PI_FREQ_DOMAIN_PERIPH = 0,
  PI_FREQ_DOMAIN_SFU    = 3,
} pi_freq_domain_e;


#ifdef __cplusplus
extern "C" {
#endif

void pi_soc_idle_us(int count);

void pi_l2_ret_ctrl(void *chunk, int size, int retentive);

#ifdef __cplusplus
}
#endif
#endif
