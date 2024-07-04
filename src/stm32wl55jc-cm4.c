
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "stm32wl55jc-cm4.h"


static void setRegisterBits(volatile uint32_t* reg, uint32_t bits) {
    *reg |= bits;
}

GpioA* takeGpioA(Rcc* rcc) {
    if (!gpioa_taken) {
        gpioa_taken = true;
        clockEnable(rcc, GpioAClock);
        return GPIOA_MEM_ADDR;
    } else {
        return NULL;
    }
}

GpioB* takeGpioB(Rcc* rcc) {
    if (!gpiob_taken) {
        gpiob_taken = true;
        clockEnable(rcc, GpioBClock);
        return GPIOB_MEM_ADDR;
    } else {
        return NULL;
    }
}

Rcc* takeRcc() {
    if (!rcc_taken) {
        rcc_taken = true;
        return RCC_MEM_ADDR;
    } else {
        return NULL;
    }
}

int rccSetRegisterBits(Rcc* rcc, RccRegister reg, uint32_t bits) {
    volatile uint32_t* reg_ptr = NULL;

    switch (reg) {
        case Ahb1Enr:
            if ((bits & ~(Dma1EnBit | Dma2EnBit | DmaMux1EnBit | CrcEnBit)) != 0) {
                return -1;
            }
            reg_ptr = &rcc->AHB1ENR;
            break;

        case Ahb2Enr:
            if ((bits & ~(GpioAEnBit | GpioBEnBit | GpioCEnBit | GpioHEnBit)) != 0) {
                return -1;
            }
            reg_ptr = &rcc->AHB2ENR;
            break;

        case Ahb3Enr:
            if ((bits & ~(PkaEnBit | AesEnBit | RngEnBit | HsemEnBit | IpccEnBit | FlashEnBit)) != 0) {
                return -1;
            }
            reg_ptr = &rcc->AHB3ENR;
            break;

        case Apb1Enr1:
            if ((bits & ~(Tim2EnBit | RtcApbEnBit | WWdgEnBit | Spi2s2EnBit | Usart2EnBit | I2c1EnBit | I2c2EnBit | I2c3EnBit | DacEnBit | LpTim1EnBit)) != 0) {
                return -1;
            }
            reg_ptr = &rcc->APB1ENR1;
            break;

        case Apb1Enr2:
            if ((bits & ~(LpUart1EnBit | LpTim2EnBit | LpTim3EnBit)) != 0) {
                return -1;
            }
            reg_ptr = &rcc->APB1ENR2;
            break;

        case Apb2Enr:
            if ((bits & ~(AdcEnBit | Tim1EnBit | Spi1EnBit | Usart1EnBit | Tim16EnBit | Tim17EnBit)) != 0) {
                return -1;
            }
            reg_ptr = &rcc->APB2ENR;
            break;

        case Apb3Enr:
            if ((bits & ~(SubGhzSpiEnBit)) != 0) {
                return -1;
            }
            reg_ptr = &rcc->APB3ENR;
            break;
    }

    setRegisterBits(reg_ptr, bits);
    return 0;
}

void clockEnable(Rcc* rcc, PeripheralClock periph_clock) {
    RccRegister enable_register;
    uint32_t enr_bit;

    switch (periph_clock) {
        case Dma1Clock:
            enr_bit = Dma1EnBit;
            /* FALLTHRU */
        case Dma2Clock:
            enr_bit = Dma2EnBit;
            /* FALLTHRU */
        case DmaMux1Clock:
            enr_bit = DmaMux1EnBit;
            /* FALLTHRU */
        case CrcClock:
            enr_bit = CrcEnBit;
            enable_register = Ahb1Enr;
            break;

        case GpioAClock:
            enr_bit = GpioAEnBit;
            /* FALLTHRU */
        case GpioBClock:
            enr_bit = GpioBEnBit;
            /* FALLTHRU */
        case GpioCClock:
            enr_bit = GpioCEnBit;
            /* FALLTHRU */
        case GpioHClock:
            enr_bit = GpioHEnBit;
            enable_register = Ahb2Enr;
            break;

        case PkaClock:
            enr_bit = PkaEnBit;
            /* FALLTHRU */
        case AesClock:
            enr_bit = AesEnBit;
            /* FALLTHRU */
        case RngClock:
            enr_bit = RngEnBit;
            /* FALLTHRU */
        case HsemClock:
            enr_bit = HsemEnBit;
            /* FALLTHRU */
        case IpccClock:
            enr_bit = IpccEnBit;
            /* FALLTHRU */
        case FlashClock:
            enr_bit = FlashEnBit;
            enable_register = Ahb3Enr;
            break;

        case Tim2Clock:
            enr_bit = Tim2EnBit;
            /* FALLTHRU */
        case RtcApbClock:
            enr_bit = RtcApbEnBit;
            /* FALLTHRU */
        case WWdgClock:
            enr_bit = WWdgEnBit;
            /* FALLTHRU */
        case Spi2s2Clock:
            enr_bit = Spi2s2EnBit;
            /* FALLTHRU */
        case Usart2Clock:
            enr_bit = Usart2EnBit;
            /* FALLTHRU */
        case I2c1Clock:
            enr_bit = I2c1EnBit;
            /* FALLTHRU */
        case I2c2Clock:
            enr_bit = I2c2EnBit;
            /* FALLTHRU */
        case I2c3Clock:
            enr_bit = I2c3EnBit;
            /* FALLTHRU */
        case DacClock:
            enr_bit = DacEnBit;
            /* FALLTHRU */
        case LpTim1Clock:
            enr_bit = LpTim1EnBit;
            enable_register = Apb1Enr1;
            break;

        case LpUart1Clock:
            enr_bit = LpUart1EnBit;
            /* FALLTHRU */
        case LpTim2Clock:
            enr_bit = LpTim2EnBit;
            /* FALLTHRU */
        case LpTim3Clock:
            enr_bit = LpTim3EnBit;
            enable_register = Apb1Enr2;
            break;

        case AdcClock:
            enr_bit = AdcEnBit;
            /* FALLTHRU */
        case Tim1Clock:
            enr_bit = Tim1EnBit;
            /* FALLTHRU */
        case Spi1Clock:
            enr_bit = Spi1EnBit;
            /* FALLTHRU */
        case Usart1Clock:
            enr_bit = Usart1EnBit;
            /* FALLTHRU */
        case Tim16Clock:
            enr_bit = Tim16EnBit;
            /* FALLTHRU */
        case Tim17Clock:
            enr_bit = Tim17EnBit;
            enable_register = Apb2Enr;
            break;

        case SubGhzSpiClock:
            enr_bit = SubGhzSpiEnBit;
            enable_register = Apb3Enr;
            break;

        default: return;
    }

    rccSetRegisterBits(rcc, enable_register, enr_bit);
}

// void unset_reg_bits(uint32_t* reg, uint32_t bits) {}
// write_reg_bits()


