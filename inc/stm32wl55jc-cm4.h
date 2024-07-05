#ifndef STM32WL55JC_CM4_H
#define STM32WL55JC_CM4_H

#include <stddef.h>
#include <stdbool.h>

#include "gpiob.h"


typedef struct Peripherals {
    GPIOB* const GPIOB;
} Peripherals;

Peripherals* takePeripherals();
Peripherals* stealPeripherals();

#endif
