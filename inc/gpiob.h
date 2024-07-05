#ifndef GPIOB_H
#define GPIOB_H

#include "gpiob-moder.h"


// GPIOB Register Block
typedef struct GPIOB {
    // 0x00 - GPIO port mode register
    GPIOB_MODER moder;
} GPIOB;



void reset_GPIOB_MODER();

#endif
