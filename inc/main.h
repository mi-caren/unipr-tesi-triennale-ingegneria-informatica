#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdlib.h>

#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "rcc.h"

// address in memory where systick registers are mapped
#define SYSTICK_REG_MEM_ADDR    	0xe000e010

#define SYSTICK                 	( ( struct systick * ) SYSTICK_REG_MEM_ADDR )

#define MAX_NUMBER_OF_DIGITS		( 11 )

// keeps track of the number of times systick reacked the end of count
extern volatile uint32_t systick_ovf;

struct systick {
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile uint32_t CALIB;
};

enum {
    LOW,
    HIGH
};

// perform a NOP for `cout` times
void spin(volatile uint32_t count);

void systick_init_ms();

void delay(unsigned int ms);

int int_to_string(int n, char *buf);

#endif
