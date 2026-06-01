/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Shomy
 */

#ifndef DA_X_HAL_H
#define DA_X_HAL_H

#include <types.h>

typedef struct {
    int (*read)(u8* buffer, u32* length);
    int (*write)(u8* buffer, u32 length);
    int (*log_to_pc)(const u8* buffer, u32 length);
    int (*log_to_uart)(const u8* buffer, u32 length);
} com_channel_struct;

typedef int (*HHANDLE)(com_channel_struct*);

typedef struct __attribute__((packed)) pointer_table_t {
    u32 magic;
    u32 uart_base;
    u32 register_device_ctrl;
    u32 malloc;
    u32 free;
    u32 mmc_get_card;
} pointer_table_t;

#endif //DA_X_COM_H
