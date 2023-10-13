#include <stdint.h>

#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "rcc.h"
#include "utils.h"
#include "sdi12.h"


#define MONITOR_ROW_LENGTH                  32
#define MAX_CONSEC_IDENTICAL_PRINTS         8

#define SYSTICK_OVF_PER_SEC                 (1200)

volatile uint32_t systick_ovf;
uint32_t systick_ovf_per_sec;




int main(void) {
    rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOBEN);
	gpio_set_mode(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT);

    // Initialize LPUART to communicate with serial terminal
    uart_init(LPUART1, 115200);
    sdi12_init(USART1);

    // init SysTick
    systick_init(SYSTICK_OVF_PER_SEC);

    // ensure timers_count = 0 to avoid memory leaks when resetting the MCU
    cpu_timers_clean();
    struct CpuTimer *cpu_timer_blink = cpu_timer_new(2000);
    struct CpuTimer *cpu_timer_sdi12_wake_up = cpu_timer_new(10000);

    uart_write_buf(LPUART1, "\n\r");
    uart_write_buf(LPUART1, "App start");
    uart_write_buf(LPUART1, "\n\r");

    #ifdef DEBUG
        gpio_set_mode(GPIOB, GPIO_PIN_2, GPIO_MODE_OUTPUT);
        gpio_set_mode(GPIOB, GPIO_PIN_4, GPIO_MODE_OUTPUT);
    #endif

    while (1) {
        if (cpu_timer_wait(cpu_timer_sdi12_wake_up)) {
            uint8_t sensor_address = 1;
            struct Float values[SDI12_MAX_MEASUREMENTS];
            uint8_t values_count;
            uint8_t err = sdi12_get_measurement(USART1, sensor_address, values, &values_count);
            if (err != SDI12_GET_MEASUREMENT_OK) {
                app_log("(SDI12_ERR_GET_MEASUREMENT %d) Error while getting measurements from sensor %d", (int[]){err, sensor_address});
                continue;
            }

            app_log("Received Values:", (int[]){});
            // 0: ElectricalConductivity
            // 1: Temperature
            // 2: VolumetricWaterContent
            // TTN test: 01 | 02 C9 FD 02 | 00 01 1A 01 | 00 00 00 00
            for (uint8_t values_idx = 0; values_idx < values_count; values_idx++) {
                app_log("%d: %d.%d", (int[]){
                    values_idx,
                    values[values_idx].value / power(10, values[values_idx].decimal_count),
                    values[values_idx].value % power(10, values[values_idx].decimal_count) });
            }
            uart_write_byte(LPUART1, '\n');
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
    for (uint8_t i = 0; i < cpu_timer_get_timers_count(); i++) {
        cpu_timer_tick(i);
    }

    // monitor_pin(GPIOA, GPIO_PIN_9);
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
    uart_write_buf(LPUART1, "\n\r");
}
