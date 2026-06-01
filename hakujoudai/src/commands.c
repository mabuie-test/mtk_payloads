/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Shomy, R0rt1z2
 */

#include <types.h>
#include <libc.h>
#include <xml.h>
#include <heap.h>
#include <commands.h>
#include <debug.h>

#define STATUS_OK   0x00000000
#define STATUS_ERR  0xC0010001

#define MXML_TYPE_ELEMENT  1
#define MXML_TYPE_TEXT     2
#define MXML_TYPE_OPAQUE   3

int (*const volatile download)(struct com_channel_struct *, const char *, char **, uint32_t *, const char *);

int cmd_boot_to(struct com_channel_struct *channel, const char *xml)
{
    (void)xml;
    volatile uintptr_t ext_addr = 0x68000000;
    char *buf = (char *)ext_addr;
    uint32_t len = 0x1000000;

    printf("%s: loading extensions to 0x%lx (max 0x%lx bytes)\n",
           __func__, (unsigned long)ext_addr, (unsigned long)len);

    int status = download(channel, "ext", &buf, &len, "ext");
    if (status != STATUS_OK) {
        printf("%s: download failed: 0x%lx\n", __func__, (unsigned long)status);
        return status;
    }

    printf("%s: scheduling call to 0x%lx\n", __func__, (unsigned long)ext_addr);

    get_cmd_dpc()->cb = (cmd_dpc_cb)ext_addr;
    get_cmd_dpc()->arg = &status;

    return status;
}

int cmd_patch_mem(struct com_channel_struct *channel, const char *xml)
{
    xml_parser_t tree;

    XML_LOAD(tree, xml, "da/arg/address", "da/arg/length", NULL);

    u32 addr = XML_ATOULL(tree, "da/arg/address");
    u32 len  = XML_ATOULL(tree, "da/arg/length");

    printf("%s: patching %lu bytes at 0x%lx\n",
           __func__, addr, addr);

    char *dst = (char *)(uptr)addr;

    /* + 4 or download fails on '*pdata_len <= total_length' */
    u32 size = len + 4;

    int status = download(channel, "mempatch.bin", &dst, &size, "memory patch");
    if (status != STATUS_OK) {
        printf("%s: download failed: 0x%lx\n", __func__, (unsigned long)status);
        return status;
    }

    return STATUS_OK;
}

int cmd_call_function(struct com_channel_struct *channel, const char *xml)
{
    (void)channel;

    xml_parser_t tree;

    XML_LOAD(tree, xml, "da/arg/address", NULL);


    u32 addr = XML_ATOULL(tree, "da/arg/address");

    printf("%s: scheduling call to 0x%lx\n", __func__, addr);

    get_cmd_dpc()->cb = (cmd_dpc_cb)(uptr)addr;

    return STATUS_OK;
}
