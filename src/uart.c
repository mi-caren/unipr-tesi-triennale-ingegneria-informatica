#include "uart.h"
#include "utils.h"
#include "gpio.h"
#include "rcc.h"


void uart_init(struct Uart *uart, uint32_t baud_rate) {
    if (uart == LPUART1) {
        rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOAEN);
        rcc_lp_peripheral_ck_enable(RCC_APB1ENR2_BIT_LPUART1EN);
        gpio_set_mode(GPIOA, GPIO_PIN_2, GPIO_MODE_ALTERNATE_FUNCTION);
        gpio_set_af(GPIOA, GPIO_PIN_2, GPIO_PA2_AF_LPUART1_TX);
        gpio_set_mode(GPIOA, GPIO_PIN_3, GPIO_MODE_ALTERNATE_FUNCTION);
        gpio_set_af(GPIOA, GPIO_PIN_3, GPIO_PA3_AF_LPUART1_RX);
        uart->BRR = ( ( 256 * LPUART_KER_CK_PRES ) ) / baud_rate;
        uart->CR1 |= LPUART_CR1_BIT_UE | LPUART_CR1_BIT_TE;
    }
}

void uart_write_byte(struct Uart *uart, uint8_t byte) {
    uart->TDR = byte;
    while ((uart->ISR & LPUART_ISR_BIT_TXE) == 0) {
        spin(1);
    }
}

void uart_write_buf(struct Uart *uart, char *buf) {
    while (*buf != 0) {
        uart_write_byte(uart, *(buf++));
    }
}
