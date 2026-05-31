/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2025-2026 Shomy
 */

#ifndef SEJ_H
#define SEJ_H

#include <types.h>

extern volatile uintptr_t hacc_base;

static inline volatile uint32_t* SEJ_REG(uint32_t offset) {
    return (volatile uint32_t*)(hacc_base + offset);
}

#define SEJ_CG                      (0x1 << 10)

#define HACC_AES_TEST_SRC            (0x02000000)
#define HACC_AES_TEST_TMP            (0x02100000)
#define HACC_AES_TEST_DST            (0x02200000)

#define HACC_CFG_0                    (0x5a5a3257)	/* CHECKME */
#define HACC_CFG_1                    (0x66975412)	/* CHECKME */
#define HACC_CFG_2                    (0x66975412)	/* CHECKME */
#define HACC_CFG_3                    (0x5a5a3257)	/* CHECKME */

#define SEJ_CON                     SEJ_REG(0x0000)
#define SEJ_ACON                    SEJ_REG(0x0004)
#define SEJ_ACON2                   SEJ_REG(0x0008)
#define SEJ_ACONK                   SEJ_REG(0x000C)
#define SEJ_ASRC0                   SEJ_REG(0x0010)
#define SEJ_ASRC1                   SEJ_REG(0x0014)
#define SEJ_ASRC2                   SEJ_REG(0x0018)
#define SEJ_ASRC3                   SEJ_REG(0x001C)
#define SEJ_AKEY0                   SEJ_REG(0x0020)
#define SEJ_AKEY1                   SEJ_REG(0x0024)
#define SEJ_AKEY2                   SEJ_REG(0x0028)
#define SEJ_AKEY3                   SEJ_REG(0x002C)
#define SEJ_AKEY4                   SEJ_REG(0x0030)
#define SEJ_AKEY5                   SEJ_REG(0x0034)
#define SEJ_AKEY6                   SEJ_REG(0x0038)
#define SEJ_AKEY7                   SEJ_REG(0x003C)
#define SEJ_ACFG0                   SEJ_REG(0x0040)
#define SEJ_ACFG1                   SEJ_REG(0x0044)
#define SEJ_ACFG2                   SEJ_REG(0x0048)
#define SEJ_ACFG3                   SEJ_REG(0x004c)
#define SEJ_AOUT0                   SEJ_REG(0x0050)
#define SEJ_AOUT1                   SEJ_REG(0x0054)
#define SEJ_AOUT2                   SEJ_REG(0x0058)
#define SEJ_AOUT3                   SEJ_REG(0x005C)
#define SEJ_SW_OTP0                 SEJ_REG(0x0060)
#define SEJ_SW_OTP1                 SEJ_REG(0x0064)
#define SEJ_SW_OTP2                 SEJ_REG(0x0068)
#define SEJ_SW_OTP3                 SEJ_REG(0x006c)
#define SEJ_SW_OTP4                 SEJ_REG(0x0070)
#define SEJ_SW_OTP5                 SEJ_REG(0x0074)
#define SEJ_SW_OTP6                 SEJ_REG(0x0078)
#define SEJ_SW_OTP7                 SEJ_REG(0x007c)
#define SEJ_SECINIT0                SEJ_REG(0x0080)
#define SEJ_SECINIT1                SEJ_REG(0x0084)
#define SEJ_SECINIT2                SEJ_REG(0x0088)
#define SEJ_MKJ                     SEJ_REG(0x00a0)
#define SEJ_SCON                    SEJ_REG(0x00b0)
#define SEJ_SINT                    SEJ_REG(0x00b4)
#define SEJ_SOUT                    SEJ_REG(0x00b8)
#define SEJ_CON1                    SEJ_REG(0x00c0)
#define SEJ_CON1_LOCK               SEJ_REG(0x00c4) // Used in BROM for JTAG, might be debug mode?
#define SEJ_ACONK2                  SEJ_REG(0x00bc)
#define SEJ_RCON                    SEJ_REG(0x0100)
#define SEJ_RCON2                   SEJ_REG(0x0104)
#define SEJ_RNG_IV0                 SEJ_REG(0x0110)
#define SEJ_RNG_IV1                 SEJ_REG(0x0114)
#define SEJ_RNG_IV2                 SEJ_REG(0x0118)
#define SEJ_RNG_IV3                 SEJ_REG(0x011c)
#define SEJ_RNG_OUT0                SEJ_REG(0x0120)
#define SEJ_RNG_OUT1                SEJ_REG(0x0124)
#define SEJ_RNG_OUT2                SEJ_REG(0x0128)
#define SEJ_RNG_OUT3                SEJ_REG(0x012c)
#define SEJ_SGPT_CON                SEJ_REG(0x0220)
#define SEJ_SGPT_CLK                SEJ_REG(0x0224)
#define SEJ_SGPT_CNTL               SEJ_REG(0x0228)
#define SEJ_SGPT_CNTH               SEJ_REG(0x022c)

