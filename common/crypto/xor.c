/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2025-2026 Shomy
 */

#include <crypto/xor.h>

void xor_buf(const u8* buf, const u8* key, u32 size, u8* out) {
    for (u32 i = 0; i < size; i++) {
        out[i] = buf[i] ^ key[i];
    }
}
