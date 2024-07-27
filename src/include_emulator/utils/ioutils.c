#include <stdlib.h>
#include <stdio.h>
#include "../utils.h"
#include "../defs.h"

// Function to print the machine and non-zero memory
void print_reg(machine reg)
{

  printf("machine:\n");
  for (int i = 0; i < NUMBER_OF_GPR; i++)
  {
    if (i < 10)
    {
      printf("X0%d = %16.16lx\n", i, reg.GP[i]);
    }
    else
    {
      printf("X%d = %16.16lx\n", i, reg.GP[i]);
    }
  }

  printf("PC = %16.16lx", reg.PC);

  printf("\nPSTATE : ");
  printf("%c", reg.pstate.N ? 'N' : '-');
  printf("%c", reg.pstate.Z ? 'Z' : '-');
  printf("%c", reg.pstate.C ? 'C' : '-');
  printf("%c\n", reg.pstate.V ? 'V' : '-');

  printf("Non-zero memory:\n");
  for (int i = 0; i < SIZE_OF_MEMORY; i += 4)
  {
    if (reg.memory[i] != 0 || reg.memory[i + 1] != 0 || reg.memory[i + 2] != 0 || reg.memory[i + 3] != 0)
    {
      uint32_t instruction = 0;
      uint32_t bs = 1;
      for (int j = i; j < i + 4; j++)
      {
        instruction += (reg.memory[j] * bs);
        bs <<= BYTE;
      }
      printf("0x%08x: %08x\n", i, instruction);
    }
  }
}

// Writes the contents of the machine and memory to a file.
void write_to_file(FILE *out, machine reg)
{

  fputs("machine:\n", out);
  for (int i = 0; i < NUMBER_OF_GPR; i++)
  {
    char output[100];
    if (i < 10)
    {
      sprintf(output, "X0%d = %16.16lx\n", i, reg.GP[i]);
    }
    else
    {
      sprintf(output, "X%d = %16.16lx\n", i, reg.GP[i]);
    }
    fputs(output, out);
  }

  char output[100];
  sprintf(output, "PC = %16.16lx", reg.PC);
  fputs(output, out);

  fputs("\nPSTATE : ", out);
  fputs(reg.pstate.N ? "N" : "-", out);
  fputs(reg.pstate.Z ? "Z" : "-", out);
  fputs(reg.pstate.C ? "C" : "-", out);
  fputs(reg.pstate.V ? "V" : "-\n", out);

  fputs("Non-zero memory:\n", out);
  for (int i = 0; i < SIZE_OF_MEMORY; i += 4)
  {
    if (reg.memory[i] != 0 || reg.memory[i + 1] != 0 || reg.memory[i + 2] != 0 || reg.memory[i + 3] != 0)
    {
      uint32_t instruction = 0;
      uint32_t bs = 1;
      for (int j = i; j < i + 4; j++)
      {
        instruction += (reg.memory[j] * bs);
        bs <<= BYTE;
      }
      char output2[100];
      sprintf(output2, "0x%08x: %08x\n", i, instruction);
      fputs(output2, out);
    }
  }

  if (ferror(out))
  {
    fprintf(stderr, "Error occurred writing to output.txt\n");
    exit(1);
  }
  fclose(out);
}