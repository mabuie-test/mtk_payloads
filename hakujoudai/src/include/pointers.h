/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Shomy
 */

#pragma once

#include <types.h>
#include <commands.h>
#include <drivers/uart.h>

extern void (*volatile register_major_command)(const char *, const char *, HHANDLE);
extern void (*volatile dagent_command_loop2)(void);
extern volatile uptr heap_struct;

extern int (*volatile download)(struct com_channel_struct *, const char *, char **, uint32_t *, const char *);

#define PTR_TABLE_MAGIC 0x554B4148

typedef struct {
    u32 magic;
    u32 uart_base;
    u32 register_major_command;
    u32 dagent_command_loop2;
    u32 heap_struct;
} pointer_table_t;

void (*volatile register_major_command)(const char *, const char *, HHANDLE);
void (*volatile dagent_command_loop2)(void);
volatile uptr heap_struct;

__attribute__((used, section(".pointer_table")))
volatile pointer_table_t PTR_TABLE = {
    .magic = PTR_TABLE_MAGIC,

    .uart_base                = 0x00000000,
    .register_major_command   = 0x00000000,
    .dagent_command_loop2     = 0x00000000,
    .heap_struct              = 0x00000000,
};

u32 init_pointers(void) {
    if (PTR_TABLE.magic != PTR_TABLE_MAGIC)
        return 1;

    mtk_uart_set_base(PTR_TABLE.uart_base);
    register_major_command  = (void *)(uptr)PTR_TABLE.register_major_command;
    dagent_command_loop2     = (void *)(uptr)PTR_TABLE.dagent_command_loop2;
    heap_struct              = (uptr)PTR_TABLE.heap_struct;

    return 0;
}