#define SEJ_SWDT                    SEJ_REG(0x0300)

#define SEJ_DEBUG_SYS_EN            SEJ_REG(0x00d8)
#define SEJ_UNK1                    SEJ_REG(0x00dc) // Debug port lock


/* AES */
#define SEJ_AES_DEC                 0x00000000
#define SEJ_AES_ENC                 0x00000001
#define SEJ_AES_MODE_MASK           0x00000002
#define SEJ_AES_ECB                 0x00000000
#define SEJ_AES_CBC                 0x00000002
#define SEJ_AES_TYPE_MASK           0x00000030
#define SEJ_AES_128                 0x00000000
#define SEJ_AES_192                 0x00000010
#define SEJ_AES_256                 0x00000020
#define SEJ_AES_CHG_BO_MASK         0x00001000
#define SEJ_AES_CHG_BO_OFF          0x00000000 // Change byte order
#define SEJ_AES_CHG_BO_ON           0x00001000
#define SEJ_AES_START               0x00000001
#define SEJ_AES_CLR                 0x00000002
#define SEJ_AES_RDY                 0x00008000
#define SEJ_AES_KDF_START           0x40000000
#define SEJ_KDF_RDY                 0x80000000

/* AES key relevant */
#define SEJ_AES_BK2C                0x00000010
#define SEJ_AES_R2K                 0x00000100
#define SEJ_AES_OLD_BK2C            0x00000001
#define SEJ_HRK_EN                  0x00000002
#define SEJ_HRK_DIS                 0x00000000

/* SECINIT magic */
#define SEJ_SECINIT0_MAGIC          0xAE0ACBEA
#define SEJ_SECINIT1_MAGIC          0xCD957018
#define SEJ_SECINIT2_MAGIC          0x46293911

/* RNG */
#define SEJ_RNG_SRC_EN              0x00000001
#define SEJ_RNG_SRC_IV              0x00000004
#define SEJ_RNG_MOD_DIS             0x00000000
#define SEJ_RNG_MOD_EN              0x00000002
#define SEJ_RNG_START               0x00000001
#define SEJ_RNG_RDY                 0x00008000

/* MISC */
#define SEJ_SWOTP_LOCK              (1 << 4) // CON
#define SEJ_AC_DISABLE              (1 << 5) // CON
#define SEJ_SEC_INIT_LOCK           (1 << 6) // CON
#define SEJ_JTAG_LOCK               (1 << 0) // 0xc4
#define SEJ_DEBUG_LOCK              (1 << 8) // 0xc4

#define SEJ_AES_MAX_KEY_SZ          (32)
#define AES_CFG_SZ                  (16)
#define AES_BLK_SZ                  (16)
#define SEJ_HW_KEY_SZ               (16)

/******************************************************************************
 * TYPE DEFINITIONS
 ******************************************************************************/
