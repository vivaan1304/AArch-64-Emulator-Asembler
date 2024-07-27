#include "include_emulator/utils.h"
#include "include_emulator/defs.h"
#include "include_emulator/instructions.h"
#include <stdlib.h>

// Decode the instruction into its respective group and return
// -1: HALT
// 0: imm
// 1: reg
// 2: loads+stores
// 3: Branches
// 4: ERROR

void execute_phase(machine *reg, long number_of_instructions, uint32_t instructions[])
{
  uint32_t inst = instructions[0];
  int code = decode_instruction(inst);

  while (code != -1)
  {
    switch (code)
    {
    case 0:
      handle_immediate(reg, inst);
      reg->PC += 4;
      break;
    case 1:
      handle_register(reg, inst);
      reg->PC += 4;
      break;
    case 2:
      handle_sdti(reg, inst);
      reg->PC += 4;
      break;
    case 3:
      handle_branch(reg, inst);
      break;
    default:
      throw_error("INVALID INSTRUCTION\n");
      break;
    }
    if (reg->PC / 4 > number_of_instructions)
      break;
    inst = instructions[reg->PC / 4];
    code = decode_instruction(inst);
  }
}

// Main function to execute the emulator
int main(int argc, char **argv)
{

  // takes in input file(bin) and (output file)
  if (argc < 2)
    throw_error("Please input the binary file\n");

  FILE *in = fopen(argv[1], "rb");
  if (in == NULL)
    throw_error("Can't open the file\n");

  fseek(in, 0L, SEEK_END);
  long size = ftell(in);
  rewind(in);

  int ch;
  int arr[size];
  int pos = 0;

  machine reg = init_reg();
  while ((ch = getc(in)) != EOF)
  {
    reg.memory[pos] = ch;
    arr[pos++] = ch;
  }

  long numberOfInstructions = size / 4;
  uint32_t instructions[numberOfInstructions];

  pos = 0;
  for (int i = 0; i < size; i += 4)
  {
    uint32_t instruction = 0;
    uint32_t bs = 1;
    for (int j = i; j < i + 4; j++)
    {
      instruction += (arr[j] * bs);
      bs <<= BYTE;
    }
    instructions[pos++] = instruction;
  }

  execute_phase(&reg, numberOfInstructions, instructions);
  if (argc == 3)
  {
    FILE *out = fopen(argv[2], "w");
    if (out == NULL)
      throw_error("Cant open file");
    write_to_file(out, reg);
    return 0;
  }

  print_reg(reg);
  free(reg.memory);
  return 0;
}
