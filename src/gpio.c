#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


#include "gpio.h"

static struct PinTaken {
    bool porta_pin0;
    bool porta_pin1;
    bool porta_pin2;
    bool porta_pin3;
    bool porta_pin4;
    bool porta_pin5;
    bool porta_pin6;
    bool porta_pin7;
    bool porta_pin8;
    bool porta_pin9;
    bool porta_pin10;
    bool porta_pin11;
    bool porta_pin12;
    bool porta_pin13;
    bool porta_pin14;
    bool porta_pin15;

    bool portb_pin0;
    bool portb_pin1;
    bool portb_pin2;
    bool portb_pin3;
    bool portb_pin4;
    bool portb_pin5;
    bool portb_pin6;
    bool portb_pin7;
    bool portb_pin8;
    bool portb_pin9;
    bool portb_pin10;
    bool portb_pin11;
    bool portb_pin12;
    bool portb_pin13;
    bool portb_pin14;
    bool portb_pin15;
} pin_taken = {
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
};

static void setRegisterBits(volatile uint32_t* reg, uint32_t bits) {
    *reg |= bits;
}

static void unsetRegisterBits(volatile uint32_t* reg, uint32_t bits) {
    *reg &= ~(bits);
}

static volatile uint32_t* gpioXGetRegister(GpioX* gpiox, GpioRegister reg) {
    switch (reg) {
        case ModerX: return &gpiox->MODER;
        default:     return NULL;
    }
}

static void gpioXSetRegisterBits(GpioX* gpiox, GpioRegister reg, uint32_t bits) {
    setRegisterBits(gpioXGetRegister(gpiox, reg), bits);
}

static void gpioXUnsetRegisterBits(GpioX* gpiox, GpioRegister reg, uint32_t bits) {
    unsetRegisterBits(gpioXGetRegister(gpiox, reg), bits);
}

static void gpioXSetPinMode(GpioX* gpiox, PortXPin pin, PinMode mode) {
    uint32_t bits = 0b11 << ( pin * 2 );
    gpioXUnsetRegisterBits(gpiox, ModerX, bits);
    bits = mode << ( pin * 2 );
    gpioXSetRegisterBits(gpiox, ModerX, bits);
}

static bool pinTaken(GpioPort port, PortXPin pin) {
    switch (port) {
        case PortA:
            switch (pin) {
                case Pin0:  return pin_taken.porta_pin0;
                case Pin1:  return pin_taken.porta_pin1;
                case Pin2:  return pin_taken.porta_pin2;
                case Pin3:  return pin_taken.porta_pin3;
                case Pin4:  return pin_taken.porta_pin4;
                case Pin5:  return pin_taken.porta_pin5;
                case Pin6:  return pin_taken.porta_pin6;
                case Pin7:  return pin_taken.porta_pin7;
                case Pin8:  return pin_taken.porta_pin8;
                case Pin9:  return pin_taken.porta_pin9;
                case Pin10: return pin_taken.porta_pin10;
                case Pin11: return pin_taken.porta_pin11;
                case Pin12: return pin_taken.porta_pin12;
                case Pin13: return pin_taken.porta_pin13;
                case Pin14: return pin_taken.porta_pin14;
                case Pin15: return pin_taken.porta_pin15;
            }
        case PortB:
            switch (pin) {
                case Pin0:  return pin_taken.portb_pin0;
                case Pin1:  return pin_taken.portb_pin1;
                case Pin2:  return pin_taken.portb_pin2;
                case Pin3:  return pin_taken.portb_pin3;
                case Pin4:  return pin_taken.portb_pin4;
                case Pin5:  return pin_taken.portb_pin5;
                case Pin6:  return pin_taken.portb_pin6;
                case Pin7:  return pin_taken.portb_pin7;
                case Pin8:  return pin_taken.portb_pin8;
                case Pin9:  return pin_taken.portb_pin9;
                case Pin10: return pin_taken.portb_pin10;
                case Pin11: return pin_taken.portb_pin11;
                case Pin12: return pin_taken.portb_pin12;
                case Pin13: return pin_taken.portb_pin13;
                case Pin14: return pin_taken.portb_pin14;
                case Pin15: return pin_taken.portb_pin15;
            }
        case PortC: return true;
        case PortH: return true;
    }
}

