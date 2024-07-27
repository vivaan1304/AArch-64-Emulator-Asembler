// shifts.c

// This file contains functions for performing the several bitwise and arithmetic operations
// such as logical shifts, arithmetic shifts, and rotations. It also includes a
// function to handle register operations based on specific instruction encodings.

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "../instructions.h"
#include "../utils.h"
#include "../defs.h"

// Called as: (u)int64_t result = lsl(input, amount, mode);
// Logical shift left
// Performs a logical shift left on the input value by the specified "amount".
// `mode` determines whether the operation is in 32-bit (false) or 64-bit (true) mode.
uint64_t lsl(uint64_t input, int amount, bool mode)
{
  uint64_t modeMask = mode ? MODE64 : MODE32; // select appropriate mask based on appropriate mode
  return (input << amount) & modeMask;
}

// Called as: (u)int64_t result = lsr(input, amount, mode);
// Logical shift right
// Performs a logical shift right on the input value by the specified amount.
// `mode` determines whether the operation is in 32-bit (false) or 64-bit (true) mode.
uint64_t lsr(uint64_t input, int amount, bool mode)
{
  uint64_t modeMask = mode ? MODE64 : MODE32;
  return (input >> amount) & modeMask;
}

// Arithmetic shift right
// Performs an arithmetic shift right on the input value by the specified amount.
// `mode` determines whether the operation is in 32-bit (false) or 64-bit (true) mode.
uint64_t asr(uint64_t input, int amount, bool mode)
{
  uint64_t modeMask = mode ? MODE64 : MODE32;
  uint64_t signMask = mode ? SIGN64 : SIGN32;

  uint64_t sign = input & signMask;        // extract the sign bit (MSB)
  uint64_t intermediate = input >> amount; // perform initial shift
  for (int i = 0; i < amount; i++)
  {
    intermediate = intermediate | sign;
    sign = sign >> 1; // for each bit shifted, extend the MSB!
  }
  return intermediate & modeMask;
}

// Called as: uint64_t result = ror(input, amount, mode);
// Rotate right
// Performs a rotate right operation on the input value by the amount above.
// `mode` determines whether the operation is in 32-bit (false) or 64-bit (true) mode.
uint64_t ror(uint64_t input, int amount, bool mode)
{
  uint64_t modeMask = mode ? MODE64 : MODE32;

  // uint64_t rotateMask = (1 << amount) - 1;
  // create mask for bits that will be rotated
  uint64_t rotateMask = 1;
  rotateMask = rotateMask << amount;
  rotateMask--;

  int bitWidth = mode ? 64 : 32;
  // determine bit width based on model

  uint64_t intermediate = (input & modeMask) >> amount;
  uint64_t rotatedBitsMovedLeft = (input & rotateMask) << (bitWidth - amount);
  intermediate = intermediate | rotatedBitsMovedLeft;
  // combine rotated bits with shifted input
  return intermediate;
}

// Called as: int64_t result = apply_shift(rm, shift_code, shift_amount, width);
// Apply shift
// Applies a shift operation specified by shift_code on the value rm by shift_amount.
// shift_code: 0 for LSL, 1 for LSR, 2 for ASR, 3 for ROR
// width determines whether the operation is in 32-bit (0) or 64-bit (1) mode.

int64_t apply_shift(int64_t rm, int shift_code, int shift_amount, int width)
{
  int64_t result;

  switch (shift_code)
  {
  case 0: // LSL
    result = lsl(rm, shift_amount, width);
    break;

  case 1: // LSR
    result = lsr(rm, shift_amount, width);
    break;

  case 2: // ASR
    result = asr(rm, shift_amount, width);
    break;

  case 3: // ROR
    result = ror(rm, shift_amount, width);
    break;
  }

  if (!width)
    result &= ZERO;
  return result;
}

