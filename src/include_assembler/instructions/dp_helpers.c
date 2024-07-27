#include "../instructions.h"
#include <stdint.h>
#include <string.h>

uint32_t handle_arithm(char **tokens)
{
  uint32_t mask = 0;
  char *ptr = tokens[1];
  uint32_t result = set_width(0, tokens); // bit 31
  result = set_regs(result, tokens);      // bits 0 - 9
  ptr = tokens[3];
  if (*ptr == '#')
  {                       // immediate
    result |= 0x22 << 23; // 0x22 is 100010 in binary bits 23 -28
    // this sets bits 23 -28 to indicate an arithmetic immidiate
    mask = return_imm(ptr) << 10; // imm12 value
    result |= mask;               // bits 10 -21
    if (tokens[4] != NULL && strcmp(tokens[5], "#12") == 0)
    {
      result |= 1 << 22; // case for shifting // bit 22
    }
  }
  else
  {                               // register
    mask = check_zero(ptr) << 16; // rm register
    result |= mask;               // bits 16 - 20
    mask = 5 << 25;               // 00101 in binary
    result |= mask;               // for bits 25 - 30
    result = set_shift(result, tokens);
    result |= 1 << 24; // to indicate arithmetic bit 24
  }
  return result;
}

uint32_t handle_logic(char **tokens)
{
  uint32_t result = set_width(0, tokens); // bit 31
  result = set_regs(result, tokens);      // bits 0-9
  result = set_shift(result, tokens);     // bits 22-24 and 10-15
  char *ptr = tokens[3];
  uint32_t mask = check_zero(ptr) << 16; // rm register bits 16-20
  result |= mask;
  mask = 5 << 25; // bits 25 - 28
  result |= mask;
  return result;
}

uint32_t handle_wide(char **tokens)
{
  char *ptr = tokens[1];
  uint32_t result = set_width(0, tokens); // bit 31
  uint32_t mask = check_zero(ptr);
  result |= mask; // bits 0-4
  ptr = tokens[2];
  mask = return_imm(ptr) << 5; // imm16 value
  result |= mask;              // bits 5 - 20
  if (tokens[3] != NULL)
  {
    ptr = tokens[4];
    mask = return_imm(ptr) / 16;
    result |= (mask << 21); // bits 21-22
  }
  mask = 0x25 << 23; // 010101 in binary to indicate wide move
  result |= mask;    // bits 23 -28
  return result;
}

uint32_t handle_mul(char **tokens)
{
  char *ptr = tokens[3];
  uint32_t result = set_regs(0, tokens); // bits 0-9
  uint32_t mask = check_zero(ptr);
  result |= mask << 16; // bit 16-20 rm
  ptr = tokens[4];
  mask = check_zero(ptr);
  result |= mask << 10; // bits 10-14 ra
  mask = 0xD8 << 21;    // 00 1101 1000 in binary bits 21 - 30
  result |= mask;
  result = set_width(result, tokens); // bit 31
  return result;
}

uint32_t handle_alias_cct(char *alias, char **tokens)
{
  // helper to handle cmp, cmn & tst instructions
  tokens[0] = alias; // Set the alias directly
  if (tokens[3] != NULL)
  { // Check for optional instruction
    tokens[5] = tokens[4];
    tokens[4] = tokens[3];
    tokens[6] = NULL;
  }
  else
  {
    tokens[4] = NULL;
  }
  tokens[3] = tokens[2]; // Move rm
  tokens[2] = tokens[1]; // Move rn
  if (*tokens[1] == 'x')
  {                    // Check register width
    tokens[1] = "xzr"; // For 0 register
  }
  else
  {
    tokens[1] = "wzr";
  }
  return assemble_dp(tokens);
}

uint32_t handle_alias_nm(char *alias, char **tokens)
{
  // helper for neg(s) & mvn
  tokens[0] = alias; // Set the alias directly

  if (tokens[2] != NULL)
  { // Check for optional instruction
    tokens[4] = tokens[3];
    tokens[3] = tokens[2];
    tokens[5] = NULL;
  }
  else
  {
    tokens[3] = NULL;
  }

  if (*tokens[1] == 'x')
  { // Check register width
    tokens[2] = "xzr";
  }
  else
  {
    tokens[2] = "wzr";
  }

  return assemble_dp(tokens);
}

uint32_t handle_alias_mults(char *alias, char **tokens)
{
  // helper for mul and mneg
  tokens[0] = alias; // Set the alias directly

  if (*tokens[1] == 'x')
  { // Check register width
    tokens[4] = "xzr";
  }
  else
  {
    tokens[4] = "wzr";
  }
  tokens[5] = NULL;

  return assemble_dp(tokens);
}