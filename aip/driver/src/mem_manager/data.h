#ifndef __DATA_H__
#define __DATA_H__
#include <stdint.h>
/*LowBit*/
typedef struct {
    uint8_t a : 2;
    uint8_t b : 2;
    uint8_t c : 2;
    uint8_t d : 2;
} __ubit2__;

typedef struct {
    uint8_t a : 4;
    uint8_t b : 4;
} __ubit4__;

typedef struct {
    uint8_t a : 6;
    uint8_t n : 2;
} __ubit6__;

typedef struct {
    uint16_t a : 10;
    uint16_t n : 6;
} __ubit10__;

typedef struct {
    int16_t a : 10;
    int16_t n : 6;
} __bit10__;

typedef struct {
    uint16_t a : 12;
    uint16_t n : 4;
} __ubit12__;

typedef struct {
    int16_t a : 12;
    int16_t n : 4;
} __bit12__;

typedef struct {
    uint16_t a : 14;
    uint16_t n : 2;
} __ubit14__;

typedef struct {
    int16_t a : 14;
    int16_t n : 2;
} __bit14__;

typedef union {
    uint8_t b8;
    __ubit2__ b2;
} ubit2_t;

typedef union {
    uint8_t b8;
    __ubit4__ b4;
} ubit4_t;

typedef union {
    uint8_t b8;
    __ubit6__ b6;
} ubit6_t;

typedef union {
    uint16_t b16;
    __ubit10__ b10;
} uint10_t;

typedef union {
    int16_t b16;
    __bit10__ b10;
} int10_t;

typedef union {
    uint16_t b16;
    __ubit12__ b12;
} uint12_t;

typedef union {
    int16_t b16;
    __bit12__ b12;
} int12_t;

typedef union {
    uint16_t b16;
    __ubit14__ b14;
} uint14_t;

typedef union {
    int16_t b16;
    __bit14__ b14;
} int14_t;

typedef uint8_t ubit8_t;

#define POINTER_TO_ADDR(ptr) ((uint32_t)((uintptr_t)(ptr)))
#endif /*__DATA_H__*/