// Called as: handle_register(&reg, inst);
// Handle register
// Handles various register operations based on the instruction encoding.
// Decodes the instruction and performs the corresponding arithmetic or logical operation
// as executed by the functions defined above.
void handle_register(machine *reg, uint32_t inst)
{
  // Split up the instruction into its respective form
  int op0 = getOp0(inst);
  assert(op0 == 5 || op0 == 13);
  // ensures that bits 28-25 are M101 for DP (register)
  int sf = getBits(31, 31, inst);
  int opc = getBits(29, 30, inst);
  int M = getBits(28, 28, inst);
  int opr = getBits(21, 24, inst);
  int rm_index = getBits(16, 20, inst);
  int operand = getBits(10, 15, inst);
  int rn_index = getBits(5, 9, inst);
  int rd_index = getBits(0, 4, inst);

  int ARITHMETIC_MASK = 0x9;
  int BIT_MASK = 0x8;

  // Initialise the register pointers
  int64_t *rn = NULL;
  int64_t *rd = NULL;
  int64_t *rm = NULL;

  if (rn_index != NUMBER_OF_GPR)
  {
    rn = getRegister(rn_index, reg, 1);
  }
  if (rd_index != NUMBER_OF_GPR)
  {
    rd = getRegister(rd_index, reg, sf);
  }
  if (rm_index != NUMBER_OF_GPR)
  {
    rm = getRegister(rm_index, reg, 1);
  }

  // Handle Arithmetic and Logical operations
  if (M == 0)
  {
    assert((opr & ARITHMETIC_MASK) == 8 || (opr & BIT_MASK) == 0);
    // ensure valid operations, using the assert
    bool is_arithmetic = ((opr & ARITHMETIC_MASK) == 8);
    // check if the operation is arithmetic 1xx0 (BYTE = 8
    int64_t op2;
    op2 = apply_shift(get_reg_value(rm), getBits(22, 23, inst), operand, sf);

    if (is_arithmetic)
    {
      // handle arithmetic operations based on opc
      switch (opc)
      {
      case 0: // Add
        if (rd != NULL)
          *rd = get_reg_value(rn) + op2;
        break;

      case 1: // Adds
        update_flags(get_reg_value(rn), op2, 0, reg, sf);
        if (rd != NULL)
          *rd = get_reg_value(rn) + op2;
        break;

      case 2: // Sub
        if (rd != NULL)
          *rd = get_reg_value(rn) - op2;
        break;

      case 3: // Subs
        update_flags(get_reg_value(rn), op2, 1, reg, sf);
        if (rd != NULL)
          *rd = get_reg_value(rn) - op2;
        break;

      default:
        throw_error("Invalid Command\n");
        break;
      }

      // Apply 32-bit mask, if in 32-bit mode
      if (!sf)
      {
        if (rd != NULL)
          *rd &= ZERO;
      }
    }

    else
    {
      int N = getBits(21, 21, inst);
      // Handle Logical instructions
      switch (opc)
      {
      case 0:
        // AND
        if (rd != NULL)
          *rd = N ? (get_reg_value(rn) & ~op2) : (get_reg_value(rn) & op2);
        break;
      case 1:
        // OR
        if (rd != NULL)
          *rd = N ? (get_reg_value(rn) | (~op2)) : (get_reg_value(rn) | op2);
        break;
      case 2:
        // XOR
        if (rd != NULL)
          *rd = N ? (get_reg_value(rn) ^ (~op2)) : (get_reg_value(rn) ^ op2);
        break;
      case 3:
        // AND+BICS
        if (!N)
        {
          update_condition_flags(get_reg_value(rn), op2, 0, reg, sf);
          if (rd != NULL)
            *rd = get_reg_value(rn) & op2;
        }
        else
        {
          update_condition_flags(get_reg_value(rn), op2, 1, reg, sf);
          if (rd != NULL)
            *rd = get_reg_value(rn) & (~op2);
        }
        break;
      }

      if (!sf)
      {
        if (rd != NULL)
          *rd &= ZERO;
      }
    }
  }

  // GROUP Multiply
  else if (M == 1)
  {
    assert((opr & BIT_MASK) == 8);

    int x = getBits(15, 15, inst);
    int64_t *ra = NULL;
    int ra_index = getBits(10, 14, inst);
    if (ra_index != 31)
    {
      ra = getRegister(ra_index, reg, 1);
    }

    if (x == 0)
    { // madd
      if (rd != NULL)
        *rd = get_reg_value(ra) + (get_reg_value(rn) * get_reg_value(rm));
    }
    else
    { // msub
      if (rd != NULL)
        *rd = get_reg_value(ra) - (get_reg_value(rn) * get_reg_value(rm));
    }
    if (!sf)
    {
      if (rd != NULL)
      {
        *rd &= ZERO;
      }
    }
  }
}
