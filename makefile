# Flags are set following advices of: https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags
# Most of other flags are taken from tutorial: https://github.com/cpq/bare-metal-programming-guide

CFLAGS = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
# CFLAGS += -Wpadded			# Check if there are some padded structs. Useful only if I really want to optimize performance by reducing struct size
# CFLAGS += -Wconversion	# Warnings for implicit conversions in arithmetic operations and assignments
CFLAGS += -Wformat=2 -Wformat-truncation
CFLAGS += -Wundef 			# To identify undefined macros silently evaluating as 0
CFLAGS += -fno-common		# Identify duplicate global variables

# DEBUF FLAGS
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections

# Include Flags
CFLAGS += -I. -Iinc

# ARM flags
CFLAGS += -mcpu=cortex-m4 -mthumb

# DEFINE HARDWARE
CFLAGS += -imacros inc/hardware/mcu.h -D HARDWARE=STM32WL55JC

CFLAGS += $(EXTRA_CFLAGS)

#Linker flags
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map -Wl,--print-memory-usage

SOURCES = src/*.c inc/hal/*.c inc/communication/*.c

OPENOCD_INTERFACE = interface/stlink.cfg
OPENOCD_TARGET = target/stm32wlx.cfg

OPENOCD_TRANSPORT = -c 'transport select hla_swd'

OPENOCD_TRANSPORT = -c "init"
OPENOCD_FALSH_CMDS += -c "reset halt"
OPENOCD_FALSH_CMDS += -c "sleep 10"
OPENOCD_FALSH_CMDS += -c "flash write_image erase firmware.elf"
OPENOCD_FALSH_CMDS += -c "reset"
OPENOCD_FALSH_CMDS += -c "shutdown"

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -f
endif


log:
	cu -l /dev/ttyACM0 -s 115200 -t

flash: build
	openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) $(OPENOCD_TRANSPORT) $(OPENOCD_FALSH_CMDS)

build: firmware.bin

firmware.elf: $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

clean:
	$(RM) firmware.*
