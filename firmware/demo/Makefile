# Toolchain setup
CROSS_COMPILE = riscv64-unknown-elf-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

# Paths
LITEX_CORE_ROOT = $(HOME)/Workspace/litex_new_clean/litex
LITEX_SOC_BUILD_DIR = $(HOME)/Workspace/litex_new_clean/litex/litex-boards/litex_boards/targets/build/sipeed_tang_nano_20k

# Target and sources
TARGET = demo
SRCS = main.c vector_test.c
OBJS = $(SRCS:.c=.o)

# Use local linker script
LDSCRIPT = linker.ld

# Compiler flags with all include paths
CFLAGS = \
    -nostdlib -Os \
    -march=rv32i -mabi=ilp32 \
    -Wall -Werror \
    -g3 -fomit-frame-pointer \
    -I. \
    -I$(LITEX_SOC_BUILD_DIR)/software/include \
    -I$(LITEX_SOC_BUILD_DIR)/software/include/generated \
    -I$(LITEX_SOC_BUILD_DIR)/software/bios/include \
    -I$(LITEX_CORE_ROOT)/litex/soc/software/include \
    -I$(LITEX_SOC_BUILD_DIR)/software/picolibc_src/newlib/libc/include \
    -I$(LITEX_SOC_BUILD_DIR)/software/picolibc_src/newlib/libc/tinystdio \
    -I$(LITEX_SOC_BUILD_DIR)/software/libc \
	-I$(LITEX_CORE_ROOT)/litex/soc/software/libbase \
	-I$(LITEX_CORE_ROOT)/litex/soc/software \
	-I$(LITEX_CORE_ROOT)/litex/soc/cores/cpu/serv

# Linker flags
LDFLAGS = \
    -nostdlib \
    -Wl,--gc-sections \
    -Wl,-Map=$(TARGET).elf.map \
    -T $(LDSCRIPT)

.PHONY: all clean

all: $(TARGET).bin

$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET).elf $(TARGET).bin $(OBJS) *.map