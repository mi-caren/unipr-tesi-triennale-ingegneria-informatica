#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>


#define MEM_OFFSET(base, offset)     *( (volatile uint32_t *) (base + offset) )


enum Error {
    ERROR_WRONG_PERIPHERAL = 1,
    ERROR_WRONG_REGISTER,
    ERROR_UNKNOWN_PERIPHERAL
};

void spin(volatile uint32_t count);
char* int_to_string(int n);

#endif