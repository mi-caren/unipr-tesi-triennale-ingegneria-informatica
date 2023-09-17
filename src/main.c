#include <stdint.h>

#include "main.h"
#include "gpio.h"
#include "rcc.h"



int main(void) {
    // enable GPIOB clock
    RCC->AHB2ENR |= ( 0b1 << 1 );
	gpio_set_mode(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_write(GPIOB, GPIO_PIN_15, HIGH);
        spin(999999);
        gpio_write(GPIOB, GPIO_PIN_15, LOW);
        spin(999999);
    }

    return 0;
}




void spin(volatile uint32_t count) {
	while (count--) (void) 0;
}


void _reset(void) {
	extern unsigned int _data_start, _data_end, _data_loadaddr, _bss_start, _bss_end;

    for (unsigned int *bss_p = &_bss_start; bss_p < &_bss_end; bss_p++) {
        *bss_p = 0;
    }

    unsigned int *data_lvm_p = &_data_loadaddr;
    unsigned int *data_sram1_p = &_data_start;
    while (data_sram1_p < &_data_end) {
        *data_sram1_p = *data_lvm_p;
        data_sram1_p++;
        data_lvm_p++;
    }

    main();

    while (1) (void) 0;
}
