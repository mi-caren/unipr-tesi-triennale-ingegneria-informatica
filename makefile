CC = arm-none-eabi-gcc

# Flags are set following advices of: https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags
# Most of other flags are taken from tutorial: https://github.com/cpq/bare-metal-programming-guide

CFLAGS = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
# CFLAGS += -Wpadded			# Check if there are some padded structs. Useful only if I really want to optimize performance by reducing struct size
# CFLAGS += -Wconversion	# Warnings for implicit conversions in arithmetic operations and assignments
CFLAGS += -Wformat=2 -Wformat-truncation
# To identify undefined macros silently evaluating as 0
CFLAGS += -Wundef
# Identify duplicate global variables
CFLAGS += -fno-common

# DEBUF FLAGS
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections

# ARM flags
CFLAGS += -mcpu=cortex-m4 -mthumb

# DEFINE HARDWARE
DEF = -imacros inc/hal/hardware/mcu.h -D HARDWARE=STM32WL55JC

CFLAGS += $(EXTRA_CFLAGS)

#Linker flags
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map -Wl,--print-memory-usage

# Include Flags
INC = -I. -Iinc


OPENOCD_INTERFACE = interface/stlink.cfg
OPENOCD_TARGET = target/stm32wlx.cfg

OPENOCD_TRANSPORT = -c 'transport select hla_swd'

OPENOCD_TRANSPORT = -c "init"
OPENOCD_FALSH_CMDS += -c "reset halt"
OPENOCD_FALSH_CMDS += -c "sleep 10"
OPENOCD_FALSH_CMDS += -c "flash write_image erase firmware.elf"
OPENOCD_FALSH_CMDS += -c "reset"
OPENOCD_FALSH_CMDS += -c "shutdown"


SRCS = $(shell find src inc -name '*.c')

OBJS = $(patsubst src/%.c,bin/%.o,$(SRCS))
OBJS += $(patsubst inc/hal/core/%.c,bin/%.o,$(SRCS))
OBJS += $(patsubst inc/hal/communication/%.c,bin/%.o,$(SRCS))

build: bin/firmware.bin
	echo $(OBJS)

flash: build
	openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) $(OPENOCD_TRANSPORT) $(OPENOCD_FALSH_CMDS)

bin/firmware.bin: bin/firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

bin/firmware.elf: $(OBJS)
	$(CC) $(LDFLAGS) $? -o $@

bin/%.o: $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@


# firmware.elf: $(SOURCES)
# 	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

log:
	cu -l /dev/ttyACM0 -s 115200 -t

clean:
	rm -f bin/*
