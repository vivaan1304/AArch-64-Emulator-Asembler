#include "../defs.h"
#include "../utils.h"
#include "../instructions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t assemble_s(char **tokens)
{
  return return_imm(tokens[1]);
}

uint32_t assemble_line(char **tokens, SymbolTable symtab, uint64_t PC)
{
  char *opcode = tokens[0];
  if (in_array(opcode, DP_INST))
  {
    return assemble_dp(tokens);
  }
  else if (in_array(opcode, branch_inst))
  {
    return (assemble_branch(tokens, symtab, PC));
  }
  else if (in_array(opcode, ls_inst))
  {
    return (assemble_stdi(tokens, symtab, PC));
  }
  else if (strncmp(opcode, ".int", 4) == 0)
  {
    return (assemble_s(tokens));
  }
  // default case for invalid operation
  printf("invalid opcode");
  exit(EXIT_FAILURE);
}