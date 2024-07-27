#include "../defs.h"
#include "../utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

SymbolTable create_symbol_table()
{
  SymbolTable symbolTable = NEW(SymbolTable);
  symbolTable->head = NULL;
  symbolTable->number_of_elements = 0;
  return symbolTable;
}

void insert_symbol(SymbolTable symbolTable, const char *labelToInsert, int val)
{
  Symbol newSymbol = NEW(Symbol);
  newSymbol->label = strdup(labelToInsert);
  newSymbol->address = val * 4;
  symbolTable->number_of_elements++;
  // inserting the new symbol to the head of the list
  // so, head is the latest element
  newSymbol->next = symbolTable->head;
  symbolTable->head = newSymbol;
}

int lookup_symbol(SymbolTable symbolTable, const char *label)
{
  // starts searching from the latest element
  Symbol symbol = symbolTable->head;
  while (symbol)
  { // not null
    if (strcmp(symbol->label, label) == 0)
    {

      return symbol->address;
    }
    symbol = symbol->next;
  }
  return -1; // no symbol found
}

void free_table(SymbolTable symbolTable)
{
  Symbol symbol = symbolTable->head;
  while (symbol)
  {
    Symbol temp = symbol;
    symbol = symbol->next;
    free(temp->label);
    free(temp);
  }
  free(symbolTable);
}

void print_symbol(SymbolTable st)
{
  Symbol ptr = st->head;
  while (ptr != NULL)
  {
    printf("Symbol: %s\n", ptr->label);
    printf("Address: %d\n", ptr->address);
    ptr = ptr->next;
  }
}