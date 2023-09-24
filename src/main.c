#include <stdint.h>

#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "rcc.h"
#include "utils.h"
#include "spi12.h"

#define MAX_TIMERS          10

volatile uint32_t systick_ovf;

unsigned int *timers[MAX_TIMERS];
unsigned int timers_reset_values[MAX_TIMERS];
unsigned int timers_current_values[MAX_TIMERS];
uint8_t timers_count;


int main(void) {
    rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOBEN);
	gpio_set_mode(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT);

    // Initialize LPUART to communicate with serial terminal
    uart_init(LPUART1, 115200);
    spi12_init(USART1);

    // init SysTick
    systick_init_ms();

    unsigned int timer_blink = 1000;
    unsigned int timer_gpioa_pin_9_check = 50;
    timers_count = 0;
    cpu_timer_start(&timer_blink);
    cpu_timer_start(&timer_gpioa_pin_9_check);

    uint8_t pa19_value;
    uint8_t pa19_buf = 0;
    uint8_t bit_counter = 7;
    uint8_t row_counter = 16;

    uart_write_byte(LPUART1, '\n');
    uart_write_buf(LPUART1, "App start");
    uart_write_byte(LPUART1, '\n');

    while (1) {
        if (cpu_timer_wait(&timer_gpioa_pin_9_check)) {
            pa19_value = gpio_read(GPIOA, GPIO_PIN_9);
            pa19_buf |= pa19_value << bit_counter;
            if (bit_counter == 0) {
                bit_counter = 7;
                uart_write_byte(LPUART1, pa19_buf + 0x30);
                uart_write_byte(LPUART1, ' ');
                if (--row_counter == 0) {
                    uart_write_byte(LPUART1, '\n');
                    row_counter = 16;
                }
            } else {
                bit_counter--;
            }
        }

        // blink blue LED
        if (cpu_timer_wait(&timer_blink)) {
            gpio_write(GPIOB, GPIO_PIN_15, !gpio_read(GPIOB, GPIO_PIN_15));
        }
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
    for (uint8_t i = 0; i < timers_count; i++) {
        timers_current_values[i] -= 1;
    }
}

void delay(unsigned int ms) {
    uint32_t end_ms = systick_ovf + ms;
    while (systick_ovf < end_ms) (void) 0;
}

void cpu_timer_start(unsigned int *timer_ms) {
    timers[timers_count] = timer_ms;
    timers_reset_values[timers_count] = *timer_ms;
    timers_current_values[timers_count] = *timer_ms;
    timers_count++;
}

uint8_t cpu_timer_wait(unsigned int *timer_ms) {
    uint8_t i = 0;
    while (i != timers_count) {
        if (timers[i] == timer_ms) {
            unsigned int end_ms = systick_ovf + timers_current_values[i];
            if (systick_ovf >= end_ms) {
                timers_current_values[i] = timers_reset_values[i];
                return 1;
            }

            break;
        }

        i++;
    }
    return 0;
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
