#include "uart.h"
#include "rcc.h"
#include "gpio.h"
#include "utils.h"

void spi12_init(struct Uart *uart) {
    if (uart == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR2_BIT_USART1EN;
        RCC->AHB2ENR |= RCC_AHB2ENR_BIT_GPIOAEN;

        // gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_ALTERNATE_FUNCTION);
        // gpio_set_mode(GPIOA, GPIO_PIN_10, GPIO_MODE_ALTERNATE_FUNCTION);

        gpio_set_af(GPIOA, GPIO_PIN_9, GPIO_PA9_AF_USART1_TX);
        // gpio_set_af(GPIOA, GPIO_PIN_10, GPIO_PA10_AF_USART1_RX);

        uart->CR3 |= UART_CR3_BIT_HDSEL; // half-duplex mode
        GPIOA->OTYPER |= 1 << GPIO_PIN_9; // TX pin open-drain
        GPIOA->PUPDR |= 0b01 << (GPIO_PIN_9 * 2); // TX pull-up
        // GPIOA->PUPDR |= 0b10 << (GPIO_PIN_9 * 2); // TX pull-down

        uart->CR2 |= UART_CR2_BIT_TXINV | UART_CR2_BIT_RXINV; // negative logic

        uart->BRR = UART1_KER_CK_PRES / 1200;
        
        uart->CR1 &= ~(UART_CR1_BIT_M1 | UART_CR1_BIT_M0);
        uart->CR1 |= UART_CR1_BIT_M1;   // set word length to 7

        uart->CR1 |= UART_CR1_BIT_PCE; // parity control enabled
        uart->CR1 &= ~(UART_CR1_BIT_PS); // even parity

        uart->CR2 &= ~(UART_CR2_BIT_STOP1 | UART_CR2_BIT_STOP0); // 1 stop bit

        uart->CR1 |= UART_CR1_BIT_UE | UART_CR1_BIT_TE | UART_CR1_BIT_RE;

        gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT);
        gpio_write(GPIOA, GPIO_PIN_9, LOW);
    }
}

void spi12_wake_up(struct Uart *uart) {
    if (uart == USART1) {
        gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT);
        gpio_write(GPIOA, GPIO_PIN_9, HIGH);
        delay(12);
        gpio_write(GPIOA, GPIO_PIN_9, LOW);
        delay(9);
    }
}

uint8_t spi12_start_measurement(struct Uart *uart, uint8_t sensor_address) {
    if (sensor_address > 9) {
        return 1;
    }
    char buf[] = "aM!";
    buf[0] = sensor_address + 0x30;

    if (uart == USART1) {
        gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_ALTERNATE_FUNCTION);
        uart_write_buf(USART1, buf);
    }

    return 0;
}