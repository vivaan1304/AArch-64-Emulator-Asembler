#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "../instructions.h"
#include "../utils.h"
#include "../defs.h"

/*
 * Loads or stores data from/to memory.
 */

void loadStore(long long addr, int L, int sf, int64_t *rt, machine *reg)
{
    int size = sf ? 8 : 4;
    if (L)
    {
        *rt = 0;
        for (int i = 0; i < size; i++)
        {
            *rt |= ((long long)reg->memory[addr + i]) << (i * 8);
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            reg->memory[addr + i] = ((uint64_t)*rt >> (i * 8)) & 0xFF;
        }
    }
}

/*
 * This function performs the necessary operations to execute a Single Data Transfer instruction
 * based on the provided instruction and register values.
 */

void handle_sdti(machine *reg, uint32_t inst)
{
    int op0 = getOp0(inst);
    assert(op0 == 12);

    int simm19 = getBits(5, 23, inst);
    if (simm19 & (1 << 18))
        simm19 |= ~((1 << 19) - 1); // sign extension

    int imm12 = getBits(10, 21, inst);
    int simm9 = getBits(12, 20, inst);
    if (simm9 & (1 << 8))
        simm9 |= ~((1 << 9) - 1); // sign extension

    int I = getBit(11, inst);  // 1 = Pre-indexed, 0 = Post-indexed
    int R = getBit(21, inst);  // 1 = register,    0 = signed immediate
    int L = getBit(22, inst);  // 1 = load,        0 = store
    int U = getBit(24, inst);  // 1 = unsigned,    0 = signed
    int sf = getBit(30, inst); // 1 = 64,          0 = 32
    int ll = getBit(31, inst); // 1 = SDT,         0 = loadliteral
    int rtIndex = getBits(0, 4, inst);
    int xnIndex = getBits(5, 9, inst);
    int xmIndex = getBits(16, 20, inst);

    long long uoffset, addr;
    long long tAddr = (long long)(simm19 * 4);

    int64_t *rt = getRegister(rtIndex, reg, sf);

    int64_t *xn = NULL;
    if (xnIndex != NUMBER_OF_GPR)
        xn = getRegister(xnIndex, reg, 1);

    int64_t *xm = NULL;
    if (xmIndex != NUMBER_OF_GPR)
        xm = getRegister(xmIndex, reg, 1);

    if (ll)
    {
        if (U)
        {
            // Unsigned offset
            uoffset = sf ? imm12 * 8 : imm12 * 4;
            addr = get_reg_value(xn) + uoffset;
            loadStore(addr, L, sf, rt, reg);
        }
        else
        {
            if (R)
            {
                // Register offset
                addr = get_reg_value(xn) + get_reg_value(xm);
                loadStore(addr, L, sf, rt, reg);
            }
            else
            {
                if (I)
                {
                    // Pre-indexed
                    addr = get_reg_value(xn) + simm9;
                    *xn = addr;
                    loadStore(addr, L, sf, rt, reg);
                }
                else
                {
                    // Post-indexed
                    addr = get_reg_value(xn);
                    loadStore(addr, L, sf, rt, reg);
                    *xn += simm9;
                }
            }
        }
    }
    else
    {
        // Load literal
        addr = reg->PC + tAddr;
        loadStore(addr, 1, sf, rt, reg);
    }
}