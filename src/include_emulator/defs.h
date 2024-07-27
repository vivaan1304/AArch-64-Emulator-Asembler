#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// Definitions here for constants:

#define NUMBER_OF_GPR 31
#define BYTE 8
#define WORD 32
#define REGSIZE 64
#define NEXT_INST_SERIALLY 4
#define ZERO_REG 31
#define INST_SIZE 4
#define HALT 0x8a000000
#define SIZE_OF_MEMORY (1 << 21)
#define ZERO 0x000000000ffffffff
#define ZERO64 0xffffffffffffffff
#define SIGN64 0x8000000000000000
#define SIGN32 0x0000000080000000
#define MODE64 0xffffffffffffffff
#define MODE32 0x00000000ffffffff

// Definitions here for structs used in the code:

// PSTATE fields implemented using a Struct
typedef struct
{
    bool N;
    bool Z;
    bool C;
    bool V;
} PSTATE;

// Complete combination of all elements of the machine
typedef struct
{
    uint8_t *memory;
    int64_t GP[NUMBER_OF_GPR];
    const int64_t ZR;
    uint64_t PC;
    PSTATE pstate;
} machine;

#endif