#include "../defs.h"
#include "../utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

char **tokenize(char *line, SymbolTable st, int line_num)
{
  char **tokens = malloc(MAX_ELEMENTS * sizeof(char *));
  if (!tokens)
  {
    perror("Failed to allocate memory for tokens");
    exit(EXIT_FAILURE);
  }
  line[strcspn(line, "\n")] = 0;
  int i = 0;
  char *rest = line;
  char *token;

  // Tokenize label (if exists)
  token = strtok_r(rest, " ", &rest);
  if (token && token[strlen(token) - 1] == ':')
  {
    token[strlen(token) - 1] = '\0';
    if (line_num > -1)
    {
      insert_symbol(st, token, line_num);
    }
    tokens[0] = "label";
    return tokens;
  }

  // Opcode
  if (token)
  {
    tokens[i++] = token;
  }

  // Operands
  while ((token = strtok_r(NULL, ", ", &rest)))
  {
    tokens[i++] = token;
  }

  tokens[i] = NULL; // Null-terminate the list of tokens
  // print tokens
  for (int j = 0; tokens[j] != NULL; j++)
  {
    // printf("%s ", tokens[j]);
  }
  // printf("\n");
  return tokens;
}

void write_bin_file(const char *fileName, uint32_t *instructions)
{
  FILE *file = fopen(fileName, "wb"); // Open the file in binary write mode
  if (file == NULL)
  {
    perror("Error opening file");
    return;
  }
  int i = 0;
  while (instructions[i] != -1)
  {
    size_t result = fwrite(&instructions[i], sizeof(uint32_t), 1, file); // Write the 32-bit integer to the file
    if (result != 1)
    {
      perror("Error writing to file");
    }
    i++;
  }
  fclose(file); // Close the file
}