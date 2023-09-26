#include <stdint.h>
#include "uart.h"
#include "utils.h"

#define MAX_NUMBER_OF_DIGITS		( 11 )

extern volatile uint32_t systick_ovf;
extern uint32_t systick_ovf_per_sec;



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
