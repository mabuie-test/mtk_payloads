/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * SPDX-FileCopyrightText: 2025-2026 Shomy
 */

#include <stdint.h>
#include <stddef.h>

#define STATUS_OK 0x00000000
#define EXT_ADDR ((uintptr_t)0x68000000)
#define XML_CMD "<?xml version=\"1.0\" encoding=\"utf-8\"?><host><version>1.0</version>" \
                "<command>CMD:DOWNLOAD-FILE</command><arg><checksum>CHK_NO</checksum>" \
                "<info>ext</info><source_file>ext</source_file>" \
                "<packet_length>0x200000</packet_length></arg></host>"

typedef struct com_channel_struct com_channel_struct;

struct com_channel_struct {
    int (*read)(uint8_t* buffer, uint32_t* length);
    int (*write)(uint8_t* buffer, uint32_t length);
    int (*log_to_pc)(const uint8_t* buffer, uint32_t length);
    int (*log_to_uart)(const uint8_t* buffer, uint32_t length);
};


__attribute__((section(".text.start"))) int cmd_boot_to(struct com_channel_struct* channel, const char* xml) {
    char buf[16];
    uint32_t xfered = 0, total_length = 0, chunk;

    channel->write((uint8_t *)XML_CMD, sizeof(XML_CMD));

    chunk = 16; channel->read((uint8_t *)buf, &chunk);
    chunk = 16; channel->read((uint8_t *)buf, &chunk);

    for (char *p = buf + 3; *p >= '0';) {
        total_length = total_length * 10 + (*p++ - '0');
    }

    goto entry;
    while (xfered < total_length) {
        channel->read((uint8_t *)(EXT_ADDR + xfered), &chunk);
        xfered += chunk;
entry:
        channel->write((uint8_t *)"OK", 3);
        if (xfered >= total_length) break;

        chunk = 16;
        channel->read((uint8_t *)buf, &chunk);
        channel->write((uint8_t *)"OK", 3);

        chunk = total_length - xfered;
        if (chunk > 0x200000) chunk = 0x200000;
    }

    int status = STATUS_OK;
    ((void(*)(void*))EXT_ADDR)(&status);
    return status;
}
