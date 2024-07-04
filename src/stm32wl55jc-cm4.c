
#include <stddef.h>
#include <stdbool.h>

#include "stm32wl55jc-cm4.h"


static void setRegisterBits(volatile uint32_t* reg, uint32_t bits) {
    *reg |= bits;
}

GpioA* takeGpioA() {
    if (!gpioa_taken) {
        gpioa_taken = true;
        return GPIOA_MEM_ADDR;
    } else {
        return NULL;
    }
}

GpioB* takeGpioB() {
    if (!gpiob_taken) {
        gpiob_taken = true;
        return GPIOB_MEM_ADDR;
    } else {
        return NULL;
    }
}

Rcc* takeRcc() {
    if (!rcc_taken) {
        rcc_taken = true;
        return RCC_MEM_ADDR;
    } else {
        return NULL;
    }
}

int rccSetRegisterBits(Rcc* rcc, RccRegister reg, uint32_t bits) {
    switch (reg) {
        case AHB2ENR:
            if ((bits & ~(GPIOAEN | GPIOBEN | GPIOCEN | GPIOHEN)) != 0) {
                return -1;
            }
            setRegisterBits(&rcc->AHB2ENR, bits);
            break;
    }
    return 0;
}


// void unset_reg_bits(uint32_t* reg, uint32_t bits) {}
// write_reg_bits()


