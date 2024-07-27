#include <assert.h>
#include <stdint.h>
#include "../instructions.h"
#include "../utils.h"
#include "../defs.h"
#include <stdbool.h>

/*
 * Handles immediate instructions.
 *
 * This function takes in a pointer to the machine structure and an instruction,
 * and performs the necessary operations based on the instruction type.
 */

void handle_immediate(machine *reg, uint32_t inst)
{
  int op0 = getOp0(inst);
  // assert the op0 is indeed the one we need for immediate instructions
  assert(op0 == 8 || op0 == 9);

  // extract the different bits we need for instrcutions
  int opi = getBits(23, 25, inst);
  int opc = getBits(29, 30, inst);
  int sf = getBits(31, 31, inst);
  int rd_index = getBits(0, 4, inst);

  if (opi == 2)
  {
    // get the register value and the immediate12 value
    int64_t *rn = getRegister(getBits(5, 9, inst), reg, sf);
    int imm12 = getBits(10, 21, inst);
    int sh = getBits(22, 22, inst);
    if (sh)
    {
      imm12 <<= 12;
    }

    int64_t *rd;
    bool is_zr = false;
    if (rd_index != NUMBER_OF_GPR)
      rd = getRegister(rd_index, reg, sf);
    else
      is_zr = true;

    switch (opc)
    {
    case 0:
      if (!is_zr)
        // addition
        *rd = *rn + imm12;
      break;

    case 1:
      update_flags(*rn & (sf ? ZERO64 : ZERO), imm12, 0, reg, sf);
      if (!is_zr)
        // addition with flags
        *rd = *rn + imm12;
      break;

    case 2:
      if (!is_zr)
        // subtraction
        *rd = *rn - imm12;
      break;

    case 3:
      update_flags(*rn & (sf ? ZERO64 : ZERO), imm12, 1, reg, sf);
      if (!is_zr)
        // subtraction with flags
        *rd = *rn - imm12;
      break;

    default:
      throw_error("Invalid Command\n");
      break;
    }

    if (!sf && !is_zr)
      *rd = get_32_bits(*rd);
  }

  else
  {
    // handle the mov cases
    assert(opi == 5);

    int64_t imm16 = getBits(5, 20, inst);
    int hw = getBits(21, 22, inst);
    int shift = hw * 16;
    int64_t op = imm16 << shift;

    int64_t *rd;
    bool is_zr = false;
    if (rd_index != NUMBER_OF_GPR)
      rd = getRegister(rd_index, reg, sf);
    else
      is_zr = true;

    switch (opc)
    {
    case 0:
      if (!is_zr)
        // movn
        *rd = ~op;
      break;

    case 2:
      if (!is_zr)
        // movz
        *rd = op;
      break;

    case 3:
      // movk
      if (!is_zr)
      {
        uint64_t mask = imm16;
        mask <<= shift;
        int64_t res = getBitsWithoutShift(shift + 16, 31, *rd);
        if (sf)
        {
          res = getBitsWithoutShift(shift + 16, 63, *rd);
        }
        res |= mask;
        res |= getBitsWithoutShift(0, shift - 1, *rd);
        *rd = res;
      }
      break;

    default:
      throw_error("INVALID COMMAND\n");
    }

    if (!sf)
    {
      *rd &= ZERO;
    }
  }
}