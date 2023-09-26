#include <stdint.h>
#include "uart.h"
#include "utils.h"

#define MAX_NUMBER_OF_DIGITS		11
#define MAX_TIMERS                  10


extern volatile uint32_t systick_ovf;
extern uint32_t systick_ovf_per_sec;

struct CpuTimer timers[MAX_TIMERS];
uint8_t timers_count;


void spin(volatile uint32_t count) {
	while (count--) (void) 0;
}

void delay(unsigned int ms) {
    uint32_t end_ovf = systick_ovf + ms*(systick_ovf_per_sec/1000) + (systick_ovf_per_sec%1000 != 0 ? 1 : 0);
    while (systick_ovf < end_ovf) (void) 0;
}

char* int_to_string(int n) {
    static char buf[MAX_NUMBER_OF_DIGITS + 1]; // +1 for null char (0)
    uint8_t digits[MAX_NUMBER_OF_DIGITS];
    unsigned int digits_count = 0;
    unsigned int buf_idx = 0;
    unsigned int quotient = n;
    uint8_t rest;

    if (n < 0) {
        buf[buf_idx++] = '-';
        n = n * -1;
    }

    do {
        quotient = n / 10;
        rest = n % 10;
        digits[digits_count++] = rest;
        n = quotient;
    } while (quotient != 0);

    while (digits_count != 0) {
        buf[buf_idx++] = digits[--digits_count] + 0x30;
    }
    buf[buf_idx] = 0;

    return buf;
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

uint8_t cpu_timer_get_timers_count() {
    return timers_count;
}

void cpu_timer_tick(uint8_t index) {
    if (index < MAX_TIMERS) {
        timers[index].current_value -= 1;
    }
}