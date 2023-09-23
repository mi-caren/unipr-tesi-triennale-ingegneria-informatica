#include <stdint.h>

#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "rcc.h"
#include "utils.h"

volatile uint32_t systick_ovf;


int main(void) {
    rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOBEN);
	gpio_set_mode(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT);

    // Initialize LPUART to communicate with serial terminal
    uart_init(LPUART1, 115200);

    // init SysTick
    systick_init_ms();

    int loop_count = 0;
    while (1) {
        gpio_write(GPIOB, GPIO_PIN_15, HIGH);

        app_log("App made %d loops", (int[]){ loop_count++ });

        delay(1000);
        gpio_write(GPIOB, GPIO_PIN_15, LOW);
        delay(1000);
    }

    return 0;
}




void systick_init_ms() {
    SYSTICK->VAL = 0;
    SYSTICK->LOAD = 4000000 / 1000;
    SYSTICK->CTRL |= 0b111;
    systick_ovf = 0;
}

void systick_handler(void) {
    systick_ovf++;
}

void delay(unsigned int ms) {
    uint32_t end_ms = systick_ovf + ms;
    while (systick_ovf < end_ms) (void) 0;
}


void app_log(char *buf, int *buf_values) {
    unsigned int buf_values_idx = 0;
    while (*buf != 0) {
        if (*buf == '%' && *(buf + 1) == 'd') {
            char *value_buf = int_to_string(buf_values[buf_values_idx++]);
            uart_write_buf(LPUART1, value_buf);
            buf++;
        } else {
            uart_write_byte(LPUART1, *buf);
        }
        buf++;
    }
    uart_write_byte(LPUART1, '\n');
}
