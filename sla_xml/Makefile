CROSS_COMPILE    ?= arm-none-eabi-
CROSS_COMPILE_64 ?= aarch64-none-linux-gnu-

CC_ARM      := $(CROSS_COMPILE)gcc
LD_ARM      := $(CROSS_COMPILE)gcc
OBJCOPY_ARM := $(CROSS_COMPILE)objcopy

CC_ARM64      := $(CROSS_COMPILE_64)gcc
LD_ARM64      := $(CROSS_COMPILE_64)gcc
OBJCOPY_ARM64 := $(CROSS_COMPILE_64)objcopy

BUILD_DIR := build
SRC       := main.c

OUT_ARM   := ../payloads/sla_xml_arm
OUT_ARM64 := ../payloads/sla_xml_arm64

COMMON_CFLAGS := -std=gnu99 -Os -fPIE \
	-fno-builtin-printf -fno-builtin-memcpy \
	-fno-strict-aliasing -DPRINTF_DISABLE_SUPPORT_FLOAT=1 \
	-flto -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables \
	-fno-unwind-tables -fomit-frame-pointer -fno-common

CFLAGS_ARM   := $(COMMON_CFLAGS) -marm -mcpu=cortex-a9 -mno-unaligned-access
CFLAGS_ARM64 := $(COMMON_CFLAGS) -march=armv8-a -mcmodel=tiny

COMMON_LDFLAGS := -nodefaultlibs -nostdlib -flto \
    -Wl,--gc-sections -Wl,--build-id=none -Wl,--strip-all \
    -Wl,--no-warn-rwx-segments -Wl,--nmagic

LDFLAGS_ARM   := $(COMMON_LDFLAGS) -T linker.x
LDFLAGS_ARM64 := $(COMMON_LDFLAGS) -T linker64.x

ARM_OBJ   := $(BUILD_DIR)/arm/main.o
ARM_ELF   := $(BUILD_DIR)/arm/sla_xml.elf
ARM_BIN   := $(OUT_ARM).bin

ARM64_OBJ := $(BUILD_DIR)/arm64/main.o
ARM64_ELF := $(BUILD_DIR)/arm64/sla_xml.elf
ARM64_BIN := $(OUT_ARM64).bin

all: arm arm64

arm: $(ARM_BIN)

$(ARM_BIN): $(ARM_ELF)
	$(OBJCOPY_ARM) -O binary $< $@

$(ARM_ELF): $(ARM_OBJ)
	$(LD_ARM) $(LDFLAGS_ARM) -o $@ $^

$(ARM_OBJ): $(SRC)
	mkdir -p $(@D)
	$(CC_ARM) $(CFLAGS_ARM) -c $< -o $@

arm64: $(ARM64_BIN)

$(ARM64_BIN): $(ARM64_ELF)
	$(OBJCOPY_ARM64) -O binary $< $@

$(ARM64_ELF): $(ARM64_OBJ)
	$(LD_ARM64) $(LDFLAGS_ARM64) -o $@ $^

$(ARM64_OBJ): $(SRC)
	mkdir -p $(@D)
	$(CC_ARM64) $(CFLAGS_ARM64) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all arm arm64 clean
