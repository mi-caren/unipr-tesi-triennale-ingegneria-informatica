#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>


#define GPIO_MEM_ADDR		0x48000000

#define GPIOA_OFFSET		0x0000
#define GPIOB_OFFSET		0x0400

#define GPIOA               ( ( struct gpio * ) ( GPIO_MEM_ADDR + GPIOA_OFFSET ) )
#define GPIOB               ( ( struct gpio * ) ( GPIO_MEM_ADDR + GPIOB_OFFSET ) )


struct gpio {
    // configuration registers
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;

    // data registers
    volatile uint32_t IDR;
    volatile uint32_t ODR;

    // set/reset register
    volatile uint32_t BSRR;

    // locking register
    volatile uint32_t LCKR;

    // alternate function registers
    volatile uint32_t AFRH;
    volatile uint32_t AFRL;

};

enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE_FUNCTION,
    GPIO_MODE_ANALOG
};

enum {
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15
};


int gpio_set_mode(struct gpio *gpio, uint8_t pin, uint8_t mode);

int gpio_write(struct gpio *gpio, uint8_t pin, uint8_t val);


#endif