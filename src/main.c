#include <stdint.h>

#include "main.h"

volatile uint32_t systick_ovf;

__attribute__((section(".vectors"))) void (*tab[16 + 62]) (void) = {
	_estack,
	_reset,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	systick_handler
};

int main(void) {
    rcc_gpio_ck_enable(RCC_AHB2ENR_BIT_GPIOBEN);
	gpio_set_mode(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT);

    lpuart_init(115200);

    // init SysTick
    systick_init_ms();

    while (1) {
        gpio_write(GPIOB, GPIO_PIN_15, HIGH);
        lpuart_write_buf(LPUART1, "hi\n", 3);
        delay(1000);
        gpio_write(GPIOB, GPIO_PIN_15, LOW);
        delay(1000);
    }

    return 0;
}



void spin(volatile uint32_t count) {
	while (count--) (void) 0;
}

void systick_init_ms() {
    SYSTICK->VAL = 0;
    SYSTICK->LOAD = 4000000 / 1000;
    SYSTICK->CTRL |= 0b111;
}

void systick_handler(void) {
    systick_ovf++;
}

void delay(unsigned int ms) {
    uint32_t end_ms = systick_ovf + ms;
    while (systick_ovf < end_ms) (void) 0;
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
