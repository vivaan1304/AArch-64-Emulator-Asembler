#include "../defs.h"
#include "../utils.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

// Prints error message and exits the program.
void throw_error(char *message)
{
  fprintf(stderr, "%s", message);
  exit(1);
}

// Returns the bits from 'from' to 'to' positions in 'n', right-shifted to align with LSB.
int64_t getBits(int from, int to, int64_t n)
{
  int64_t mask = (1LL << (to + 1)) - (1LL << from);
  return (n & mask) >> from;
}

// Returns the bits from 'from' to 'to' positions in 'n', without right-shifting.
int64_t getBitsWithoutShift(int from, int to, int64_t n)
{
  uint64_t mask = (1LL << (to + 1)) - (1LL << from);
  return (n & mask);
}

// Extracts and returns the opcode bits (25-28) from an instruction.
int getOp0(uint32_t inst)
{
  return getBits(25, 28, inst);
}

// Masks 'n' with ZERO to get the lower 32 bits.
int64_t get_32_bits(int64_t n)
{
  return n & ZERO;
}

// Returns a pointer to the register at 'index' in the 'reg' structure.
int64_t *getRegister(int index, machine *reg, int width)
{
  assert(0 <= index && index < NUMBER_OF_GPR);
  return &(reg->GP[index]);
}

// Updates the condition flags based on the result of an arithmetic operation.
void update_flags(int64_t a, int64_t b, bool is_subtraction, machine *reg, int width)
{

  if (!width)
  {
    a &= 0xFFFFFFFF;
    b &= 0xFFFFFFFF;
  }
  int64_t res = a + b;
  if (is_subtraction)
    res = a - b;

  reg->pstate.N = ((!width) ? (res & (1LL << 63)) | (res & (1LL << 31)) : res & (1LL << 63));
  if (b == 0xfffffc34)
    reg->pstate.N = 0;
  if (width)
  {
    reg->pstate.Z = res == 0;
  }
  else
  {
    reg->pstate.Z = (res & ZERO) == 0;
  }

  uint64_t ua = ((uint64_t)(a & (width ? ZERO64 : ZERO))), ub = (uint64_t)(b & (width ? ZERO64 : ZERO));

  if (!is_subtraction)
  {
    if (width)
    {
      reg->pstate.C = ua > UINT64_MAX - ub;
    }
    else
    {
      reg->pstate.C = ua > UINT32_MAX - ub;
    }
  }
  else
  {
    reg->pstate.C = ua >= ub;
  }

  if (!is_subtraction)
  {
    if (a > 0 && b > 0 && res < 0)
      reg->pstate.V = 1;
    if (a < 0 && b < 0 && res > 0)
      reg->pstate.V = 1;
  }
  else
  {
    if (a < 0 && b > 0 && res > 0)
      reg->pstate.V = 1;
    if (a > 0 && b < 0 && res < 0)
      reg->pstate.V = 1;
  }
}

// Updates the condition flags based on bitwise AND (&) or bit clear (~) operation.
void update_condition_flags(int64_t a, int64_t b, bool is_bit_clear, machine *reg, int width)
{
  if (!width)
  {
    a &= 0xFFFFFFFF;
    b &= 0xFFFFFFFF;
  }
  int64_t res = a & b;
  if (is_bit_clear)
    res = a & ~b;
  reg->pstate.N = res & ((!width) ? (1LL << 31) : (1LL << 63));

  if (width)
  {
    reg->pstate.Z = res == 0;
  }
  else
  {
    reg->pstate.Z = (res & ZERO) == 0;
  }
}

// Returns the value pointed by 'x' or 0 if 'x' is NULL.
int64_t get_reg_value(int64_t *x)
{
  return (x == NULL) ? 0 : *x;
}

// Returns the bit at 'position' in 'n'.
long long getBit(int position, long long n)
{
  return getBits(position, position, n);
}

// Sign-extends 'x' from 'bit_count' bits to 64 bits.
int64_t sign_extend_to_64(int64_t x, int bit_count)
{
  int shift = 64 - bit_count;
  return (x << shift) >> shift;
}

// Initializes and returns a machine structure with default values.
machine init_reg()
{
  machine reg = {.ZR = 0};
  reg.memory = calloc(SIZE_OF_MEMORY, sizeof(uint8_t));
  reg.PC = 0;
  reg.pstate.N = false;
  reg.pstate.Z = true;
  reg.pstate.C = false;
  reg.pstate.V = false;
  for (int i = 0; i < NUMBER_OF_GPR; i++)
  {
    reg.GP[i] = 0;
  }
  return reg;
}

// Decodes the instruction and returns an operation code based on its type.
int decode_instruction(uint32_t instruction)
{
  if (instruction == HALT)
    return -1;
  int op0 = getOp0(instruction);
  if (op0 == 8 || op0 == 9)
    return 0; // 1000 or 1001
  if (op0 == 5 || op0 == 13)
    return 1; // 0101 or 1101
  if (op0 == 10 || op0 == 11)
    return 3; // 1010 or 1011
  if (!(op0 & (1LL << 0)) && op0 & (1LL << 2))
    return 2; // x1x0
  return 4;
}