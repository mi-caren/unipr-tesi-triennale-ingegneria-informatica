#ifndef __STM32WL55JC_H
#define __STM32WL55JC_H

#include <stdint.h>

#define GPIO_MEM_ADDR		0x48000000

#define GPIOA_OFFSET		0x0000
#define GPIOB_OFFSET		0x0400

#define GPIOA               ( ( struct Gpio * ) ( GPIO_MEM_ADDR + GPIOA_OFFSET ) )
#define GPIOB               ( ( struct Gpio * ) ( GPIO_MEM_ADDR + GPIOB_OFFSET ) )


typedef struct Gpio {
    // configuration registers
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;

    // data registers
    volatile uint32_t IDR;
    volatile uint32_t ODR;

    // set/reset register
    volatile uint32_t BSRR;

    // locking register
    volatile uint32_t LCKR;

    // alternate function registers
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
} Gpio;

#endif