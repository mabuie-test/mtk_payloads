/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2025-2026 Shomy
 */

#include <string.h>
#include <types.h>
#include <security/sej.h>
#include <mmio.h>

#define ERR_SEC_CYPHER_KEY_INVALID 0xC0020032
#define ERR_SEC_CYPHER_DATA_UNALIGNED 0xC0020033

int sej_do_aes(AES_OPS ops, u8* src, u8* dst, u32 size) {
    if ((((g_sej_ctx.blk_sz - 1) | 0xF) & size) != 0)
        return ERR_SEC_CYPHER_DATA_UNALIGNED;

    AES_IV* iv = &g_sej_ctx.iv;;
    if (g_sej_ctx.use_custom_iv) {
        iv = &g_sej_ctx.custom_iv;
    }

    u32* iv_words = (u32*)iv->vector;
    writel(iv_words[0], SEJ_ACFG0);
    writel(iv_words[1], SEJ_ACFG1);
    writel(iv_words[2], SEJ_ACFG2);
    writel(iv_words[3], SEJ_ACFG3);

    /* Set Encryption / Decryption mode, whil keeping the rest */
    writel((readl(SEJ_ACON) & ~1) | (ops & 1), SEJ_ACON);

    SEJ_V3_Run((volatile u32*)src, size, (volatile u32*)dst);

    return 0;
}

u32 sej_set_mode(AES_MODE mode) {
    u32 val = 0;
    val = readl(SEJ_ACON);
    val &= ~(SEJ_AES_MODE_MASK);
    val |= mode ? SEJ_AES_CBC : 0;
    writel(val, SEJ_ACON);

    if (mode == AES_ECB_MODE) {
        for (int i = 0; i < 16; i++) {
            g_sej_ctx.iv.vector[i] = 0;
        }
    }

    return 0;
}

u32 sej_set_iv(AES_IV* iv) {
    memcpy(g_sej_ctx.iv.vector, iv->vector, 16);

    return 0;
}

u32 sej_set_custom_iv(AES_IV* iv, u32 size) {
    if (size > 16)
        return 1;

    memcpy(g_sej_ctx.custom_iv.vector, iv->vector, size);

    g_sej_ctx.use_custom_iv = true;
    return 0;
}


void sej_clear_custom_iv(void) {
    memset(&g_sej_ctx.custom_iv, 0, 16);
    g_sej_ctx.use_custom_iv = false;
}

u32 sej_set_custom_key(u8* key, u32 size) {
    if (size > SEJ_AES_MAX_KEY_SZ) {
        return 1;
    }

    memset(g_sej_ctx.custom_key, 0, SEJ_AES_MAX_KEY_SZ);
    memcpy(g_sej_ctx.custom_key, key, size);

    return 0;
}

void sej_clear_custom_key(void) {
    memset(g_sej_ctx.custom_key, 0, 32);
}
