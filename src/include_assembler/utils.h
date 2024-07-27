#ifndef UTILS_H
#define UTILS_H

#include "defs.h"
#include <stdint.h>
// symbol table functions
SymbolTable create_symbol_table();

void insert_symbol(SymbolTable symbolTable, const char *labelToInsert, int val);

int lookup_symbol(SymbolTable symbolTable, const char *label);

void free_table(SymbolTable symbolTable);
void print_symbol(SymbolTable st);

// tokenization function
char **tokenize(char *line, SymbolTable st, int line_num);

// writing to file

void write_bin_file(const char *fileName, uint32_t *instructions);

#endif
