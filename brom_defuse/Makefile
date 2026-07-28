CROSS_COMPILE ?= arm-none-eabi-

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
AS := $(CROSS_COMPILE)as
SIZE := $(CROSS_COMPILE)size

SRC := main.c
START := start.S
LINKER := linker.x

BUILDDIR := build
OUTDIR := ../payloads

OUT_ELF := $(BUILDDIR)/secpatcher.elf
OUT_BIN := $(OUTDIR)/secpatcher.bin

CFLAGS := -std=gnu99 -Os -mthumb -mcpu=cortex-a9 -fno-builtin-printf \
          -fno-strict-aliasing -fno-builtin-memcpy -fPIE -mno-unaligned-access \
          -Wall -Wextra
LDFLAGS := -nodefaultlibs -nostdlib -Wl,--build-id=none,--no-warn-rwx-segments

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g
else
CFLAGS += -DNDEBUG
endif

all: $(OUT_BIN)

$(OUT_BIN): $(OUT_ELF)
	mkdir -p $(OUTDIR)
	$(OBJCOPY) -O binary $< $@
	@echo "Binary size: $$(stat -c%s $(OUT_BIN)) bytes"

$(OUT_ELF): $(BUILDDIR)/main.o $(BUILDDIR)/start.o $(LINKER)
	mkdir -p $(BUILDDIR)
	$(LD) -o $@ $(BUILDDIR)/main.o $(BUILDDIR)/start.o $(LDFLAGS) -T $(LINKER)
	$(SIZE) $@

$(BUILDDIR)/main.o: $(SRC)
	mkdir -p $(BUILDDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILDDIR)/start.o: $(START)
	mkdir -p $(BUILDDIR)
	$(AS) -o $@ $<

clean:
	rm -rf $(BUILDDIR)
	rm -f $(OUT_BIN)

.PHONY: all clean
