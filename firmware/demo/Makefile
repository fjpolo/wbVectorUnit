BUILD_DIR?=../build/ # This default will be overridden by command line

# Corrected include paths:
SOC_DIRECTORY = ${PWD}/../../litex/litex/litex/soc/software/
BUILD_DIR = ${PWD}/../../litex/litex-boards/litex_boards/targets/build/sipeed_tang_nano_20k/
LIBBASE_DIRECTORY = ${PWD}/../../litex/litex/litex/soc/software/libbase/
include ${PWD}/../../litex/litex-boards/litex_boards/targets/build/sipeed_tang_nano_20k/software/include/generated/variables.mak
include ${PWD}/../../litex/litex/litex/soc/software/common.mak

OBJECTS  = donut.o helloc.o crt0.o  vector_test.o main.o
ifdef WITH_CXX
    OBJECTS += hellocpp.o
    CFLAGS += -DWITH_CXX
endif

all: demo.bin


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

vpath %.a $(PACKAGES:%=../%)

demo.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -T linker.ld -N -o $@ \
		$(OBJECTS) \
		$(PACKAGES:%=-L$(BUILD_DIR)/software/%) \
		-Wl,--whole-archive \
		-Wl,--gc-sections \
		-Wl,-Map,$@.map \
		$(LIBS:lib%=-l%)

ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

donut.o: CFLAGS  += -w

# This VPATH is for finding source files that are NOT in the current directory.
# It relies on BIOS_DIRECTORY and CPU_DIRECTORY being defined in variables.mak.
# If all your needed .c/.S files are in firmware/demo, this might not be strictly needed for them,
# but it's crucial for any LiteX common libraries.
VPATH = $(BIOS_DIRECTORY):$(BIOS_DIRECTORY)/cmds:$(CPU_DIRECTORY)


%.o: %.cpp
	$(compilexx)

%.o: %.c
	$(compile)

%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) demo.elf demo.bin .*~ *~

.PHONY: all clean