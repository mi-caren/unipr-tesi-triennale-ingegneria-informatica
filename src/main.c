#include <stdint.h>

#include "main.h"

volatile uint32_t systick_ovf;


int main(void) {
    rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOBEN);
	gpio_set_mode(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT);

    lpuart_init(115200);

    // init SysTick
    systick_init_ms();

    while (1) {
        gpio_write(GPIOB, GPIO_PIN_15, HIGH);
        lpuart_write_buf(LPUART1, "hi\n", 3);
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
