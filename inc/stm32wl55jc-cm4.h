#ifndef STM32WL55JC_CM4_H
#define STM32WL55JC_CM4_H

#include <stddef.h>
#include <stdbool.h>

#include "gpio.h"
#include "rcc.h"

typedef struct gpio Gpio;
typedef Gpio GpioA;
typedef Gpio GpioB;

typedef struct rcc Rcc;

#define GPIOA_MEM_ADDR     (GpioA*) 0x48000000
#define GPIOB_MEM_ADDR     (GpioB*) 0x48000400

#define RCC_MEM_ADDR       (Rcc*)   0x58000000

static bool gpioa_taken = false;
static bool gpiob_taken = false;
static bool rcc_taken = false;

typedef enum RccRegister {
    AHB2ENR = 0x04c,
} RccRegister;

typedef enum RccAhb2EnrBits {
    GPIOAEN         = 1 << 0,
    GPIOBEN         = 1 << 1,
    GPIOCEN         = 1 << 2,
    GPIOHEN         = 1 << 7,
} RccAhb2EnrBits;

GpioA* takeGpioA();
GpioB* takeGpioB();
Rcc* takeRcc();

int rccSetRegisterBits(Rcc* rcc, RccRegister reg, uint32_t bits);


// void unset_reg_bits(uint32_t* reg, uint32_t bits) {}
// write_reg_bits()

#endif
