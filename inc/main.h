#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>


enum {
    LOW,
    HIGH
};

// reset handler
__attribute__((naked, noreturn)) void _reset(void);

// end of stack, provided by linker
extern void _estack(void);

// vector table. 16 cortex-m4 specific interrupts + 62 stm32wl55 interrupts
__attribute__((section(".vectors"))) void (*tab[16 + 62]) (void) = {
	_estack,
	_reset
};

// perform a NOP for `cout` times
void spin(volatile uint32_t count);

#endif