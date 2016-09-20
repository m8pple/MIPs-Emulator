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
    
void slt(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void addi(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void beq(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void lw(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void addiu(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void addu(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void AND(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void OR(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void sllv(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void subu(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void sw(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void XOR(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void add(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void XORI(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void LB(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void SB(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void sub(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void bne(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void lh(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void LBU(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void SLTU(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void SRA(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void SRAV(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void SRL(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void J(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void bgtz(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void bltz(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
void lui(mips_mem_h mem, mips_cpu_h cpu, uint32_t instr, uint32_t val, FILE*fp);
#ifdef __cplusplus
};
#endif

#endif
