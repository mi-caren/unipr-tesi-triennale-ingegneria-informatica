#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>


#define MEM_OFFSET(base, offset)     *( (volatile uint32_t *) (base + offset) )

// millisecond timer made with software
struct CpuTimer {
    unsigned int reset_value;
	unsigned int current_value;
};

enum Error {
    ERROR_WRONG_PERIPHERAL = 1,
    ERROR_WRONG_REGISTER,
    ERROR_UNKNOWN_PERIPHERAL
};

void spin(volatile uint32_t count);
char* int_to_string(int n);
void delay(unsigned int ms);

struct CpuTimer* cpu_timer_new(unsigned int timeout);
uint8_t cpu_timer_wait(struct CpuTimer *cpu_timer);
void cpu_timer_reset(struct CpuTimer *cpu_timer);
void cpu_timer_init(struct CpuTimer *cpu_timer, unsigned int timeout);
uint8_t cpu_timer_get_timers_count();
void cpu_timer_tick(uint8_t index);
void cpu_timer_remove(struct CpuTimer *cpu_timer);
void cpu_timers_clean();
double power(uint8_t base, int8_t exp);

#endif