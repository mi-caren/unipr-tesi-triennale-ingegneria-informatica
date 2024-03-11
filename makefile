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

#Linker flags
LDFLAGS = -Tlink.ld -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map -Wl,--print-memory-usage
# LDFLAGS += -nostdlib


OPENOCD_INTERFACE = interface/stlink.cfg
OPENOCD_TARGET = target/stm32wlx.cfg

OPENOCD_TRANSPORT = -c 'transport select hla_swd'

OPENOCD_TRANSPORT = -c "init"
OPENOCD_FALSH_CMDS += -c "reset halt"
OPENOCD_FALSH_CMDS += -c "sleep 10"
OPENOCD_FALSH_CMDS += -c "flash write_image erase firmware.elf"
OPENOCD_FALSH_CMDS += -c "reset"
OPENOCD_FALSH_CMDS += -c "shutdown"


# DEFINE HARDWARE
DEF = -imacros inc/hal/hardware/mcu.h -D HARDWARE=STM32WL55JC
# Include Flags
INC = -I. -Iinc

CFLAGS += $(DEF)
CFLAGS += $(INC)


include inc/hal/core/makefile
include inc/hal/communication/makefile


MAIN_SRCS = $(shell find src -name '*.c')
OBJS = $(patsubst src/%.c,bin/%.o,$(MAIN_SRCS))

OBJS += $(LIB_HAL_CORE_OBJS)
OBJS += $(LIB_HAL_COMMUNICATION_OBJS)


build: bin/firmware.bin

flash: build
	openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) $(OPENOCD_TRANSPORT) $(OPENOCD_FALSH_CMDS)

bin/firmware.bin: bin/firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

bin/firmware.elf: $(OBJS)
	@echo "Linking..."
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	@echo

bin/%.o: src/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo

test:
	@echo "srcs: $(MAIN_SRCS) $(LIB_HAL_CORE_SRCS) $(LIB_HAL_COMMUNICATION_SRCS)"
	@echo "objs: $(OBJS)"

log:
	cu -l /dev/ttyACM0 -s 115200 -t

clean:
	rm -f bin/*
