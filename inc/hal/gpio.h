#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#ifdef HARDWARE

#if HARDWARE == STM32WL55JC
    #include "hardware/stm32wl55jc.h"
#endif

#endif


enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE_FUNCTION,
    GPIO_MODE_ANALOG
};

enum {
    // PA2 alternate functions
    GPIO_PA2_AF_LSCO                = 0,
    GPIO_PA2_AF_TIM2_CH3            = 1,
    GPIO_PA2_AF_USART2_TX           = 7,
    GPIO_PA2_AF_LPUART1_TX          = 8,
    GPIO_PA2_AF_COMP2_OUT           = 12,
    GPIO_PA2_AF_DEBUG_PWR_LDORDY    = 13,
    GPIO_PA2_AF_CM4_EVENTOUT        = 15,

    // PA3 alternate functions
    GPIO_PA3_AF_TIM2_CH4            = 1,
    GPIO_PA3_AF_I2S2_MCK            = 5,
    GPIO_PA3_AF_USART2_RX           = 7,
    GPIO_PA3_AF_LPUART1_RX          = 8,
    GPIO_PA3_AF_CM4_EVENTOUT        = 15,

    // PA9 alternate functions
    GPIO_PA9_AF_TIM1_CH2            = 1,
    GPIO_PA9_AF_SPI2_NSS            = 3,
    GPIO_PA9_AF_I2S2_WS             = 3,
    GPIO_PA9_AF_I2C1_SCL            = 4,
    GPIO_PA9_AF_SPI2_SCK            = 5,
    GPIO_PA9_AF_I2S2_CK             = 5,
    GPIO_PA9_AF_USART1_TX           = 7,
    GPIO_PA9_AF_CM4_EVENTOUT        = 15,

    // PA10 alternate functions
    GPIO_PA10_AF_RTC_REFIN           = 0,
    GPIO_PA10_AF_TIM1_CH3            = 1,
    GPIO_PA10_AF_I2C1_SDA            = 4,
    GPIO_PA10_AF_SPI2_MOSI           = 5,
    GPIO_PA10_AF_I2S2_SD             = 5,
    GPIO_PA10_AF_USART1_RX           = 7,
    GPIO_PA10_AF_DEBUG_RF_HSE32RDY   = 13,
    GPIO_PA10_AF_TIM17_BKIN          = 14,
    GPIO_PA10_AF_CM4_EVENTOUT        = 15,
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

enum {
    LOW,
    HIGH
};

int gpio_set_mode(Gpio *gpio, uint8_t pin, uint8_t mode);

int gpio_set_af(Gpio *gpio, uint8_t pin, uint8_t af);

int gpio_write(Gpio *gpio, uint8_t pin, uint8_t val);
int8_t gpio_read(Gpio *gpio, uint8_t pin);

#endif
