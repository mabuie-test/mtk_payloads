/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Shomy
 */

#include <stdint.h>
#include <stddef.h>

#define STATUS_OK 0x00000000

typedef struct com_channel_struct com_channel_struct;

struct com_channel_struct {
    int (*read)(uint8_t* buffer, uint32_t* length);
    int (*write)(uint8_t* buffer, uint32_t length);
    int (*log_to_pc)(const uint8_t* buffer, uint32_t length);
    int (*log_to_uart)(const uint8_t* buffer, uint32_t length);
};

typedef int (*fp_read_host_file_t)(struct com_channel_struct *channel, const char *file_name,
                                    char **ppdata, uint32_t *pdata_len, const char *info);

volatile uint32_t magic_download = 0x53434d44;
volatile uint32_t magic_sec_cmds = 0x53434d45;

__attribute__((section(".text.start"))) int cmd_security_set_remote_sec_policy(struct com_channel_struct* channel, const char* xml) {
    (void)xml;

    char* dest = (char*)0x200000;
    uint32_t len = 0x8000;

    ((fp_read_host_file_t)(uintptr_t)magic_download)(channel, "sla", &dest, &len, "sla");
    ((void (*)(void))(uintptr_t)magic_sec_cmds)();

    return STATUS_OK;
}
