#include "../instructions.h"
#include "../utils.h"
#include "../defs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t assemble_branch(char **tokens, SymbolTable symtab, uint64_t PC)
{
  uint32_t result = 5 << 26; // bits 26 -29
  uint32_t mask = 0;
  int offset = 0;
  if (strcmp(tokens[0], "b") == 0)
  { // unconditional jump
    // no need to set bits 30-31 as are 0
    offset = (lookup_symbol(symtab, tokens[1]) - PC) / 4;
    mask = signed_nbit(offset, 26);
    result |= mask; // bits 0 - 25
  }
  else if (strcmp(tokens[0], "br") == 0)
  { // register jump
    mask = 3 << 30;
    result |= mask;     // bits 30-31
    mask = 0x21F << 16; // 10 0001 1111 in binary
    result |= mask;     // bits 10 - 25 (bits not in mask are 0)
    mask = check_zero(tokens[1]) << 5;
    result |= mask; // bits 0-9 (bits not in mask are 0)
  }
  else
  { // bcond jump
    mask = 1 << 30;
    result |= mask; // bit 30 - 31

    offset = (lookup_symbol(symtab, tokens[1]) - PC) / 4;

    mask = signed_nbit(offset, 19) << 5;
    result |= mask; // bits 4 - 25
    unsigned long cond = hash_string(tokens[0] + 2);
    switch (cond)
    {
    case 49968:
    { // eq -equal
      // result is unchanged at 0
    }
    break;

    case 50001:
    { // ne -not equal
      mask = 1;
      result |= mask;
    }
    break;

    case 49966:
    { // ge -signed greater or equal
      mask = 10;
      result |= mask;
    }
    break;

    case 50006:
    { // lt -signed less than
      mask = 11;
      result |= mask;
    }
    break;

    case 49981:
    { // gt -signed greater than
      mask = 12;
      result |= mask;
    }
    break;

    case 49991:
    { // le -signed less than or equal
      mask = 13;
      result |= mask;
    }
    break;

    case 49943:
    { // al -always
      mask = 14;
      result |= mask;
    }
    break;

    default:
    {
      printf("invalid instruction");
      exit(EXIT_FAILURE);
    }
    }
  }
  return result;
}