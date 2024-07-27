#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "defs.h"

// assembler helpers
unsigned long hash_string(const char *str);
int in_array(char *target, char const *array[]);
uint32_t return_imm(const char *immediate);

uint32_t check_zero(char *reg);
uint32_t set_width(uint32_t result, char **tokens);
uint32_t set_regs(uint32_t result, char **tokens);

uint32_t set_shift(uint32_t result, char **tokens);

char get_last_char(const char *str);
uint32_t signed_nbit(int value, int n);
// data processing helpers

uint32_t handle_arithm(char **tokens);
uint32_t handle_logic(char **tokens);

uint32_t handle_wide(char **tokens);
uint32_t handle_mul(char **tokens);

uint32_t handle_alias_cct(char *alias, char **tokens);

uint32_t handle_alias_nm(char *alias, char **tokens);
uint32_t handle_alias_mults(char *alias, char **tokens);

// stdi helpers

uint32_t handle_sdt(char **tokens);

uint32_t handle_litLoad(char **tokens, SymbolTable symtab, uint64_t PC);
// assemblers

uint32_t assemble_dp(char **tokens);

uint32_t assemble_stdi(char **tokens, SymbolTable symtab, uint64_t PC);

uint32_t assemble_branch(char **tokens, SymbolTable symtab, uint64_t PC);

uint32_t assemble_s(char **tokens);

uint32_t assemble_line(char **tokens, SymbolTable symtab, uint64_t PC);

#endif