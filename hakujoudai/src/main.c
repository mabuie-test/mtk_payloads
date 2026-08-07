/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Shomy, R0rt1z2
 */

#include <types.h>
#include <libc.h>
#include <pointers.h>
#include <debug.h>
#include <heap.h>
#include <logo.h>
#include <commands.h>

__attribute__((section(".text.start"))) void main(void)
{

    // Initialize the pointer table, so we can use
    // printf, fix the heap and go back the command loop
    if (init_pointers() != 0) {
        return;
    }

    // Set up the printf callbacks so the log gets dispatched
    // to UART
    printf_register_cb(uart_putc);

    printf("%s", banner);

    heap_dump(heap_struct);
    printf("\n");
    heap_fix(heap_struct);
    printf("\n");
    heap_dump(heap_struct);
    printf("\n");

    register_major_command("CMD:EXP-CALL-FUNC", "1", cmd_call_function);
    register_major_command("CMD:EXP-PATCH-MEM", "1", cmd_patch_mem);

    dagent_command_loop2();
}
