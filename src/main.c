#include <stdint.h>

#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "rcc.h"
#include "utils.h"
#include "spi12.h"

#define MAX_TIMERS                          10

#define MONITOR_ROW_LENGTH                  32
#define MAX_CONSEC_IDENTICAL_PRINTS         8

#define SYSTICK_OVF_PER_SEC                 (1200)

volatile uint32_t systick_ovf;
uint32_t systick_ovf_per_sec;

struct CpuTimer timers[MAX_TIMERS];
uint8_t timers_count;


int main(void) {
    rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOBEN);
	gpio_set_mode(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT);

    // Initialize LPUART to communicate with serial terminal
    uart_init(LPUART1, 115200);
    spi12_init(USART1);

    // init SysTick
    systick_init(SYSTICK_OVF_PER_SEC);

    timers_count = 0;
    struct CpuTimer *cpu_timer_blink = cpu_timer_new(2000);
    struct CpuTimer *cpu_timer_spi12_wake_up = cpu_timer_new(5000);
    struct CpuTimer *cpu_timer_sensor_response_timeout = cpu_timer_new(15);

    uart_write_byte(LPUART1, '\n');
    uart_write_buf(LPUART1, "App start");
    uart_write_byte(LPUART1, '\n');

    while (1) {
        if (cpu_timer_wait(cpu_timer_spi12_wake_up)) {
            spi12_wake_up(USART1);
            spi12_start_measurement(USART1, 0);
            cpu_timer_reset(cpu_timer_sensor_response_timeout);
            uint8_t sensor_response_timed_out = 1;

            while (!cpu_timer_wait(cpu_timer_sensor_response_timeout) && !uart_data_received(USART1)) (void) 0;

            if (sensor_response_timed_out) {
                uart_write_buf(LPUART1, "No responde from sensor!\n");
                continue;
            }

            
            // Sono stati ricevuti dei dati
        }

        // blink blue LED
        if (cpu_timer_wait(cpu_timer_blink)) {
            gpio_write(GPIOB, GPIO_PIN_15, !gpio_read(GPIOB, GPIO_PIN_15));
        }
    }

    return 0;
}




void systick_init(uint32_t ovf_per_sec) {
    systick_ovf_per_sec = ovf_per_sec;
    SYSTICK->VAL = 0;
    SYSTICK->LOAD = 4000000 / ovf_per_sec;
    SYSTICK->CTRL |= 0b111;
    systick_ovf = 0;
}

void systick_handler(void) {
    systick_ovf++;
    for (uint8_t i = 0; i < timers_count; i++) {
        timers[i].current_value -= 1;
    }

    monitor_pin(GPIOA, GPIO_PIN_9);
}

void monitor_pin(struct gpio *gpio, uint8_t pin) {
    static uint8_t pin_value = 0;
    static uint8_t pin_buf = 0;
    static uint8_t bit_counter = 7;
    static uint8_t row_counter = MONITOR_ROW_LENGTH;
    static uint8_t last_buf_read = 0;
    static uint32_t identical_prints_count = 0;;

    pin_value = gpio_read(gpio, pin);
    pin_buf |= pin_value << bit_counter;
    if (bit_counter == 0) {
        bit_counter = 7;
        if (last_buf_read == pin_buf) {
            identical_prints_count++;
        } else {
            identical_prints_count = 0;
        }
        if (identical_prints_count < MAX_CONSEC_IDENTICAL_PRINTS) {
            uart_write_byte(LPUART1, int_to_hex_char((0xf0 & pin_buf) >> 4));
            uart_write_byte(LPUART1, int_to_hex_char(0xf & pin_buf));
            uart_write_byte(LPUART1, ' ');
            if (--row_counter == 0) {
                uart_write_byte(LPUART1, '\n');
                row_counter = MONITOR_ROW_LENGTH;
            }
        }
        if (identical_prints_count == MAX_CONSEC_IDENTICAL_PRINTS) {
            uart_write_buf(LPUART1, "... ");
        }
        last_buf_read = pin_buf;
        pin_buf = 0;
    } else {
        bit_counter--;
    }
}

char int_to_hex_char(uint8_t n) {
    if (n > 15) {
        return 'X';
    } else if (n < 10) {
        return n + 0x30;
    } else {
        return n - 10 + 0x41;
    }
}

struct CpuTimer* cpu_timer_new(unsigned int timeout) {
    struct CpuTimer new_timer = {
        .reset_value = timeout*(systick_ovf_per_sec/1000),
        .current_value = timeout*(systick_ovf_per_sec/1000)
    };
    timers[timers_count++] = new_timer;
    return &(timers[timers_count - 1]);
}

void cpu_timer_reset(struct CpuTimer *cpu_timer) {
    cpu_timer->current_value = cpu_timer->reset_value;
}

uint8_t cpu_timer_wait(struct CpuTimer *cpu_timer) {
    uint8_t i = 0;
    while (i != timers_count) {
        if (&(timers[i]) == cpu_timer) {
            uint32_t end_ovf = systick_ovf + timers[i].current_value + (timers[i].reset_value%1000 != 0 ? 1 : 0);
            if (systick_ovf >= end_ovf) {
                timers[i].current_value = timers[i].reset_value;
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
