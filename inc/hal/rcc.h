#ifndef RCC_H
#define RCC_H

#ifdef HARDWARE

#if HARDWARE == STM32WL55JC
    #include "hardware/stm32wl55jc.h"
#endif

#endif

#include <stdint.h>


enum {
    // AHB2ENR bits
    RCC_AHB2ENR_BIT_GPIOAEN         = 1 << 0,
    RCC_AHB2ENR_BIT_GPIOBEN         = 1 << 1,
    RCC_AHB2ENR_BIT_GPIOCEN         = 1 << 2,
    RCC_AHB2ENR_BIT_GPIOHEN         = 1 << 7,

    // APB1ENR2 bits
    RCC_APB1ENR2_BIT_LPUART1EN      = 1 << 0,
    RCC_APB1ENR2_BIT_LPTIM2EN       = 1 << 5,
    RCC_APB1ENR2_BIT_LPTIM3EN       = 1 << 6,

    // APB2ENR bits
    RCC_APB2ENR2_BIT_ADCEN          = 1 << 9,
    RCC_APB2ENR2_BIT_TIM1EN         = 1 << 11,
    RCC_APB2ENR2_BIT_SPI1EN         = 1 << 12,
    RCC_APB2ENR2_BIT_USART1EN       = 1 << 14,
    RCC_APB2ENR2_BIT_TIM16EN        = 1 << 17,
    RCC_APB2ENR2_BIT_TIM17EN        = 1 << 18,
};

void rcc_gpio_ck_enable(uint32_t bits);
void rcc_lp_peripheral_ck_enable(uint32_t bits);

#endif
