CC = arm-none-eabi-gcc

# Flags are set following advices of: https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags
# Most of other flags are taken from tutorial: https://github.com/cpq/bare-metal-programming-guide

CFLAGS = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
# CFLAGS += -Wpadded                   # Check if there are some padded structs. Useful only if I really want to optimize performance by reducing struct size
# CFLAGS += -Wconversion       # Warnings for implicit conversions in arithmetic operations and assignments
CFLAGS += -Wformat=2 -Wformat-truncation
CFLAGS += -Wno-unused-value -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
# To identify undefined macros silently evaluating as 0
CFLAGS += -Wundef
# Identify duplicate global variables
CFLAGS += -fno-common

# DEBUF FLAGS
CFLAGS += -g3 -ffunction-sections -fdata-sections

# optimize for size -> this is disabled because generates calls to memeset
# CFLAGS += -Os


# ARM flags
CFLAGS += -mcpu=cortex-m4 -mthumb

CFLAGS += $(EXTRA_CFLAGS)

#Linker flags
LDFLAGS ?= -Tlink.ld -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map -Wl,--print-memory-usage
LDFLAGS += -nostdlib
# Include Flags
INC += -I. -Iinc
SRCS = $(shell find src -name '*.c')
OBJS = $(patsubst src/%.c,bin/%.o,$(SRCS))

OPENOCD_INTERFACE = interface/stlink.cfg
OPENOCD_TARGET = target/stm32wlx.cfg

OPENOCD_TRANSPORT = -c 'transport select hla_swd'

OPENOCD_TRANSPORT = -c "init"
OPENOCD_FALSH_CMDS += -c "reset halt"
OPENOCD_FALSH_CMDS += -c "sleep 10"
OPENOCD_FALSH_CMDS += -c "flash write_image erase bin/firmware.elf"
OPENOCD_FALSH_CMDS += -c "reset"
OPENOCD_FALSH_CMDS += -c "shutdown"


build: bin/firmware.bin
	@echo "Everything built!"

bin/firmware.bin: bin/firmware.elf
	@arm-none-eabi-objcopy -O binary $< $@

bin/firmware.elf: $(OBJS)
	@echo "Linking..."
	@$(CC) $(LDFLAGS) $(OBJS) -o $@

bin/%.o: src/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

flash: bin/firmware.bin
	openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) $(OPENOCD_TRANSPORT) $(OPENOCD_FALSH_CMDS)



log:
	cu -l /dev/ttyACM0 -s 115200 -t

clean:
	rm -f bin/*
