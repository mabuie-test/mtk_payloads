/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2025-2026 Shomy
 */

#ifndef SEJ_SGPT_H
#define SEJ_SGPT_H

#include <types.h>

void sej_gpt1_init(bool start);
u32 sej_gpt1_get_tick(void);
bool sej_gpt1_timeout_tick(u32 start_tick, u32 timeout_tick);
u32 sej_gpt1_tick2time_us(u32 tick);
u32 sej_gpt1_tick2time_ms(u32 tick);
u32 sej_gpt1_time2tick_us(u32 time_us);
u32 sej_gpt1_time2tick_ms(u32 time_ms);
void sej_gpt1_busy_wait_us(u32 time_us);
void sej_gpt1_busy_wait_ms(u32 time_ms);
void sej_mdelay(u32 time_ms);
void sej_udelay(u32 time_us);

#endif // SEJ_SGPT_H