static void takePin(GpioPort port, PortXPin pin) {
    switch (port) {
        case PortA:
            switch (pin) {
                case Pin0: pin_taken.porta_pin0 = true; return;
                case Pin1: pin_taken.porta_pin1 = true; return;
                case Pin2: pin_taken.porta_pin2 = true; return;
                case Pin3: pin_taken.porta_pin3 = true; return;
                case Pin4: pin_taken.porta_pin4 = true; return;
                case Pin5: pin_taken.porta_pin5 = true; return;
                case Pin6: pin_taken.porta_pin6 = true; return;
                case Pin7: pin_taken.porta_pin7 = true; return;
                case Pin8: pin_taken.porta_pin8 = true; return;
                case Pin9: pin_taken.porta_pin9 = true; return;
                case Pin10: pin_taken.porta_pin10 = true; return;
                case Pin11: pin_taken.porta_pin11 = true; return;
                case Pin12: pin_taken.porta_pin12 = true; return;
                case Pin13: pin_taken.porta_pin13 = true; return;
                case Pin14: pin_taken.porta_pin14 = true; return;
                case Pin15: pin_taken.porta_pin15 = true; return;
            }
        case PortB:
            switch (pin) {
                case Pin0:  pin_taken.portb_pin0 = true; return;
                case Pin1:  pin_taken.portb_pin1 = true; return;
                case Pin2:  pin_taken.portb_pin2 = true; return;
                case Pin3:  pin_taken.portb_pin3 = true; return;
                case Pin4:  pin_taken.portb_pin4 = true; return;
                case Pin5:  pin_taken.portb_pin5 = true; return;
                case Pin6:  pin_taken.portb_pin6 = true; return;
                case Pin7:  pin_taken.portb_pin7 = true; return;
                case Pin8:  pin_taken.portb_pin8 = true; return;
                case Pin9:  pin_taken.portb_pin9 = true; return;
                case Pin10: pin_taken.portb_pin10 = true; return;
                case Pin11: pin_taken.portb_pin11 = true; return;
                case Pin12: pin_taken.portb_pin12 = true; return;
                case Pin13: pin_taken.portb_pin13 = true; return;
                case Pin14: pin_taken.portb_pin14 = true; return;
                case Pin15: pin_taken.portb_pin15 = true; return;
            }
        case PortC: return;
        case PortH: return;
    }
}

int gpio_set_mode(struct gpio *gpio, uint8_t pin, uint8_t mode) {
    if (pin > 15) {
        return -1;
    }
    if (mode > 3) {
        return -2;
    }

    gpio->MODER &= ~( 0b11 << ( pin * 2 ) );
    gpio->MODER |= ( mode << ( pin * 2 ) );

    return 0;
}

void gpioASetPinMode(GpioA* gpioa, PortAPin pin, PinMode mode) {
    gpioXSetPinMode(gpioa, pin, mode);
}

void gpioBSetPinMode(GpioB* gpiob, PortBPin pin, PinMode mode) {
    gpioXSetPinMode(gpiob, pin, mode);
}

OutputPin takePortXOutputPin(GpioX* gpiox, PortXPin pin) {
    
}

OutputPin takePortBOutputPin(GpioB* gpiob, PortBPin pin) {
    if (!pinTaken(PortB, pin)) {
        takePin(PortB, pin);
        OutputPin output_pin = {
            
        }
    }
}

OutputPin takeOutputPin(GpioPort port, PortXPin pin) {
    if (!pinTaken(port, pin))
    
    OutputPin output_pin = {
        .port = port,
        .pin = pin,
        .level = Low,
    }
}

void setOutputPinLevel(OutputPin pin, PinLevel level) {
    
}

int gpio_set_af(struct gpio *gpio, uint8_t pin, uint8_t af) {
    if (pin > 15) {
        return -1;
    }
    if (af > 15) {
        return -2;
    }

    gpio->AFR[ pin / 8 ] &= ~( 0b1111 << ( ( pin % 8 ) * 4 ) );
    gpio->AFR[ pin / 8 ] |= ( af << ( ( pin % 8 ) * 4 ) );

    return 0;
}

int gpio_write(struct gpio *gpio, uint8_t pin, uint8_t val) {
    if (pin > 15) {
        return -1;
    }
    if (val > 1) {
    	return -2;
    }

    gpio->BSRR = ( ( 0b1 << pin ) << ( val == 1 ? 0 : 16 ) );

	return 0;
}

int8_t gpio_read(struct  gpio *gpio, uint8_t pin) {
    if (pin > 15) {
        return -1;
    }

    return (gpio->IDR & (1 << pin)) >> pin;
}
