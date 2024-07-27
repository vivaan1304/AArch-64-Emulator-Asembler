#ifndef DEFS_H
#define DEFS_H

#define DP_INST ((char const *[]){ \
    "add", "adds", "sub", "subs",  \
    "cmp", "cmn",                  \
    "neg", "negs",                 \
    "and", "ands", "bic", "bics",  \
    "eor", "orr", "eon", "orn",    \
    "tst",                         \
    "movk", "movn", "movz",        \
    "mov",                         \
    "mvn",                         \
    "madd", "msub",                \
    "mul", "mneg", NULL})

#define branch_inst ((char const *[]){   \
    "b", "b.eq", "b.ne", "b.ge", "b.lt", \
    "b.gt", "b.le", "b.al", "br", NULL})
#define ls_inst ((char const *[]){ \
    "str", "ldr", NULL})

// SYMBOL TABLE:
struct Symbol
{
  char *label;
  int address;
  struct Symbol *next;
};
typedef struct Symbol *Symbol;

struct SymbolTable
{
  struct Symbol *head;
  int number_of_elements;
};
// pointer to the symbol table
typedef struct SymbolTable *SymbolTable;

#define MAX_ELEMENTS 1024

#define TABLE_SIZE 1 << 24;
#define NEW(t) malloc(sizeof(struct t))

#endif
