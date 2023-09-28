CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -Wno-unused-value -fno-common \
            -g3 -Os -ffunction-sections -fdata-sections -I. -Iinc \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = src/*.c

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
