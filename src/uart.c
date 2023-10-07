#include "uart.h"
#include "utils.h"
#include "gpio.h"
#include "rcc.h"
#include "utils.h"


void uart_init(struct Uart *uart, uint32_t baud_rate) {
    if (uart == LPUART1) {
        rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOAEN);
        rcc_lp_peripheral_ck_enable(RCC_APB1ENR2_BIT_LPUART1EN);
        gpio_set_mode(GPIOA, GPIO_PIN_2, GPIO_MODE_ALTERNATE_FUNCTION);
        gpio_set_af(GPIOA, GPIO_PIN_2, GPIO_PA2_AF_LPUART1_TX);
        gpio_set_mode(GPIOA, GPIO_PIN_3, GPIO_MODE_ALTERNATE_FUNCTION);
        gpio_set_af(GPIOA, GPIO_PIN_3, GPIO_PA3_AF_LPUART1_RX);
        uart->BRR = ( ( 256 * LPUART_KER_CK_PRES ) ) / baud_rate;
        uart->CR1 |= UART_CR1_BIT_UE | UART_CR1_BIT_TE;
    }
}

void uart_write_byte(struct Uart *uart, uint8_t byte) {
    uart->TDR = byte;
    while ((uart->ISR & UART_ISR_BIT_TXE) == 0) {
        spin(1);
    }
}

uint16_t uart_read_data(struct Uart *uart, uint8_t wl) {
    if (!uart_data_received(uart)) {
        return UART_ERR_READ_DATA_NO_DATA;
    }
    return uart->RDR & 0x1ff & ~(3 << wl);
}

uint8_t uart_get_word_length(struct Uart *uart) {
    uint8_t wl = 0;
    // potrei creare una funzione che dato un bit restituisca la sua posizione
    // @TODO
    wl |= (uart->CR1 & UART_CR1_BIT_M1) >> 28 << 1;
    wl |= (uart->CR1 & UART_CR1_BIT_M0) >> 12;
    switch (wl) {
        case 0:
            return 8;
        case 1:
            return 9;
        case 2:
            return 7;
        default:
            return UART_ERR_GET_WL;
    }
}

void uart_write_buf(struct Uart *uart, char *buf) {
    while (*buf != 0) {
        uart_write_byte(uart, *(buf++));
    }
}

bool uart_data_received(struct Uart *uart) {
    if ((uart->ISR & UART_ISR_BIT_RXNE) != 0) {
        return true;
    }
    return false;
}

bool uart_transmission_completed(struct Uart *uart) {
    if ((uart->ISR & UART_ISR_BIT_TC) != 0) {
        // uart->ICR |= UART_ICR_BIT_TCCF;
        return true;
    }
    return false;
}