#ifndef UART_H
#define UART_H

// #include "main.h"
#include <stdint.h>
#include <stdlib.h>

#define LPUART1_MEM_ADDR        0x40008000

#define LPUART1                 ( ( struct lpuart * ) LPUART1_MEM_ADDR )

#define LPUART_KER_CK_PRES          ( 4000000 ) // 4Mhz

struct lpuart {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t _RESERVED[2];
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
    volatile uint32_t PRESC;
};

enum {
    // CR1 bits
    LPUART_CR1_BIT_UE           = 1 << 0,
    LPUART_CR1_BIT_UESM         = 1 << 1,
    LPUART_CR1_BIT_RE           = 1 << 2,
    LPUART_CR1_BIT_TE           = 1 << 3,
    LPUART_CR1_BIT_IDLEIE       = 1 << 4,
    LPUART_CR1_BIT_RXNEIE       = 1 << 5,
    LPUART_CR1_BIT_TCIE         = 1 << 6,
    LPUART_CR1_BIT_TXEIE        = 1 << 7,
    LPUART_CR1_BIT_PEIE         = 1 << 8,
    LPUART_CR1_BIT_PS           = 1 << 9,
    LPUART_CR1_BIT_PCE          = 1 << 10,
    LPUART_CR1_BIT_WAKE         = 1 << 11,
    LPUART_CR1_BIT_MME          = 1 << 13,
    LPUART_CR1_BIT_CMIE         = 1 << 14,
    LPUART_CR1_BIT_FIFOEN       = 1 << 29,

    // ISR bits
    LPUART_ISR_BIT_PE           = 1 << 0,
    LPUART_ISR_BIT_FE           = 1 << 1,
    LPUART_ISR_BIT_NE           = 1 << 2,
    LPUART_ISR_BIT_ORE          = 1 << 3,
    LPUART_ISR_BIT_IDLE         = 1 << 4,
    LPUART_ISR_BIT_RXNE         = 1 << 5,
    LPUART_ISR_BIT_TC           = 1 << 6,
    LPUART_ISR_BIT_TXE          = 1 << 7,
    LPUART_ISR_BIT_CTSIF        = 1 << 9,
    LPUART_ISR_BIT_CTS          = 1 << 10,
    LPUART_ISR_BIT_BUSY         = 1 << 16,
    LPUART_ISR_BIT_CMF          = 1 << 17,
    LPUART_ISR_BIT_SBKF         = 1 << 18,
    LPUART_ISR_BIT_RWU          = 1 << 19,
    LPUART_ISR_BIT_WUF          = 1 << 20,
    LPUART_ISR_BIT_TEACK        = 1 << 21,
    LPUART_ISR_BIT_REACK        = 1 << 22,
};


void lpuart_write_byte(struct lpuart *lpuart, uint8_t byte);

void lpuart_write_buf(struct lpuart *lpuart, char *buf, size_t len);

void lpuart_init(uint32_t baud_rate);

#endif
