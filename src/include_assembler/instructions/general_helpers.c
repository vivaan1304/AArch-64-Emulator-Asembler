#include "../defs.h"
#include "../utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

unsigned long hash_string(const char *str)
{
  unsigned long hash = 1974;
  int c;

  while ((c = *str++))
  {
    hash = ((hash << 2) + hash) + c;
  }

  return hash;
}
// Define a helper to check if a string is in an array of strings
int in_array(char *target, char const *array[])
{
  int found = 0;
  for (int i = 0; array[i] != NULL; i++)
  {
    if (strcmp(target, array[i]) == 0)
    {
      found = 1;
      break;
    }
  }
  return found;
}

uint32_t return_imm(const char *immediate)
{
  char *endptr;
  if (immediate == NULL)
    return 0;
  if (strncmp(immediate, "#0x", 3) == 0)
  {
    // Case for hexadecimal value
    return (uint32_t)strtoul(immediate + 3, &endptr, 16);
  }
  else if (strncmp(immediate, "0x", 2) == 0)
  { // for int directive
    return (uint32_t)strtoul(immediate + 2, &endptr, 16);
  }
  else if (strncmp(immediate, "#", 1) == 0)
  {
    // Case for decimal value
    return (uint32_t)strtoul(immediate + 1, &endptr, 10);
  }
  else
  {
    // for int directive
    return (uint32_t)strtoul(immediate, &endptr, 10);
  }
}

uint32_t check_zero(char *reg)
{ // checks for the encoding of the 0 reg
  if (strcmp("zr", reg + 1) == 0)
  {
    return 31;
  }
  return atoi(reg + 1);
}

uint32_t set_width(uint32_t result, char **tokens)
{
  char *ptr = tokens[1];
  uint32_t mask = 1 << 31;
  result &= ~mask; // clear bit 31
  if (*ptr == 'x')
  {
    // case for 64 bit registers sf-bit
    result |= mask;
  }
  return result;
}
uint32_t set_regs(uint32_t result, char **tokens)
{
  char *ptr = tokens[1];
  uint32_t mask = check_zero(ptr); // first register rd
  result |= mask;
  ptr = tokens[2];
  mask = check_zero(ptr) << 5; // second register rn
  result |= mask;
  return result;
}

uint32_t set_shift(uint32_t result, char **tokens)
{
  uint32_t mask = 0;
  if (tokens[4] != NULL)
  { // in case of shift
    if (strcmp("lsl", tokens[4]) == 0)
    {
      mask = 0 << 22; // 00 in binary
    }
    else if (strcmp("lsr", tokens[4]) == 0)
    {
      mask = 1 << 22; // 01 in binary
    }
    else if (strcmp("asr", tokens[4]) == 0)
    {
      mask = 2 << 22; // 10 in binary
    }
    else if (strcmp("ror", tokens[4]) == 0)
    {
      mask = 3 << 22; // 11 in binary
    }
    result |= mask; // bits 22 -23
    char *ptr = tokens[5];
    mask = return_imm(ptr) << 10; // value of shift bits 10-15
    result |= mask;
  }
  return result;
}

char get_last_char(const char *str)
{
  // Check if the string is NULL or empty
  if (str == NULL || *str == '\0')
  {
    return '\0'; // Return null character if the string is NULL or empty
  }

  size_t len = strlen(str); // Get the length of the string

  return str[len - 1]; // Return the last character
}
uint32_t signed_nbit(int value, int n)
{
  // Check if n is within valid range (1 to 32)
  if (n < 1 || n > 32)
  {
    return 0;
  }

  // Mask to isolate n bits
  uint32_t mask = (1 << n) - 1;

  // Sign extend the input value and mask to n bits
  uint32_t result;

  if (value < 0)
  {
    // Negative value case: perform sign extension and mask to n bits
    result = ((uint32_t)value & mask) | (~mask & ((1 << n) - 1));
  }
  else
  {
    // Positive value case: mask to n bits
    result = (uint32_t)value & mask;
  }

  return result;
}