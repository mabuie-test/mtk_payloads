/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2025-2026 Shomy
 */

#ifndef KEY_DERIVE_H
#define KEY_DERIVE_H

#include <types.h>
#include <crypto/sbrom/sbrom.h>

typedef enum {
    RPMB_KEY = 0,
    FDE_KEY = 1,
    TEE_KEY = 2,
    AES_IMG_ENC = 3,
    AES_CUSTOM = 4,
    CUSTOM_0 = 5, // Moto
    CUSTOM_1 = 6, // BASE_KEY (not sure for what it is)
    UNKNOWN_0 = 7, // Unknown, from Goya pl
    UNKNOWN_1 = 8, // Unknown too

    INPUT_KEY = 0xFF // Input Key
} KeyType;

typedef struct {
    u32 src_key;
    u8* label;
    u32 label_sz;
    u8* salt;
    u32 salt_sz;
    u32 policy; // 22
} key_attr_t;

u32 key_derive(KeyType key_type, u8 *out, u32 len);
u32 key_derive_input(u8 *label, u32 label_sz, u8 *salt, u32 salt_sz, u8 *out, u32 len);

#endif /* KEY_DERIVE_H */
