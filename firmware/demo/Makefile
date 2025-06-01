# Define the root of your primary LiteX framework installation.
# This path is where common.mak was found in previous steps.
LITEX_CORE_ROOT = $(HOME)/Workspace/wbVectorUnit/litex/litex

# Define the root of your specific SoC's build directory.
# This is where generated headers, BIOS, and picolibc sources are.
# This path is relative to where your `sipeed_tang_nano_20k.py` builds.
LITEX_SOC_BUILD_DIR = $(HOME)/Workspace/wbVectorUnit/litex/litex-boards/litex_boards/targets/build/sipeed_tang_nano_20k

# Include common LiteX software variables and rules FIRST.
# This will set defaults for CC, LD, OBJCOPY etc.
include $(LITEX_CORE_ROOT)/litex/soc/software/common.mak

# Override the compiler variables to ensure they are correct.
# The 'override' directive ensures these definitions take precedence,
# even over definitions from included Makefiles.
override CROSS_COMPILE = riscv64-unknown-elf-
override CC = $(CROSS_COMPILE)gcc
override LD = $(CROSS_COMPILE)ld
override OBJCOPY = $(CROSS_COMPILE)objcopy

$(info DEBUG: CC is set to "$(CC)") # Keep this for debugging

# Application-specific settings
TARGET = demo
SRCS = main.c vector_test.c

# Object files (automatically generated from SRCS)
OBJS = $(SRCS:.c=.o)

# Define ALL CFLAGS, overriding any set by common.mak if necessary.
# This ensures only the paths we specify are used.
override CFLAGS = \
	-nostdlib -Os \
	-march=rv32i -mabi=ilp32 \
	-Wall -Werror \
	-g3 -no-pie -fomit-frame-pointer -fno-builtin -fno-stack-protector \
	-fexceptions -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes \
	-I$(LITEX_SOC_BUILD_DIR)/software/include \
	-I$(LITEX_SOC_BUILD_DIR)/software/include/generated \
	-I$(LITEX_SOC_BUILD_DIR)/software/bios/include \
	-I/home/fpolomimi/Workspace/wbVectorUnit/litex/litex/litex/soc/software/include \
	-I/home/fpolomimi/Workspace/wbVectorUnit/litex/litex/litex/soc/software \
	-I/home/fpolomimi/Workspace/wbVectorUnit/litex/litex/litex/soc/cores/cpu/serv \
	-I$(LITEX_SOC_BUILD_DIR)/software/picolibc_src/newlib/libc/include \
	-I$(LITEX_SOC_BUILD_DIR)/software/picolibc_src/newlib/libc/tinystdio \
	-I$(LITEX_SOC_BUILD_DIR)/software/libc

# Linker script: Use the BIOS linker script, which places the application in RAM
LDSCRIPT = $(LITEX_SOC_BUILD_DIR)/software/bios/linker.ld

# LDFLAGS: Link against the LiteX BIOS library and other necessary components
LDFLAGS += \
	-L$(LITEX_SOC_BUILD_DIR)/software/bios \
	-l$(shell basename $(LITEX_SOC_BUILD_DIR)/software/bios/bios) \
	-Wl,--gc-sections \
	-Wl,-Map=$(TARGET).elf.map \
	-T $(LDSCRIPT)

.PHONY: all clean

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET).elf $(TARGET).bin $(OBJS) $(TARGET).elf.map