#ifndef STM32WL55JC_CM4_H
#define STM32WL55JC_CM4_H

#include <stddef.h>
#include <stdbool.h>

#include "gpio.h"
#include "rcc.h"

typedef struct gpio Gpio;
typedef Gpio GpioA;
typedef Gpio GpioB;

typedef struct rcc Rcc;

#define GPIOA_MEM_ADDR     (GpioA*) 0x48000000
#define GPIOB_MEM_ADDR     (GpioB*) 0x48000400

#define RCC_MEM_ADDR       (Rcc*)   0x58000000

static bool gpioa_taken = false;
static bool gpiob_taken = false;
static bool rcc_taken = false;

typedef enum RccRegister {
    Ahb1Enr,
    Ahb2Enr,
    Ahb3Enr,
    Apb1Enr1,
    Apb1Enr2,
    Apb2Enr,
    Apb3Enr,
} RccRegister;

typedef enum RccAhb1EnrBits {
    Dma1EnBit    = 1 << 0,
    Dma2EnBit    = 1 << 1,
    DmaMux1EnBit = 1 << 2,
    CrcEnBit     = 1 << 12,
} RccAhb1EnrBits;

typedef enum RccAhb2EnrBits {
    GpioAEnBit         = 1 << 0,
    GpioBEnBit         = 1 << 1,
    GpioCEnBit         = 1 << 2,
    GpioHEnBit         = 1 << 7,
} RccAhb2EnrBits;

typedef enum RccAhb3EnrBits {
    PkaEnBit        = 1 << 16,
    AesEnBit        = 1 << 17,
    RngEnBit        = 1 << 18,
    HsemEnBit       = 1 << 19,
    IpccEnBit       = 1 << 20,
    FlashEnBit      = 1 << 25,
} RccAhb3EnrBits;

typedef enum RccApb1Enr1Bits {
    Tim2EnBit        = 1 << 0,
    RtcApbEnBit      = 1 << 10,
    WWdgEnBit        = 1 << 11,
    Spi2s2EnBit      = 1 << 14,
    Usart2EnBit      = 1 << 17,
    I2c1EnBit        = 1 << 21,
    I2c2EnBit        = 1 << 22,
    I2c3EnBit        = 1 << 23,
    DacEnBit         = 1 << 29,
    LpTim1EnBit      = 1 << 31,
} RccApb1Enr1Bits;

typedef enum RccApb1Enr2Bits {
    LpUart1EnBit    = 1 << 0,
    LpTim2EnBit     = 1 << 5,
    LpTim3EnBit     = 1 << 6,
} RccApb1Enr2Bits;

typedef enum RccApb2EnrBits {
    AdcEnBit        = 1 << 9,
    Tim1EnBit       = 1 << 11,
    Spi1EnBit       = 1 << 12,
    Usart1EnBit     = 1 << 14,
    Tim16EnBit      = 1 << 17,
    Tim17EnBit      = 1 << 18,
} RccApb2EnrBits;

typedef enum RccApb3EnrBits {
    SubGhzSpiEnBit    = 1 << 0,
} RccApb3EntBits;

typedef enum PeripheralClock {
    // AHB1ENR
    Dma1Clock,
    Dma2Clock,
    DmaMux1Clock,
    CrcClock,

    // AHB2ENR
    GpioAClock,
    GpioBClock,
    GpioCClock,
    GpioHClock,

    // AHB3ENR
    PkaClock,
    AesClock,
    RngClock,
    HsemClock,
    IpccClock,
    FlashClock,

    // APB1ENR1
    Tim2Clock,
    RtcApbClock,
    WWdgClock,
    Spi2s2Clock,
    Usart2Clock,
    I2c1Clock,
    I2c2Clock,
    I2c3Clock,
    DacClock,
    LpTim1Clock,

    // APB1ENR2
    LpUart1Clock,
    LpTim2Clock,
    LpTim3Clock,

    // APB2ENR
    AdcClock,
    Tim1Clock,
    Spi1Clock,
    Usart1Clock,
    Tim16Clock,
    Tim17Clock,

    // APB3ENR
    SubGhzSpiClock,
} PeripheralClock;

Rcc* takeRcc();
GpioA* takeGpioA(Rcc* rcc);
GpioB* takeGpioB(Rcc* rcc);

int rccSetRegisterBits(Rcc* rcc, RccRegister reg, uint32_t bits);

void clockEnable(Rcc* rcc, PeripheralClock periph_clock);
// void unset_reg_bits(uint32_t* reg, uint32_t bits) {}
// write_reg_bits()

#endif
