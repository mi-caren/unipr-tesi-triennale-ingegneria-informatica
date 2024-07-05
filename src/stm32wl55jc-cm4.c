
#include <stddef.h>
#include <stdbool.h>

#include "stm32wl55jc-cm4.h"

static bool device_peripherals_taken = false;

static Peripherals device_peripherals = {
    .GPIOB = (GPIOB* const) 0x48000400
};

Peripherals* takePeripherals() {
    if (device_peripherals_taken) {
        return NULL;
    } else {
        return stealPeripherals();
    }
}

Peripherals* stealPeripherals() {
    device_peripherals_taken = true;
    return &device_peripherals;
}
