#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

enum Error {
    ERROR_WRONG_PERIPHERAL = 1,
    ERROR_WRONG_REGISTER,
    ERROR_UNKNOWN_PERIPHERAL
};

void spin(volatile uint32_t count);
char* int_to_string(int n);
// void set_register_bits(volatile uint32_t *periph, uint32_t reg, uint32_t bits);
// uint32_t get_register_bits(volatile uint32_t *periph, uint32_t reg, uint32_t bits);
// int register_check(volatile uint32_t *periph, uint32_t reg);

#endif