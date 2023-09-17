#ifndef RCC_H
#define RCC_H

#include <stdint.h>


#define RCC_MEM_ADDR        0x58000000

#define RCC                 ( ( struct rcc * ) RCC_MEM_ADDR )


struct rcc {
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t _RESERVED1[2];
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    volatile uint32_t _RESERVED2;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t _RESERVED3;
    volatile uint32_t APB1RSTR1;
    volatile uint32_t APB1RSTR2;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB3RSTR;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t _RESERVED4;
    volatile uint32_t APB1ENR1;
    volatile uint32_t APB1ENR2;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB3ENR;
    volatile uint32_t AHB1SMENR;
    volatile uint32_t AHB2SMENR;
    volatile uint32_t AHB3SMENR;
    volatile uint32_t APB1SMENR1;
    volatile uint32_t APB1SMENR2;
    volatile uint32_t APB2SMENR;
    volatile uint32_t APB3SMENR;
    volatile uint32_t CCIPR;
    volatile uint32_t _RESERVED5;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
};


#endif