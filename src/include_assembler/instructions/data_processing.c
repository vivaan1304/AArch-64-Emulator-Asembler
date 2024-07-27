#include "../instructions.h"
#include <stdio.h>
#include <stdlib.h>

uint32_t assemble_dp(char **tokens)
{
  uint32_t result = 0x0;
  uint32_t mask = 0x0;
  unsigned long op_hash = hash_string(*tokens);
  switch (op_hash)
  {
  case 249775:
  { // case for add
    result = handle_arithm(tokens);
  }
  break;

  case 1248990:
  { // case for adds
    result = handle_arithm(tokens);
    mask = 1 << 29;
    result |= mask;
  }
  break;

  case 250308:
  { // sub
    result = handle_arithm(tokens);
    mask = 2 << 29;
    result |= mask;
  }
  break;

  case 1251655:
  { // subs
    result = handle_arithm(tokens);
    mask = 3 << 29;
    result |= mask;
  }
  break;
  case 249825:
  {                                // and
    result = handle_logic(tokens); // as opc and N are 0 nothing needs to be done
  }
  break;

  case 1249240:
  { // ands
    result = handle_logic(tokens);
    mask = 3 << 29;
    result |= mask; // N is 0
  }
  break;

  case 249824:
  { // bic
    result = handle_logic(tokens);
    mask = 1 << 21;
    result |= mask; // opc is 0
  }
  break;

  case 1249235:
  { // bics
    result = handle_logic(tokens);
    mask = 3 << 29;
    result |= mask;
    mask = 1 << 21;
    result |= mask;
  }
  break;

  case 249944:
  { // eor
    result = handle_logic(tokens);
    mask = 2 << 29;
    result |= mask; // N is 0
  }
  break;

  case 250209:
  { // orr
    result = handle_logic(tokens);
    mask = 1 << 29;
    result |= mask; // N is 0
  }
  break;

  case 249940:
  { // eon
    result = handle_logic(tokens);
    mask = 2 << 29;
    result |= mask;
    mask = 1 << 21;
    result |= mask;
  }
  break;

  case 250205:
  { // orn
    result = handle_logic(tokens);
    mask = 1 << 29;
    result |= mask;
    mask = 1 << 21;
    result |= mask;
  }
  break;
    // wide move instructions
  case 1250847:
  { // movk
    result = handle_wide(tokens);
    mask = 3 << 29;
    result |= mask;
  }
  break;

  case 1250850:
  {                               // movn
    result = handle_wide(tokens); // opc for movn is 00
  }
  break;

  case 1250862:
  { // movz
    result = handle_wide(tokens);
    mask = 2 << 29;
    result |= mask;
  }
  break;

  case 1250400:
  {                              // madd
    result = handle_mul(tokens); // x is 0 as per the spec
  }
  break;

  case 1250933:
  { // msub
    result = handle_mul(tokens);
    mask = 1 << 15;
    result |= mask;
  }
  break;
    // below are the alias types
  case 249882:
  { // cmp
    result = handle_alias_cct("subs", tokens);
  }
  break;

  case 249880:
  { // cmn
    result = handle_alias_cct("adds", tokens);
  }
  break;

  case 250341:
  { // tst
    result = handle_alias_cct("ands", tokens);
  }
  break;

  case 250108:
  { // neg
    result = handle_alias_nm("sub", tokens);
  }
  break;

  case 1250655:
  { // negs
    result = handle_alias_nm("subs", tokens);
  }
  break;

  case 250175:
  { // mvn
    result = handle_alias_nm("orn", tokens);
  }
  break;

  case 250148:
  { // mov
    tokens[0] = "orr";
    tokens[3] = tokens[2]; // rm moved
    if (*tokens[1] == 'x')
    {                    // checks register width
      tokens[2] = "xzr"; // for 0 register
    }
    else
    {
      tokens[2] = "wzr";
    }
    result = assemble_dp(tokens);
  }
  break;

  case 250168:
  { // mul
    result = handle_alias_mults("madd", tokens);
  }
  break;

  case 1250733:
  { // mneg
    result = handle_alias_mults("msub", tokens);
  }
  break;

  default:
  {
    printf("invalid instruction\n");
    exit(EXIT_FAILURE);
  }
  break;
  }
  return result;
}