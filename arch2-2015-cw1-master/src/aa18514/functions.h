/*! \file mips_test.h
    Defines the functions used to test a processor.
*/
#ifndef functions_header
#define functions_header
#include "mips.h"


/* This allows the header to be used from both C and C++, so
programs can be written in either (or both) languages. */
#ifdef __cplusplus
extern "C"{
#endif
    
void slt(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void addi(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void beq(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void lw(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void addiu(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void addu(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void AND(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void OR(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void sllv(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void subu(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void sw(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void XOR(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void add(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void XORI(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void LB(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void SB(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void sub(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void bne(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void lh(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void LBU(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void SLTU(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void SRA(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void SRAV(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void SRL(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void J(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void bgtz(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void bltz(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
void lui(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val);
#ifdef __cplusplus
};
#endif

#endif
