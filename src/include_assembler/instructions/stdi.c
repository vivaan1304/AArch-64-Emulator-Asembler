#include "../utils.h"
#include "../instructions.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint32_t handle_sdt(char **tokens)
{
  tokens[2]++;
  uint32_t mask = 1 << 31;
  char *endptr;
  uint32_t result = set_width(0, tokens) >> 1; // as sf is bit 30 not 31 here
  result |= mask;                              // bits 30- 31
  result = set_regs(result, tokens);           // bits 0-9
  if (strcmp(*tokens, "ldr") == 0)
  {
    mask = 1 << 22;
    result |= mask; // bit 22
  }
  mask = 7 << 27;
  result |= mask; // bits 25 - 29
  // bit 23 is 0
  // below are the different cases of sdti
  if (tokens[3] == NULL || (*tokens[3] == '#' && get_last_char(tokens[3]) == ']'))
  { // case for unsigned offset
    mask = return_imm(tokens[3]);
    mask = (*tokens[2] == 'x') ? mask / 8 : mask / 4;
    result |= mask << 10; // bits 10-21
    mask = 1 << 24;       // bit 24 U
    result |= mask;
  }
  else if (*tokens[3] != '#' && get_last_char(tokens[3]) == ']')
  { // case for register offset
    mask = 1 << 21;
    result |= mask; // bit 21
    mask = (uint32_t)strtoul(tokens[3] + 1, &endptr, 10) << 16;
    result |= mask;    // bits 16 - 20
    mask = 0x1A << 10; // 011010 in biary
    result |= mask;    // bits 10 - 15
  }
  else
  { // pre/ post index case
    if (get_last_char(tokens[3]) == '!')
    { // pre indexed
      mask = 3 << 10;
    }
    else
    {
      mask = 1 << 10; // post indexed
    }
    result |= mask; // bits 10 - 11
    // bit 21 need not be set at it is already 0
    mask = signed_nbit(return_imm(tokens[3]), 9) << 12; // converts to a simm9

    result |= mask; // bits 12- 20
    // bit 21 is 0
  }
  return result;
}

uint32_t handle_litLoad(char **tokens, SymbolTable symtab, uint64_t PC)
{
  int offset = 0;
  uint32_t result = set_width(0, tokens) >> 1; // as sf is bit 30 not 31 here
  uint32_t mask = atoi(tokens[1] + 1);
  result |= mask; // bits 0-4
  mask = 3 << 27;
  result |= mask; // bits 24-29 31
  if (*tokens[2] == '#')
  { // literal integer
    offset = (return_imm(tokens[2]) - PC) / 4;
  }
  else
  { // label
    offset = (lookup_symbol(symtab, tokens[2]) - PC) / 4;
  }
  mask = signed_nbit(offset, 19) << 5;
  result |= mask; // bits 5 -23
  return result;
}

uint32_t assemble_stdi(char **tokens, SymbolTable symtab, uint64_t PC)
{
  if (*tokens[2] != '[')
  {
    return (handle_litLoad(tokens, symtab, PC));
  }
  else
  {
    return (handle_sdt(tokens));
  }
}