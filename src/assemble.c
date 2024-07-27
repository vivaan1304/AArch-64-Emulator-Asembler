#include "include_assembler/defs.h"
#include "include_assembler/utils.h"
#include "include_assembler/instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <assert.h>

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Usage: %s <assembly_file.s>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[1], "r");
  if (!file)
  {
    perror("Error opening file");
    return EXIT_FAILURE;
  }
  SymbolTable st = create_symbol_table();
  int line_num = 0;
  char line[256];

  while (fgets(line, sizeof(line), file))
  {
    if (strlen(line) == 1)
      continue;
    char **tokens = tokenize(line, st, line_num);
    if (strcmp(tokens[0], "label") != 0)
    {
      line_num++;
    }
  }
  // completed first pass
  rewind(file);
  // second
  uint64_t PC = 0;
  uint32_t instructions[MAX_ELEMENTS];
  int i = 0;
  while (fgets(line, sizeof(line), file))
  {
    if (strlen(line) == 1)
      continue;
    char **tokens = tokenize(line, st, -1);
    if (strcmp(tokens[0], "label") != 0)
    {
      uint32_t result = assemble_line(tokens, st, PC);
      PC += 4;
      instructions[i] = result;
      i++;
    }
    instructions[i] = -1; // termination
  }
  write_bin_file(argv[2], instructions);
  fclose(file);
  free(st);
  return EXIT_SUCCESS;
}
