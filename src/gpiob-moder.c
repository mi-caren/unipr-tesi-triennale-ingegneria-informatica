#include "gpiob-moder.h"


// READABLE

static GPIOB_MODER_MODE15_R readGpiobModerMode15(Register moder) {
    return (GPIOB_MODER_MODE15_R) ( (moder >> 30) & 0b11 );
}
static GPIOB_MODER_MODE14_R readGpiobModerMode14(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE14_R) ( (moder >> 28) & 0b11 );
}
static GPIOB_MODER_MODE13_R readGpiobModerMode13(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE13_R) ( (moder >> 26) & 0b11 );
}
static GPIOB_MODER_MODE12_R readGpiobModerMode12(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE12_R) ( (moder >> 24) & 0b11 );
}
static GPIOB_MODER_MODE11_R readGpiobModerMode11(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE11_R) ( (moder >> 22) & 0b11 );
}
static GPIOB_MODER_MODE10_R readGpiobModerMode10(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE10_R) ( (moder >> 20) & 0b11 );
}
static GPIOB_MODER_MODE9_R readGpiobModerMode9(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE9_R) ( (moder >> 18) & 0b11 );
}
static GPIOB_MODER_MODE8_R readGpiobModerMode8(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE8_R) ( (moder >> 16) & 0b11 );
}
static GPIOB_MODER_MODE7_R readGpiobModerMode7(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE7_R) ( (moder >> 14) & 0b11 );
}
static GPIOB_MODER_MODE6_R readGpiobModerMode6(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE6_R) ( (moder >> 12) & 0b11 );
}
static GPIOB_MODER_MODE5_R readGpiobModerMode5(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE5_R) ( (moder >> 10) & 0b11 );
}
static GPIOB_MODER_MODE4_R readGpiobModerMode4(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE4_R) ( (moder >> 8) & 0b11 );
}
static GPIOB_MODER_MODE3_R readGpiobModerMode3(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE3_R) ( (moder >> 6) & 0b11 );
}
static GPIOB_MODER_MODE2_R readGpiobModerMode2(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE2_R) ( (moder >> 4) & 0b11 );
}
static GPIOB_MODER_MODE1_R readGpiobModerMode1(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE1_R) ( (moder >> 2) & 0b11 );
}
static GPIOB_MODER_MODE0_R readGpiobModerMode0(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE0_R) ( (moder >> 0) & 0b11 );
}

static GPIOB_MODER_R gpiob_moder_r = {
    .mode15 = readGpiobModerMode15,
    .mode14 = readGpiobModerMode14,
    .mode13 = readGpiobModerMode13,
    .mode12 = readGpiobModerMode12,
    .mode11 = readGpiobModerMode11,
    .mode10 = readGpiobModerMode10,
    .mode9  = readGpiobModerMode9,
    .mode8  = readGpiobModerMode8,
    .mode7  = readGpiobModerMode7,
    .mode6  = readGpiobModerMode6,
    .mode5  = readGpiobModerMode5,
    .mode4  = readGpiobModerMode4,
    .mode3  = readGpiobModerMode3,
    .mode2  = readGpiobModerMode2,
    .mode1  = readGpiobModerMode1,
    .mode0  = readGpiobModerMode0,
};

// GPIOB_MODER_R readGpiobModer(GPIOB_MODER moder) {
//     return (GPIOB_MODER_R) {
//         .mode15 = (GPIOB_MODER_MODE15_R) ( (moder >> 30) & 0b11 ),
//         .mode14 = (GPIOB_MODER_MODE14_R) ( (moder >> 28) & 0b11 ),
//         .mode13 = (GPIOB_MODER_MODE13_R) ( (moder >> 26) & 0b11 ),
//         .mode12 = (GPIOB_MODER_MODE12_R) ( (moder >> 24) & 0b11 ),
//         .mode11 = (GPIOB_MODER_MODE11_R) ( (moder >> 22) & 0b11 ),
//         .mode10 = (GPIOB_MODER_MODE10_R) ( (moder >> 20) & 0b11 ),
//         .mode9  = (GPIOB_MODER_MODE9_R)  ( (moder >> 18) & 0b11 ),
//         .mode8  = (GPIOB_MODER_MODE8_R)  ( (moder >> 16) & 0b11 ),
//         .mode7  = (GPIOB_MODER_MODE7_R)  ( (moder >> 14) & 0b11 ),
//         .mode6  = (GPIOB_MODER_MODE6_R)  ( (moder >> 12) & 0b11 ),
//         .mode5  = (GPIOB_MODER_MODE5_R)  ( (moder >> 10) & 0b11 ),
//         .mode4  = (GPIOB_MODER_MODE4_R)  ( (moder >> 8)  & 0b11 ),
//         .mode3  = (GPIOB_MODER_MODE3_R)  ( (moder >> 6)  & 0b11 ),
//         .mode2  = (GPIOB_MODER_MODE2_R)  ( (moder >> 4)  & 0b11 ),
//         .mode1  = (GPIOB_MODER_MODE1_R)  ( (moder >> 2)  & 0b11 ),
//         .mode0  = (GPIOB_MODER_MODE0_R)  ( (moder >> 0)  & 0b11 ),
//     };
// }

GPIOB_MODER_R* gpiobModerReader() {
    return &gpiob_moder_r;
}

bool isInput(GPIOB_MODER_MODE_R mode_r) {
    return mode_r == Input;
}

bool isOutput(GPIOB_MODER_MODE_R mode_r) {
    return mode_r == Output;
}

bool isAlternate(GPIOB_MODER_MODE_R mode_r) {
    return mode_r == Alternate;
}

bool isAnalog(GPIOB_MODER_MODE_R mode_r) {
    return mode_r == Analog;
}


// WRITABLE

void writeGpiobModerMode15Input(GPIOB_MODER moder) {
    return (GPIOB_MODER_MODE15_R) {
        
    };
}

static GPIOB_MODER_W gpiob_moder_w = {
    .mode15 = writeGpiobModerMode15Input(GPIOB_MODER moder)
};

GPIOB_MODER_W* writeGpiobModer() {
    return &gpiob_moder_w;
}

// RESETTABLE
void resetGpiobModer(GPIOB_MODER* moder) {
    *moder = GPIOB_MODER_RESET_VALUE;
}


