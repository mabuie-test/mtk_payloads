/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2025-2026 Shomy
 */

#include <security/sej.h>
#include <mmio.h>
#include <debug.h>

#define GPT_CLK_SYS           (0)
#define GPT_CLK_RTC           (1)

#define GPT_CLK_DIV1          (0)

#define GPT_EN                (1 << 0)
#define GPT_FREERUN           (3 << 4)


#define GPT_MAX_US_TIMEOUT ((u32)330382100)
#define GPT_MAX_MS_TIMEOUT ((u32)330382)

#define GPT_1US_TICK       ((u32)13)
#define GPT_1MS_TICK       ((u32)13000)

#define TIME_TO_TICK_US(us) ((us)*GPT_1US_TICK + ((us)*0 + (1000-1))/1000)
#define TIME_TO_TICK_MS(ms) ((ms)*GPT_1MS_TICK + ((ms)*520 + (1000-1))/1000)


static void sej_gpt1_stop(void) {
    writel(0, SEJ_SGPT_CON); // Disable
    writel(2, SEJ_SGPT_CON); // Clear
}

static void sej_gpt1_start(void) {
    writel(GPT_CLK_SYS | GPT_CLK_DIV1, SEJ_SGPT_CLK);
    writel(GPT_EN | GPT_FREERUN, SEJ_SGPT_CON);
}



void sej_gpt1_init(bool start) {
    sej_gpt1_stop();

    if (start)
        sej_gpt1_start();
    //writel(0, SEJ_SGPT_CON); // Disable
    //writel(2, SEJ_SGPT_CON); // Clear
    //
    //writel(0, SEJ_SGPT_CLK);
    //
    //writel(0x30, SEJ_SGPT_CON);
    //writel(0x31, SEJ_SGPT_CON);
}

u32 sej_gpt1_get_tick(void)
{
    return readl_relaxed(SEJ_SGPT_CNTL);
}


bool sej_gpt1_timeout_tick(u32 start_tick, u32 timeout_tick) {
    register u32 cur_tick;
    register u32 elapsed_tick;

    cur_tick = sej_gpt1_get_tick();

    if (cur_tick >= start_tick)
        elapsed_tick = cur_tick - start_tick;
    else
        elapsed_tick = (0xFFFFFFFF - start_tick) + cur_tick;

    return elapsed_tick >= timeout_tick;
}

u32 sej_gpt1_tick2time_us(u32 tick) {
    return (tick * 1000 + (GPT_1US_TICK - 1)) / GPT_1US_TICK;
}

u32 sej_gpt1_tick2time_ms(u32 tick) {
    return (tick * 1000 + (GPT_1MS_TICK - 1)) / GPT_1MS_TICK;
}

u32 sej_gpt1_time2tick_us(u32 time_us) {
    if (time_us > GPT_MAX_US_TIMEOUT)
        return GPT_MAX_US_TIMEOUT;

    return TIME_TO_TICK_US(time_us);
}

u32 sej_gpt1_time2tick_ms(u32 time_ms) {
    if (time_ms > GPT_MAX_MS_TIMEOUT)
        return GPT_MAX_MS_TIMEOUT;

    return TIME_TO_TICK_MS(time_ms);
}

void sej_gpt1_busy_wait_us(u32 time_us) {
    u32 start_tick = sej_gpt1_get_tick();
    u32 timeout_tick = sej_gpt1_time2tick_us(time_us);

    while (!sej_gpt1_timeout_tick(start_tick, timeout_tick));
}

void sej_gpt1_busy_wait_ms(u32 time_ms) {
    u32 start_tick = sej_gpt1_get_tick();
    u32 timeout_tick = sej_gpt1_time2tick_ms(time_ms);

    while (!sej_gpt1_timeout_tick(start_tick, timeout_tick));
}

void sej_mdelay(u32 ms) {
    sej_gpt1_busy_wait_ms(ms);
}

void sej_udelay(u32 us) {
    sej_gpt1_busy_wait_us(us);
}
