#include <stdint.h>
#include <assert.h>
#include "../instructions.h"
#include "../utils.h"
#include "../defs.h"

// Function to handle branch instructions
void handle_branch(machine *reg, uint32_t inst)
{
  int opt = getBits(30, 31, inst);
  switch (opt)
  {
  case 0: // Unconditional Jump
    reg->PC += sign_extend_to_64(getBits(0, 25, inst), 26) * INST_SIZE;
    break;

  case 1: // beq, check the conditions and jump accordingly
  {
    int code = getBits(0, 3, inst); // the code for the type of comparison
    int offset = sign_extend_to_64(getBits(5, 23, inst), 19) * INST_SIZE;
    switch (code)
    {
    case 0: // EQ - equal
      reg->PC += (reg->pstate.Z) ? offset : NEXT_INST_SERIALLY;
      break;
    case 1: // NE - not equal
      reg->PC += (!reg->pstate.Z) ? offset : NEXT_INST_SERIALLY;
      break;
    case 10: // GE - signed greater or equal
      reg->PC += (reg->pstate.Z == reg->pstate.N) ? offset : NEXT_INST_SERIALLY;
      break;
    case 11: // LT - signed less than
      reg->PC += (reg->pstate.Z != reg->pstate.N) ? offset : NEXT_INST_SERIALLY;
      break;
    case 12: // GT - signed greater than
      reg->PC += (reg->pstate.Z == reg->pstate.N && reg->pstate.Z == 0) ? offset : NEXT_INST_SERIALLY;
      break;
    case 13: // LE - signed less than or equal
      reg->PC += (!(reg->pstate.Z == reg->pstate.N && reg->pstate.Z == 0)) ? offset : NEXT_INST_SERIALLY;
      break;
    case 14: // AL - always jump
      reg->PC += offset;
      break;
    default:
      throw_error("Invalid Condition\n");
      break;
    }
  }
  break;

  case 3: // jump to an address held in a register
  {
    int xn = getBits(5, 9, inst);
    assert(xn != ZERO_REG); // as per the spec - to not implement zero register
    reg->PC = reg->GP[xn];
  }
  break;

  default:
    throw_error("Invalid Command\n");
    break;
  }
}