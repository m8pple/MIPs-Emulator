#include "mips.h"
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

class mips_cpu_impl
{
public: 
	FILE* logDst; 
	unsigned logLevel; 
	void set_pc(uint32_t val){
		pc = val; 
	}	
	void set_pcNext(uint32_t val){
		pcNext = val; 
	}
	uint32_t get_pc(){
		return pc; 
	}
	uint32_t get_pcNext(){
		return pcNext;
	}
	void set_memory(mips_mem_h *memory){
		mem = *memory;  
	}
	
	mips_mem_h get_memory(){
		return mem; 
	}
	
	mips_error get_register(unsigned index, uint32_t * value){
		*value =regs[index]; 
		return mips_Success; 
	}
	
	mips_error set_register(unsigned index, uint32_t value){
		regs[index] = value; 
		return mips_Success; 
	}
	void set_HI_register(uint32_t value){
		HI = value; 
	}
	
	void set_LO_register(uint32_t value){
		LO = value;
	}
	
	FILE* get_file_handler(){
		return logDst; 
	}

private: 
	uint32_t pc;
    uint32_t pcNext;
    uint32_t regs[32];
    mips_mem_h mem;
	uint32_t HI;
	uint32_t LO;
};
	
mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest)
{
    state->logLevel = level;
    state->logDst = dest;
    return mips_Success;
}

mips_cpu_h mips_cpu_create(mips_mem_h mem)
{
    mips_cpu_impl *cpu=new mips_cpu_impl;
    mips_error e = mips_cpu_reset(cpu);
    cpu -> set_memory(&mem);
    return cpu;
}

mips_error mips_cpu_reset(mips_cpu_h cpu){
	cpu->set_pc(0); 
	cpu->set_pcNext(4); 
    mips_error e;
	for(int i=0;i<32;i++){
        e = cpu->set_register(i, 0);
    }
	cpu -> set_HI_register(0); 
	cpu -> set_LO_register(0); 
	return mips_Success; 
}

mips_error mips_cpu_set_pc(
	mips_cpu_h state, 
	uint32_t pc
	)
	{
		if(pc %4 == 0 && state!=NULL){
			state->set_pc(pc);
			return mips_Success; 
		}
		else{
			return mips_ExceptionInvalidAddress;
			}
	}
	
	mips_error mips_cpu_get_pc(
		mips_cpu_h state, 
		uint32_t *pc)
		{
			if(state -> pc %4 == 0 && state != NULL){ 
				*pc = state -> get_pc(); 
				return mips_Success;
			}
			else{
				return mips_ExceptionInvalidAddress;
			}
		}
		
void mips_cpu_free(mips_cpu_h state){
	if(state != NULL)
		delete state; 
}

/*! \param pData Array of 4 bytes
    \retval The big-endian equivlent
*/
uint32_t to_big(const uint8_t *pData)
{
    return
        (((uint32_t)pData[0])<<24)
        |
        (((uint32_t)pData[1])<<16)
        |
        (((uint32_t)pData[2])<<8)
        |
        (((uint32_t)pData[3])<<0);
}

