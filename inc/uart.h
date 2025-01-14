#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


#define USART2                  ( ( struct Uart * ) 0x40004400 )
#define LPUART1                 ( ( struct Uart * ) 0x40008000 )
#define USART1                  ( ( struct Uart * ) 0x40013800 )


#define LPUART_KER_CK_PRES          4000000 // 4Mhz
#define UART1_KER_CK_PRES           4000000 // 4Mhz


struct Uart {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
    volatile uint32_t PRESC;
};


enum UartRegistersBits {
    // CR1 bits (FIFO disabled)
    UART_CR1_BIT_UE             = 1 << 0,
    LPUART_CR1_BIT_UESM         = 1 << 1,
    UART_CR1_BIT_RE             = 1 << 2,
    UART_CR1_BIT_TE             = 1 << 3,
    LPUART_CR1_BIT_IDLEIE       = 1 << 4,
    LPUART_CR1_BIT_RXNEIE       = 1 << 5,
    LPUART_CR1_BIT_TCIE         = 1 << 6,
    LPUART_CR1_BIT_TXEIE        = 1 << 7,
    LPUART_CR1_BIT_PEIE         = 1 << 8,
    UART_CR1_BIT_PS             = 1 << 9,
    UART_CR1_BIT_PCE            = 1 << 10,
    LPUART_CR1_BIT_WAKE         = 1 << 11,
    UART_CR1_BIT_M0             = 1 << 12,
    LPUART_CR1_BIT_MME          = 1 << 13,
    LPUART_CR1_BIT_CMIE         = 1 << 14,

    UART_CR1_BIT_OVER8          = 1 << 15,
    UART_CR1_BIT_RTOIE          = 1 << 26,
    UART_CR1_BIT_EOBIE          = 1 << 27,
    UART_CR1_BIT_M1             = 1 << 28,

    LPUART_CR1_BIT_FIFOEN       = 1 << 29,

    UART_CR1_BIT_TXFEIE         = 1 << 30,
    UART_CR1_BIT_RXFFIE         = 1 << 31,

    // CR2 bits
    UART_CR2_BIT_STOP0          = 1 << 12,
    UART_CR2_BIT_STOP1          = 1 << 13,
    UART_CR2_BIT_RXINV          = 1 << 16,
    UART_CR2_BIT_TXINV          = 1 << 17,
    UART_CR2_BIT_DATAINV        = 1 << 18,

    // CR3 bits
    UART_CR3_BIT_HDSEL          = 1 << 3,

    // ISR bits
    UART_ISR_BIT_PE             = 1 << 0,
    UART_ISR_BIT_FE             = 1 << 1,
    UART_ISR_BIT_NE             = 1 << 2,
    UART_ISR_BIT_ORE            = 1 << 3,
    UART_ISR_BIT_IDLE           = 1 << 4,
    UART_ISR_BIT_RXNE           = 1 << 5,
    UART_ISR_BIT_TC             = 1 << 6,
    UART_ISR_BIT_TXE            = 1 << 7,
    UART_ISR_BIT_LBDF           = 1 << 8,
    UART_ISR_BIT_CTSIF          = 1 << 9,
    UART_ISR_BIT_CTS            = 1 << 10,
    UART_ISR_BIT_BUSY           = 1 << 16,
    UART_ISR_BIT_CMF            = 1 << 17,
    UART_ISR_BIT_SBKF           = 1 << 18,
    UART_ISR_BIT_RWU            = 1 << 19,
    UART_ISR_BIT_WUF            = 1 << 20,
    UART_ISR_BIT_TEACK          = 1 << 21,
    UART_ISR_BIT_REACK          = 1 << 22,

    // ICR bits
    UART_ICR_BIT_PECF           = 1 << 0,
    UART_ICR_BIT_FECF           = 1 << 1,
    UART_ICR_BIT_NECF           = 1 << 2,
    UART_ICR_BIT_ORECF          = 1 << 3,
    UART_ICR_BIT_IDLECF         = 1 << 4,
    UART_ICR_BIT_TCCF           = 1 << 6,
    UART_ICR_BIT_LBDF           = 1 << 8,

    // RQR bits
    UART_RQR_BIT_RXFRQ          = 1 << 3,
};

enum UartErrors {
    UART_ERR_READ_DATA_WL               = 0x200,
    UART_ERR_READ_DATA_NO_DATA          = 0x201,

    UART_ERR_GET_WL                     = 0x0,
};

void uart_init(struct Uart *uart, uint32_t baud_rate);
void uart_write_byte(struct Uart *uart, uint8_t byte);
void uart_write_buf(struct Uart *uart, char *buf);
bool uart_data_received(struct Uart *uart);
bool uart_transmission_completed(struct Uart *uart);
uint16_t uart_read_data(struct Uart *uart, uint8_t wl);
uint8_t uart_get_word_length(struct Uart *uart);

#endif
