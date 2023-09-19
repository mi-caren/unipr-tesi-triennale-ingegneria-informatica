#include <stdint.h>

#include "main.h"

volatile uint32_t systick_ovf;


int main(void) {
    char str[MAX_NUMBER_OF_DIGITS];
    char str_len;
    rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOBEN);
	gpio_set_mode(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT);

    lpuart_init(115200);

    // init SysTick
    systick_init_ms();

    while (1) {
        gpio_write(GPIOB, GPIO_PIN_15, HIGH);

        str_len = int_to_string(-123, str);
        lpuart_write_buf(LPUART1, str, str_len);
        lpuart_write_byte(LPUART1, '\n');

        delay(1000);
        gpio_write(GPIOB, GPIO_PIN_15, LOW);
        delay(1000);
    }

    return 0;
}



void spin(volatile uint32_t count) {
	while (count--) (void) 0;
}

void systick_init_ms() {
    SYSTICK->VAL = 0;
    SYSTICK->LOAD = 4000000 / 1000;
    SYSTICK->CTRL |= 0b111;
}

void systick_handler(void) {
    systick_ovf++;
}

void delay(unsigned int ms) {
    uint32_t end_ms = systick_ovf + ms;
    while (systick_ovf < end_ms) (void) 0;
}

int int_to_string(int n, char *buf) {
    uint8_t digits[MAX_NUMBER_OF_DIGITS];
    unsigned int digits_count = 0;
    unsigned int buf_idx = 0;
    unsigned int quotient = n;
    uint8_t rest;

    if (n < 0) {
        buf[buf_idx++] = '-';
        n = n * -1;
    }

    while (quotient != 0) {
        quotient = n / 10;
        rest = n % 10;
        digits[digits_count++] = rest;
        n = quotient;
    }

    while (digits_count != 0) {
        buf[buf_idx++] = digits[--digits_count] + 0x30;
    }

    return buf_idx;
}
