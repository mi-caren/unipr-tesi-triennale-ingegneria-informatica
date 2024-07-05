#ifndef GPIOB_MODER_H
#define GPIOB_MODER_H

#include <stdbool.h>
#include <stdint.h>


#include "generics.h"


typedef enum GPIOB_MODER_MODE_A {
    Input = 0,
    Output = 1,
    Alternate = 2,
    Analog = 3,
} GPIOB_MODER_MODE_A;

typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE15_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE14_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE13_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE12_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE11_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE10_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE9_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE8_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE7_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE6_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE5_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE4_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE3_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE2_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE1_A;
typedef GPIOB_MODER_MODE_A GPIOB_MODER_MODE0_A;


// READABLE
// typedef uint8_t GPIOB_MODER_MODE_R;
typedef struct GPIOB_MODER_MODE_R {
    uint8_t            (*bits)        (Register moder);
    GPIOB_MODER_MODE_A (*variant)     (Register moder);
    bool               (*isInput)     (Register moder);
    bool               (*isOutput)    (Register moder);
    bool               (*isAlternate) (Register moder);
    bool               (*isAnalog)    (Register moder);
} GPIOB_MODER_MODE_R;

typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE15_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE14_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE13_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE12_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE11_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE10_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE9_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE8_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE7_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE6_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE5_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE4_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE3_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE2_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE1_R;
typedef GPIOB_MODER_MODE_R GPIOB_MODER_MODE0_R;

typedef struct GPIOB_MODER_R {
    GPIOB_MODER_MODE15_R (*mode15) ();
    GPIOB_MODER_MODE14_R (*mode14) ();
    GPIOB_MODER_MODE13_R (*mode13) ();
    GPIOB_MODER_MODE12_R (*mode12) ();
    GPIOB_MODER_MODE11_R (*mode11) ();
    GPIOB_MODER_MODE10_R (*mode10) ();
    GPIOB_MODER_MODE9_R  (*mode9)  ();
    GPIOB_MODER_MODE8_R  (*mode8)  ();
    GPIOB_MODER_MODE7_R  (*mode7)  ();
    GPIOB_MODER_MODE6_R  (*mode6)  ();
    GPIOB_MODER_MODE5_R  (*mode5)  ();
    GPIOB_MODER_MODE4_R  (*mode4)  ();
    GPIOB_MODER_MODE3_R  (*mode3)  ();
    GPIOB_MODER_MODE2_R  (*mode2)  ();
    GPIOB_MODER_MODE1_R  (*mode1)  ();
    GPIOB_MODER_MODE0_R  (*mode0)  ();
} GPIOB_MODER_R;

uint8_t bits(Register moder);
GPIOB_MODER_MODE_A variant(Register moder);
bool isInput(Register moder);
bool isOutput(Register moder);
bool isAlternate(Register moder);
bool isAnalog(Register moder);

GPIOB_MODER_R* gpiobModerReader();


// WRITABLE

typedef struct GPIOB_MODER_MODE_W {
    void (*input)     ();
    void (*output)    ();
    void (*alternate) ();
    void (*analog)    ();
} GPIOB_MODER_MODE_W;

typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE15_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE14_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE13_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE12_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE11_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE10_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE9_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE8_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE7_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE6_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE5_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE4_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE3_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE2_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE1_W;
typedef GPIOB_MODER_MODE_W GPIOB_MODER_MODE0_W;

typedef struct GPIOB_MODER_W {
    GPIOB_MODER_MODE15_W (*mode15) (Register moder);
    GPIOB_MODER_MODE14_W (*mode14) (Register moder);
    GPIOB_MODER_MODE13_W (*mode13) (Register moder);
    GPIOB_MODER_MODE12_W (*mode12) (Register moder);
    GPIOB_MODER_MODE11_W (*mode11) (Register moder);
    GPIOB_MODER_MODE10_W (*mode10) (Register moder);
    GPIOB_MODER_MODE9_W  (*mode9)  (Register moder);
    GPIOB_MODER_MODE8_W  (*mode8)  (Register moder);
    GPIOB_MODER_MODE7_W  (*mode7)  (Register moder);
    GPIOB_MODER_MODE6_W  (*mode6)  (Register moder);
    GPIOB_MODER_MODE5_W  (*mode5)  (Register moder);
    GPIOB_MODER_MODE4_W  (*mode4)  (Register moder);
    GPIOB_MODER_MODE3_W  (*mode3)  (Register moder);
    GPIOB_MODER_MODE2_W  (*mode2)  (Register moder);
    GPIOB_MODER_MODE1_W  (*mode1)  (Register moder);
    GPIOB_MODER_MODE0_W  (*mode0)  (Register moder);
} GPIOB_MODER_W;

GPIOB_MODER_W* writeGpiobModer();


typedef struct GPIOB_MODER {
    Register ptr;
    GPIOB_MODER_R* (*read)();
} GPIOB_MODER;
// RESETTABLE

#define GPIOB_MODER_RESET_VALUE    0xfffffebf

void resetGpiobModer(GPIOB_MODER* moder);

#endif