void trans_high_low(string operand, string address, uint32_t dst, mips_cpu_h state){
	if(state->logLevel >= 1)
		fprintf (state -> logDst, "%u, %u.\n", operand, address)
	switch(operand){
		case "mthi": 
			state->HI = state->regs[dst];
			break; 
		
		case "mtlo":
			state->LO = state->regs[dst];
			break; 
		
		case "mfhi": 
			state->regs[dst] = state->HI;
			break; 
		
		case "mflo": 
			state->regs[dst] = state->LO;
			break; 
		
		default:; 
	}
}
void pShift(string operand, mips_cpu_h state, string arg1, string arg2, string arg3){
	if (state -> logLevel >= 1)
		fprintf (state -> logDst, "%u %u, %u, %u.\n", operand, dst, src2, src1);
	uint32_t val_to_shift, val_shift_by; 
	if (state->get_register(src2, &val_to_shift) == mips_Success && state->get_register(src1, &val_shift_by)
		state -> regs[dst] = val_to_shift >> (val_shift_by & 31);
	else 
		return;
}

mips_error mips_cpu_step(
	mips_cpu_h state	//! Valid (non-empty) handle to a CPU
)
{
    uint8_t buffer[4];  
    mips_error err=
	mips_mem_read(
        state->mem,		//!< Handle to target memory
        state->pc,	//!< Byte address to start transaction at
        4,	//!< Number of bytes to transfer
        buffer	//!< Receives the target bytes
    );
    
    if(!err)
        return err;
    
	mips_error e = state->set_register(0, 0);
    uint32_t instruction = to_big(buffer);
    uint32_t opcode =  (instruction>>26) & 0x3F;
	uint8_t data_from_mem[4];
	string operand; 
	if(opcode == 0x02 || opcode == 0x03){
		uint32_t address = instruction & 0x3FFFFFF;
		if (state -> logLevel >= 2)
			fprintf (state -> logDst, "J - type: address = %u.\n  instr=%08x\n", address, instruction);
		
		switch(opcode){
			//This is J type
			case 0x02:
				operand = "j"
				break; 
		
			case 0x03: 
				operand = "jal"
				e = state->set_register(31, state->pcNext + 4);
				break; 
			default:; 
		}
			
		if(state->logLevel >= 1)
			fprintf (state -> logDst, "%u, %u.\n", operand, address)
		state->set_pc(state->get_pcNext);
		state->set_pcNext((address << 2) | (state->get_pcNext) & 0xF00000);
		return e; 
	}
	else{
		uint8_t addr_jump = 4; 
		uint32_t src1 = (instruction>> 21) & 0x1F;
		uint32_t src2 = (instruction>> 16) & 0x1F;   
		uint32_t dst =  (instruction>> 11 ) & 0x1F;
		uint32_t function = (instruction >> 0) & 0x3F;     
		uint32_t shift = (instruction>> 6 ) & 0x1F ;
		uint32_t va, vb;
		e = state->get_register(src1, &va);
		e = state->get_register(src2, &vb);
		if(opcode == 0x00){
        // This is R-type
			if(state->logLevel >= 2){
				fprintf(state->logDst, "R-Type : dst=%u, src1=%u, src2=%u, shift=%u, function=%u.\n  instr=%08x\n",
					dst, src1, src2, shift, function, instruction
				);
			}
		switch(function){
			
			case 0x00:
				operand = "sll"
				if (state -> logLevel >= 1){
					fprintf (state -> logDst, "sll %u, %u, %u.\n", dst, src2, shift);
				} 
				uint32_t shift_val; 
				e = state->get_register(src2, &shift_val); 
				if (e == mips_Success)
					e = state->set_register(dst, shift_val << shift); 
				break; 
			
			case 0x10: 
				if (src1 | src2 | shift == 0)
					trans_high_low("mfhi", dst, state); 
				break;
		
			case 0x11: 
				if(src2 | dst | shift == 0)
					trans_high_low("mthi", src1, state); 
				break; 
			
			case 0x12: 
				if(src1 | src2 | dst == 0)
					trans_high_low("mflo", dst, state);
				break; 
		
			case 0x13: 
				if(src2 | dst | shift == 0)
					trans_high_low("mtlo", src1, state);
				break; 
				
			case 0x09: 
				if(src2 | shift == 0){
					if(state->logLevel >= 1){
						if(dst == 31)
							fprintf(state->logDst, "jalr %u .\n", src1);
						else
							fprintf(state->logDst, "jalr %u, %u.\n", dst, src1);
					}
					if(src1 == dst){
						e = mips_ExceptionInvalidInstruction; 
					}
					else{
						e = state->set_register(dst, state->get_pcNext); 
						addr_jump = state -> regs[src1];
					}
				}
				break; 
			
		case 0x08: 
			if(src2 | dst | shift == 0){
				if (state -> logLevel >= 1)
					fprintf (state -> logDst, "jr %u.\n", src1);
				uint32_t reg; 
				e = mips_cpu_get_register(state, src1, &reg);
				if (reg % 4)
					e = mips_ExceptionInvalidAddress; 
			}
			break; 
		
		case 0x03: 
			if (state -> logLevel >= 1)
				fprintf (state -> logDst, "sra %u, %u, %u.\n", dst, src2, shift); 
			uint32_t shift_val, temp, sign;
			e = state -> get_register(src2, &shift_val);
			if(e == mips_Success){
				temp = shift_val >> shift; 
				sign = shift_val >> 31; 
				if (sign)
					temp = ((0xFFFFFFFF << (31 - shift)) | temp);
				e = state->set_register(dst, temp);
			}
			break;
		
		case 0x18: 
			if(shift | dst == 0){
				if(state->logLevel >= 1)
					fprintf(state->logDst, "mult %u, %u, %u.\n", dst, src1, src2);
				uint32_t arg, arg1;
				if (state -> get_register(src1, &arg1) == mips_Success && state -> get_register(src2, &arg2) == mips_Success){
					int64_t res = arg1*arg;
					state->set_HI(res >> 32);
					state->set_LO(res);
				}
			}
			break;
		
		case 0x07: 
			if (state -> logLevel >= 1)
				fprintf (state -> logDst, "srav %u, %u, %u.\n", dst, src2, src1);
			uint32_t shift_val, shift_by;
			if(state->get_register(src2, &shift_val) == mips_Success && state->get_register(src1, &shift_by) == mips_Success){
				uint32_t temp = shift_val >> (shift_by & 0x1F); 
				if (shift_val >> 31 == 1)
					temp = ((0xFFFFFFFF << (31 - shift_by)) | temp);
				e = state->set_register(dst, temp);
			}
			break; 
		
		case 0x04: 
			pShift("sllv", state, dst, src2, src1); 
			break; 
		
		case 0x06:
			pShift("srlv", state, dst, src2, src1); 
			break;
		
		case 0x02:
			if (state -> logLevel >= 1)
				fprintf (state -> logDst, "srl %u, %u, %u.\n", dst, src2, shift);
			uint32_t src_val; 
			e = state->get_register(state, src2, &src_val); 
			if (e == mips_Success)
				e = state->set_register(dst, src_val >> shift);
			break;
		
		case 0x20:
			if(shift == 0){
				if (state->logLevel >= 1)
					fprintf(state->logDst, "add %u, %u, %u.\n", dst, src1, src2);
				uint32_t result = va+vb;
				uint32_t resm = result >> 31; 
				uint32_t vam = va >> 31; 
				uint32_t vbm = vb >> 31;   
				if(vam == vbm && vam != resm)
					e = mips_ExceptionArithmeticOverflow;
				else
					e = state->set_register(dst, result)
				}
			break;
		
		case 0x19:
		case 0x18: 
			uint64_t result = va*vb; 
			state->HI  = result << 32; 
			state->LO = result; 
			break; 
			/*logging level to be added*/
		
		case 0x2A: 
			if (state -> logLevel >= 1)
				fprintf(state->logDst, "slt %u, %u, %u.\n", dst, src1, src2);
			if (va < vb)
				 e = state->set_register(dst, 1);
			else
				 e = state->set_register(dst, 0);
			break;
		
		case 0x2B: 
			if (state -> logLevel >= 1)
				fprintf(state->logDst, "slt %u, %u, %u.\n", dst, src1, src2);
			if (va < vb)
				e = state->set_register(dst, 1);
			else
				e = state->set_register(dst, 0);
			break; 
		
		case 0x16:
			if (state->logLevel >= 1)
				fprintf(state->logDst, "sub %u, %u, %u.\n", dst, src1, src2);
			uint32_t result = va - vb; 
			uint32_t vam = va >> 31; 
			uint32_t vbm = va >> 31; 
			if((vam != vbm) && (vbm == (result >> 31)))
				e = mips_ExceptionArithmeticOverflow;
			else
				e = state->set_register(dst, va - vb);
			break;
			
		case 0x17:
			if (state->logLevel >= 1)
				fprintf(state->logDst, "subu %u, %u, %u.\n", dst, src1, src2);
			e  = state->set_register(dst, va - vb);
			break; 
		
		case 0x1B: 
			if (state->logLevel >= 1)
				fprintf(state->logDst, "xor %u, %u, %u.\n", dst, src1, src2);
            e = state->set_register(dst, va^vb);
			break;
		
		case 0x18:
			if(shift == 0){
				if (state->logLevel >= 1)
					fprintf(state->logDst, "and %u, %u, %u.\n", dst, src1, src2);
				e = state -> set_register(dst, va & vb);
			}
			break;
		
		case 0x19:
			if (state->logLevel >= 1)
				fprintf(state->logDst, "or %u, %u, %u.\n", dst, src1, src2);
            e = state->set_register(dst, va | vb);
			break;
		
		case 0x2B:
			if (state->logLevel >= 1)
                fprintf(state->logDst, "stlu %u, %u, %u.\n", dst, src1, src2);
            if (va < vb)
				e = state->set_register(dst, 1);
			else
				e = state->set_register(dst, 0);
			break; 

        case 0x21: 
			if(shift == 0){ 
				if(state->logLevel >= 1)
					fprintf(state->logDst, "addu %u, %u, %u.\n", dst, src1, src2);
				e = state->set_register(dst, va + vb);
			}
		break; 
		default:; 
		}
		return e; 
	 }
	else{
		uint32_t src1_i = (instruction >> 21) & 0x1F;
		uint32_t dest_i = (instruction >> 16) & 0x1F; 
		uint16_t data_i = instruction & 0xFFFF;
		if(state->logLevel >= 2){
            fprintf(state->logDst, "I-Type : dst=%u, src1=%u, src2=%u, shift=%u,  instr=%08x\n",
                dest_i, src1_i, data_i, instruction
            );
        }
		
		switch(opcode){
			case 0x06: 
			case 0x07: 
			case 0x08: 
				uint32_t get_val, res; 
				res = data_i << 2;
				if ((res >> 31) == 1)
					res |= 0xFFFF0000;  
				mips_error e = state->get_register(state, src1_i, &get_val);
				break; 
			default:; 
		}
		
		switch(opcode){
			uint32_t data, res, x,get_val1, get_val2; 
			case 0x06: 
				if (state -> logLevel >= 1)
					fprintf(state->logDst, "BLEZ %u, %u.\n", src1_i, data_i);
				if((get_val >> 31 == 1)|(get_val == 0)){
					addr_jump = res; 
					if((state -> pcNext >> 31) == 1 | (res % 4) != 0 )
						return mips_ExceptionInvalidAddress;
				}	
				break; 
			
			case 0x07: 
				if(dest_i == 0){
					if(state->logLevel >= 1)
						fprintf(state->logDst, "BGTZ %u, %u.\n", src1_i, data_i);
					if(get_val >> 31 == 0 && !get_val)
						addr_jump = res;   
					if(state -> pcNext >> 31 == 1 | res % 4 != 0)
						return mips_ExceptionInvalidAddress;
					}
				break;
					
			case 0x0A: 
			case 0x0B:
				if(va < data_i)
					mips_error e = state->set_register(dest_i, 1); 
				else 
					mips_error e = state->set_register(dest_i, 0);
				if(state -> logLevel >= 1){
					if (opcode == 0x0A)
							fprintf(state->logDst, "SLTI &u, %u.\n", dest_i, src1_i, data_i);
					else
							fprintf(state->logDst, "SLTIU &u, %u.\n", dest_i, src1_i, data_i);
				}
				break;
		
			case 0x09:
				if(state->logLevel >= 1)
					fprintf(state->logDst, "addiu %u, %u, %u.\n", dest_i, src1_i, data_i);
				uint32_t va = state -> regs[src1_i];
				uint32_t data = data_i; 
				if (data_i >> 15 == 1) 
					data |= 0xFFFF0000;
				e = state->set_register(dest_i, va + data);   
				break;
		
			case 0x08:
                if(state->logLevel >= 1)
					fprintf(state->logDst, "addi %u, %u, %u.\n", dest_i, src1_i, data_i);
                uint32_t data_m= data_i >> 15;
                data = data_i;  
				if (data_m == 1)data |= 0xFFFF0000;
                res = va + data;
                uint32_t resm = res >> 31;
                if ( ((va >> 31) == data_m) && ((va >> 31) != resm)) return mips_ExceptionArithmeticOverflow;
                else state->regs[dest_i] = res; 
				break;
		
			case 0x0C:
				if(state->logLevel >= 1)
					fprintf(state->logDst, "andi %u, %u, %u.\n", dest_i, src1_i, data_i);
				mips_error e = state->set_register(dest_i, va & data_i);
				break;

			case 0x0D:
				if(state->logLevel >= 1)
					fprintf(state->logDst, "ori %u, %u, %u.\n", dest_i, src1_i, data_i);
				uint32_t args1; 
				mips_error e1 = state->get_register(src1_i, &args1);
				mips_error e = state->set_register(dest_i, args1 | data_i);  
				break;

			case 0x0E:
				if(state->logLevel >= 1)
					fprintf(state->logDst, "xori %u, %u, %u.\n", dest_i, src1_i, data_i);
				state -> regs[dest_i] = va ^ data_i; 
				break;
				
			case 0x2B:
				if(state -> logLevel >= 1)
					fprintf(state -> logDst, "sw %u, offset(%u)", dest_i, src1_i);
				uint32_t result, value;
				mips_error e = mips_cpu_get_register(
				state,	//!< Valid (non-empty) handle to a CPU
				dest_i,		//!< Index from 0 to 31
				&result		//!< Where to write the value to
				);
            
				e = mips_cpu_get_register(
				state,	//!< Valid (non-empty) handle to a CPU
				src1_i,		//!< Index from 0 to 31
				&value		//!< Where to write the value to
				);
				data_from_mem[0] = result >> 24;
				data_from_mem[1] = result >> 16;
				data_from_mem[2] = result >> 8;
				data_from_mem[3] = result;
				uint32_t address = value + data_I;
                if (address % 4 != 0)
                    return mips_ExceptionInvalidAlignment;
				e = mips_mem_write(
				state -> mem,
				address,
				4,
				buffer
				);	
				break;
			
			case 0x0F:
				if(src1_i == 0){
					if(state->logLevel >= 1)
						fprintf(state->logDst, "lui %u, %u, %u.\n", dest_i, data_i);
					mips_error e = mips_cpu_set_register(state, dest_i, data_i << 16);
				}
				break; 
				
			
			case 0x21:
				if(state->logLevel >= 1)
					fprintf(state->logDst, "lh %u, %u, %u.\n", dest_i, src1_i, data_i);
				uint32_t data = data_i; 
				uint16_t res;  
				uint32_t val = state->regs[src1_i]; 
				uint32_t address = val + data; 
				uint32_t z = address - (address % 4);
				if (data_i >> 15)
					data = 0xFFFF0000 | uint32_t(data_i);
				if (address & 0x00000001 == 1){
					return mips_ExceptionInvalidAddress; 
				}
				else{
					mips_error e = mips_mem_read
					(
					state -> mem,
					z,
					4,
					data_from_mem
					);
					if (!data_from_mem){
						res = ((((uint16_t)x[0]) << 8)|
							(((uint16_t)x[1])<<0));
					}	
					else{
						res = ((((uint16_t)x[2]) << 8)|
							(((uint16_t)x[3])<<0));
					} 
					data = res;
					if (res >> 1 == 1) data = (0xFFFF0000 | uint32_t(res)); 
					e = mips_cpu_set_register(state, dest_i, data);
				}
				break;
		
			case 0x28:
				if(state->logLevel >= 1)
					fprintf(state->logDst, "sb %u, %u(%u).\n", dest_i, data_i, src1_i);
				uint32_t var, var1;
				mips_error e = state->get_register(src1_i, &var);
				mips_error e1 = state->get_register(dest_i, & var1);
				uint32_t total = data_i + var; 
				uint32_t byte = total % 4;  
				uint8_t val[4];
				val[byte] = var1;
				mips_error e = mips_mem_write(
				state->get_memory(),
				total - byte, 
				4, 
				val
				);
				break;
		
			case 0x25:
				if(state->logLevel >= 1)
					fprintf(state->logDst, "lhu %u, %u, %u.\n", dest_i, src1_i, data_i);
				 data = data_i;  
				if (data_i >> 15)
					data = 0xFFFF0000 | data_i;
				if ((src1_i + data) & 0x00000001 == 1){
					return mips_ExceptionInvalidAddress; 
				}
				else{
					mips_error e = mips_mem_read(
					state -> get_memory(),
					src1_i + data,
					4,
					data_from_mem
					);
					data =  (((uint32_t)data_from_mem[0])<<24)
					|
					(((uint32_t)data_from_mem[1])<<16)
					|
					(((uint32_t)data_from_mem[2])<<8)
					|
					(((uint32_t)data_from_mem[3])<<0);
					e = mips_cpu_set_register(state, dest_i, data);
				}
				break;
		
			case 0x23:
				if(state->logLevel >= 1)
					fprintf(state->logDst, "lw %u, %u, %u.\n", dest_i, src1_i, data_i);      
				e =  mips_cpu_get_register(
				state,	//!< Valid (non-empty) handle to a CPU
				src1,		//!< Index from 0 to 31
				&value		//!< Where to write the value to
				);
				e = mips_mem_read(
				state -> get_memory(),		//!< Handle to target memory
				value + data_i,	//!< Byte address to start transaction at
				4,	//!< Number of bytes to transfer
				data_from_mem//!< Receives the target bytes
				);
				e = state-> set_register(dest_i, ((((uint16_t)data_from_mem[0])<<8)
				|
				(((uint16_t)data_from_mem[1]))));
				break;
		
		case 0x29:
			if(state -> logLevel >= 1)
				fprintf(state->logDst, "sh %u, %u(%u).\n", dest_i, data_i, src1_i); 
			
			uint32_t vAddr; 
			e = state->get_register(src1_i, &vAddr);
			vAddr += data_i;  
			
			e = state->get_register(dest_i, &data);
			uint16_t half_word = 0x0000FFFF & data; 
			uint8_t a[2]; 
			a[0] = (half_word >>8) & 0xFF;
			a[1] = (half_word >> 8) & 0xFF;
			if((vAddr % 2) != 0){
				return mips_ExceptionInvalidAlignment;
			}
			else{
				mips_error e = mips_mem_write(
				state->get_memory(),
				vAddr,
				2,
				a
				); 
			}
			break; 
				
		case 0x04:
			if(state->logLevel >= 1)
                fprintf(state->logDst, "beq %u, %u, %u.\n", dest_i, src1_i, data_i);
			e = mips_cpu_get_register(state, dest_i, &get_val1);
			e = mips_cpu_get_register(state, src1_i,&get_val2);  
			if(get_val1 == get_val2){
				res = data_i << 2;
				if (res >> 31)
					res = 0xFFFF0000 | uint32_t(res);
				addr_jump =   res; 
				if(state -> get_pcNext() >> 31 | res % 4 )
					return mips_ExceptionInvalidAddress;
			}
			break;
			
		case 0x01:
			uint32_t getval; 
			e = mips_cpu_get_register(state, src1_i, &getval);
			uint16_t shift_left = data_i << 2; 
			res = shift_left; 
			if(shift_left >> 15)
				res |= 0xFFFF0000;
			if (dest_i == 1){
				if(state -> logLevel >= 1)
					fprintf (state -> logDst, "BGEZ %u, %u.\n", src1_i, data_i);
				if(shift_left >> 15 == 1 && getval >> 31 == 0)
					addr_jump = 0xFFFF0000| shift_left;
				else if((shift_left >> 15) & (getval >> 31) == 0)
					addr_jump = shift_left;
			}
			
			else if(dest_i == 17){
				if(state -> logLevel >= 1)
					fprintf (state -> logDst, "BGEZAL %u, %u.\n", src1_i, data_i);
				e = state->set_register(31, state->get_pcNext() + 4); 
				if (!getval >> 31)
					addr_jump = res; 
			}
			else if (dest_i == 0){
				if(state -> logLevel >= 1)
					fprintf (state -> logDst, "BLTZ %u, %u.\n", src1_i, data_i);
				if (getval >> 31)
					addr_jump = res; 
			}
			
			else if(dest_i == 16){
				if(state -> logLevel >= 1)
					fprintf (state -> logDst, "BLTZAL %u, %u.\n", src1_i, data_i);
				e = state->set_register(31, state->get_pcNext() + 4);
				if (getval >> 31)
					addr_jump = res; 
				}
			break;
		
		case 0x05:
			if(state->logLevel >= 1)
                fprintf(state->logDst, "bne %u, %u, %u.\n", dest_i, src1_i, data_i);
			e = mips_cpu_get_register(state, dest_i, &get_val1);
			e = mips_cpu_get_register(state, src1_i,&get_val2);
			if(get_val1 != get_val2){
				uint16_t shift_left = data_i << 2;
				if (shift_left >> 15 == 1)
					res = 0xFFFF0000 | uint32_t(shift_left);
				else
					res = uint32_t(shift_left);
				addr_jump =  res; 
			}
			break;
		
			case 0x20: 
				if(state->logLevel >= 1)
					fprintf(state->logDst, "lb %u, %u, %u.\n", dest_i, src1_i, data_i);
				e = mips_cpu_get_register(state, src1_i, &x ); 
				res = (x + data_i) % 4;
				data = data_i; 
				if (data_i >> 15)
					data |= 0xFFFF0000;
			
				e = mips_mem_read(
				state-> get_memory(), 
				x + data - res, 
				4, 
				data_from_mem);
				uint32_t y_final = uint32_t(data_from_mem[res]); 
				if (data_from_mem[res] >> 7)
					y_final |= 0xFFFFFF00;
				e = state->set_register(dest_i, y_final);
				break;
			
			case 0x24:
				if(state->logLevel >= 1)
					fprintf(state->get_file_handler(), "lbu %u, %u, %u.\n", dest_i, src1_i, data_i);
				e = mips_cpu_get_register(state, src1_i, &x );  
				res = (x + data_i) % 4;
				data = data_i; 
				if (data_i >> 15)
					data |= 0xFFFF0000;
				e = mips_mem_read(
				state-> get_memory(), 
				x + data - res, 
				4, 
				data_from_mem);
				e = state->set_register(dest_i, data_from_mem[res]);
				break;
			default:;
		}
		state->set_pc(state->get_pcNext());
		state->set_pcNext((state->get_pcNext()) + addr_jump); 
		return mips_Success; 
	}
		return mips_Success; 
	}
	}