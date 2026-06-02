/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Shomy
 */

#ifndef DA_XML_DA_H
#define DA_XML_DA_H

#include <types.h>

typedef enum {
    STORAGE_UNKNOWN=0,
    STORAGE_EMMC=1,
    STORAGE_UFS=0x30,
} storage_type;

typedef struct {
    u32 sej_base;
    u32 tzcc_base;
    u32 da2_addr;
    u32 da2_size;
    storage_type storage;
    u32 usb_log;
} da_ctx_t;


typedef struct {
    u64 start;
    u64 length;
} address_range_t;

typedef struct {
    u32 start_sector;
    u32 sector_count;
} storage_range_t;

extern volatile da_ctx_t g_da_ctx;

#endif //DA_XML_DA_H
