#include <stdint.h>
#include "uart.h"
#include "utils.h"

#define MAX_NUMBER_OF_DIGITS		( 11 )


void spin(volatile uint32_t count) {
	while (count--) (void) 0;
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


// void set_register_bits(uint32_t *periph, uint32_t reg, uint32_t bits) {
//     if (register_check(periph, reg) == 0) {
//         *(periph + ( reg * REGISTER_BYTES_DIM )) |= bits;
//     } else {
//         error_handler(ERROR_WRONG_REGISTER);
//     }
// }


// uint32_t get_register_bits(volatile uint32_t *periph, uint32_t reg, uint32_t bits) {
//     if (register_check(periph, reg) == 0) {
//         return *(periph + ( reg * REGISTER_BYTES_DIM )) & bits;
//     } else {
//         error_handler(ERROR_WRONG_REGISTER);
//     }
// }

// uint8_t register_check(volatile uint32_t *periph, uint32_t reg) {
//     uint8_t ret = 0;
//     if (periph == LPUART1 || periph == USART1 || periph == USART2) {
//         ret = uart_register_check(periph, reg);
//     } else {
//         ret = ERROR_UNKNOWN_PERIPHERAL;
//     }
//     return ret;
// }

// /*
// *   1 -> peripheral is not one of LPUART1, USART1, USART2
// *   2 -> wrong register value
// */
// uint8_t uart_register_check(volatile uint32_t *uart,  int reg) {
//     if (uart != LPUART1 && uart != USART1 && uart != USART2)
//         return ERROR_WRONG_PERIPHERAL;
//     int allowed_registers = 1 << UART_CR1 | 1 << UART_CR2 | 1 << UART_CR3 | 1 << UART_BRR |
//         1 << UART_RQR | 1 << UART_ISR | 1 << UART_ICR | 1 << UART_RDR | 1 << UART_TDR | 1 << UART_PRESC;
//     if (uart != LPUART1)
//         allowed_registers |= 1 << UART_GTPR | 1 << UART_RTOR;
//     if (((1 << reg) & allowed_registers) == 0)
//         return ERROR_WRONG_REGISTER;
//     return 0;
// }