typedef enum {
	AES_ECB_MODE,
	AES_CBC_MODE
} AES_MODE;

typedef enum {
	AES_DEC,
	AES_ENC
} AES_OPS;

typedef enum {
	AES_KEY_128 = 0,
	AES_KEY_192 = 1,
	AES_KEY_256 = 2,
} AES_KEY_SZ;

typedef enum {
	AES_SW_KEY,
	AES_HW_KEY,
	AES_HW_WRAP_KEY,
	AES_RID_KEY,
	AES_CUSTOM_KEY
} AES_KEY_ID;

typedef struct {
	unsigned char config[AES_CFG_SZ];
} AES_CFG;

typedef struct {
	unsigned int size;
	unsigned char seed[SEJ_AES_MAX_KEY_SZ];
} AES_KEY_SEED;

enum cryptmode_t {
    SW_ENCRYPTED = 0,
    HW_ENCRYPTED = 1,
    HW_ENCRYPTED_5G = 2,
    UNLOCK = 3
};

struct SymKey {
    uint32_t* key;
    uint8_t key_len;
    uint8_t mode;
    uint32_t* iv;
    uint8_t iv_len;
};

typedef struct {
    uint8_t vector[16];
} AES_IV;

typedef struct {
    AES_IV iv;
    AES_IV custom_iv;
    u32 blk_sz;
    u8 sw_key[SEJ_AES_MAX_KEY_SZ];
    u8 hw_key[SEJ_AES_MAX_KEY_SZ];
    u8 rid_key[SEJ_AES_MAX_KEY_SZ];
    u8 custom_key[SEJ_AES_MAX_KEY_SZ];
    u8 use_custom_iv;
    bool legacy;
} sej_ctx_t;

typedef struct {
    u32 length;

    AES_OPS encrypt;
    bool anti_clone;
    bool xor_en;
    bool legacy;

    AES_MODE mode;
    AES_KEY_ID key_id;
    AES_KEY_SZ key_sz;
    u8 reserved;

} sej_param_t;

extern sej_ctx_t g_sej_ctx;
extern const u32 g_ME_ID_SRC[4];
extern const u32 g_ME_ID_IV[4];
extern const u32 g_ME_ID_SWOTP[8];
extern const u32 g_ME_ID_PATTERN[3][4];
extern const u32 g_AC_FIXED_PATTERN[3][4];
extern const u32 g_AC_BOOTROM_IV[8];
extern const u32 g_HACC_CFG_2[8];
extern const u32 g_HACC_CFG_3[8];
extern const u8 DEFAULT_IV[16];
extern const u8 DEFAULT_KEY[32];

void SEJ_V3_init(AES_OPS encrypt, const u32* iv);
void SEJ_V3_Run(volatile u32* p_src, u32 length, volatile u32* p_dst);
void SEJ_V3_Terminate(void);
void sej_aes_kdf(bool hrk256);
void sej_aes_old_bk2c_key(void);

u32 sej_set_otp(u32* otp);
void sej_clear_otp(void);
u32 sej_set_iv(AES_IV* iv);
u32 sej_set_custom_iv(AES_IV* iv, u32 size);
void sej_clear_custom_iv(void);
u32 sej_set_custom_key(u8* key, u32 size);
void sej_clear_custom_key(void);

int sej_do_aes(AES_OPS ops, u8* src, u8* dst, u32 size);
int sej_set_key(AES_KEY_ID id,AES_KEY_SZ key);
u32 sej_set_mode(AES_MODE mode);
void sej_secinit_set_magic(void);

void sej_get_bootmode(u32* bootmode);

int sp_sej_enc(u8* buf, u8* out, sej_param_t desc);
int sp_sej_dec(u8* buf, u8* out, sej_param_t desc);

u32 sej_prng(u32* iv, u32* out);

void sej_init(uintptr_t base_addr);
void set_sej_base(uintptr_t base_addr);
uintptr_t get_sej_base(void);

#endif // SEJ_H
