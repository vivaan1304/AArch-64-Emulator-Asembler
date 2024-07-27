#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "defs.h"

// Function prototypes:

void handle_branch(machine *reg, uint32_t inst);
void handle_register(machine *reg, uint32_t inst);
void handle_sdti(machine *reg, uint32_t inst);
void handle_immediate(machine *reg, uint32_t inst);
#endif