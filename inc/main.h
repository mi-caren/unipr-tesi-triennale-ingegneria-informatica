#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include "gpio.h"


// address in memory where systick registers are mapped
#define SYSTICK_REG_MEM_ADDR    	0xe000e010

#define SYSTICK                 	( ( struct systick * ) SYSTICK_REG_MEM_ADDR )


// keeps track of the number of times systick reacked the end of count
extern volatile uint32_t systick_ovf;

struct systick {
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile uint32_t CALIB;
};

struct CpuTimer {
    unsigned int reset_value;
	unsigned int current_value;
};


void systick_init_ms();
void app_log(char *buf, int *buf_values);

struct CpuTimer* cpu_timer_new(unsigned int timeout);
uint8_t cpu_timer_wait(struct CpuTimer *cpu_timer);

void monitor_pin(struct gpio *gpio, uint8_t pin);
char int_to_hex_char(uint8_t n);

#endif
