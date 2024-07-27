#ifndef UTILS_H
#define UTILS_H
#include "defs.h"

// Function prototypes:

void throw_error(char *);
int64_t getBits(int from, int to, int64_t n);
int64_t getBitsWithoutShift(int from, int to, int64_t n);
int getOp0(uint32_t inst);
int64_t get_32_bits(int64_t n);
int64_t *getRegister(int index, machine *reg, int width);
void update_flags(int64_t a, int64_t b, bool is_subtraction, machine *reg, int width);
void update_condition_flags(int64_t a, int64_t b, bool is_bit_clear, machine *reg, int width);
int64_t get_reg_value(int64_t *x);
int64_t sign_extend_to_64(int64_t x, int bit_count);
long long getBit(int position, long long n);
void print_reg(machine reg);
void write_to_file(FILE *out, machine reg);
machine init_reg();
int decode_instruction(uint32_t instruction);
#endif
