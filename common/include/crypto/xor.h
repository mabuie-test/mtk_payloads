/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2025-2026 Shomy
 */

#ifndef XOR_H
#define XOR_H

#include <types.h>

void xor_buf(const u8* buf, const u8* key, u32 size, u8* out);

#endif // XOR_H
