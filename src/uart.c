#include "uart.h"
#include "main.h"

void lpuart_init(uint32_t baud_rate) {
    rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOAEN);
    rcc_lp_peripheral_ck_enable(RCC_APB1ENR2_BIT_LPUART1EN);
    gpio_set_mode(GPIOA, GPIO_PIN_2, GPIO_MODE_ALTERNATE_FUNCTION);
    gpio_set_af(GPIOA, GPIO_PIN_2, GPIO_PA2_AF_LPUART1_TX);
    gpio_set_mode(GPIOA, GPIO_PIN_3, GPIO_MODE_ALTERNATE_FUNCTION);
    gpio_set_af(GPIOA, GPIO_PIN_3, GPIO_PA3_AF_LPUART1_RX);

    LPUART1->BRR = ( ( 256 * LPUART_KER_CK_PRES ) ) / baud_rate;
    LPUART1->CR1 |= LPUART_CR1_BIT_UE | LPUART_CR1_BIT_TE;
}

void lpuart_write_byte(struct lpuart *lpuart, uint8_t byte) {
    lpuart->TDR = byte;
    while ((lpuart->ISR & LPUART_ISR_BIT_TXE) == 0) {
        spin(1);
    }
}

void lpuart_write_buf(struct lpuart *lpuart, char *buf, size_t len) {
    for (size_t i = 0; i < len; i++) {
        lpuart_write_byte(lpuart, buf[i]);
    }
}
