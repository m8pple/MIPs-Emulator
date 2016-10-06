	#include "mips.h"
	#include <iostream>
	#include <stdio.h>
	#include <string>
	using namespace std;

	class mips_cpu_impl
	{
	public: 
		template <typename T> 
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
		
		uint32_t get_LO_register(){
			return LO;
		}

		uint32_t get_HI_register(){
			return HI;
		}
		FILE* get_file_handler(){
			return logDst; 
		}

		bool logging(unsigned logLevel){
			if(logLevel > 0)
				return true; 
			else
				return false; 
		}
		unsigned get_logLevel(){
			return logLevel;
		}
		void set_logLevel(unsigned level){
			logLevel = level;
		}
		void set_file_handler(FILE* dest){
			logDst = dest;
		}
	private: 
		unsigned logLevel;
		FILE* logDst; 
		uint32_t pc;
	    uint32_t pcNext;
	    uint32_t regs[32];
	    mips_mem_h mem;
		uint32_t HI;
		uint32_t LO;
	};
		
	mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest)
	{
	    state -> set_logLevel(level);
	    state -> set_file_handler(dest);
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
				if(state->get_pc() % 4 == 0 && state != NULL){ 
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

	void load_byte(mips_cpu_h state, string operand, uint32_t dest_register, uint32_t src_register, uint32_t data_i){ 
		uint32_t reg_data,mem_data, offset;
		uint8_t data_from_mem[4];
		mips_error e;
		if(state -> get_logLevel() > 0)
			fprintf(state -> get_file_handler(), "%u, %u, %u, %u.\n", operand, dest_register, src_register, data_i);
		e = mips_cpu_get_register(state, src_register, &reg_data); 
		offset = (reg_data + data_i) % 4; 
		if (data_i >> 15)
			data_i |= 0xFFFF0000;
		e = mips_mem_read(
						state-> get_memory(), 
						reg_data + data_i - offset, 
						4, 
						data_from_mem
						);
		mem_data = data_from_mem[offset]; 
		if (mem_data >> 7)
			mem_data |= 0xFFFFFF00;
		e = state->set_register(dest_register, mem_data);
	}

	void trans_high_low(string operand, uint32_t address, uint32_t dst, mips_cpu_h state){
		if(state->get_logLevel() > 0)
			fprintf (state -> get_file_handler(), "%u, %u.\n", operand, address);
		mips_error e;
		uint32_t result; 
		if(operand == "mthi"){
			e = state->get_register(dst, &result);
			state->set_HI_register(result);
		}
		else if(operand == "mtlo"){
			e = state->get_register(dst, &result);
			state->set_LO_register(result);
		}
		else if(operand == "mfhi"){
			e = state->set_register(dst, state->get_HI_register());
		}
		else if(operand == "mflo"){
			e = state->set_register(dst, state->get_LO_register());
		}
	}

	void pShift(string operand, mips_cpu_h state, uint32_t dst, uint32_t src2, uint32_t src1){
		mips_error e;
		if (state->get_logLevel() > 0)
			fprintf (state -> get_file_handler(), "%u %u, %u, %u.\n", operand, dst, src2, src1);
		uint32_t val_to_shift, val_shift_by; 
		if (state->get_register(src2, &val_to_shift) == mips_Success && state->get_register(src1, &val_shift_by))
			e = state->set_register(dst, val_to_shift >> (val_shift_by & 31));
		else 
			return;
	}

	mips_error decode_J_instruction(uint32_t instruction, mips_cpu_h state, uint32_t opcode){
			unsigned log_level = state->get_logLevel();
			mips_error e; 
			string operand;
			uint32_t address = instruction & 0x3FFFFFF;
			if (log_level >= 2)
				fprintf (state -> get_file_handler(), "J - type: address = %u.\n  instr=%08x\n", address, instruction);
			
			switch(opcode){
				//This is J type
				case 0x02:
					operand = "j";
					break; 
			
				case 0x03: 
					operand = "jal";
					e = state->set_register(31, state->get_pcNext() + 4);
					break; 
				default:; 
			}
				
			if(log_level > 0)
				fprintf (state -> get_file_handler(), "%u, %u.\n", operand, address);
			state->set_pc(state->get_pcNext());
			state->set_pcNext((address << 2) | (state->get_pcNext()) & 0xF00000);
			return e; 
	}

	mips_error decode_R_instruction(uint32_t instruction, mips_cpu_h state, uint32_t opcode){
			mips_error e;
			unsigned log_level = state->get_logLevel();
			uint8_t memory_jump = 4; 
			uint32_t src1 = (instruction>> 21) & 0x1F;
			uint32_t src2 = (instruction>> 16) & 0x1F;   
			uint32_t dst =  (instruction>> 11 ) & 0x1F;
			uint32_t function = (instruction >> 0) & 0x3F;     
			uint32_t shift = (instruction>> 6 ) & 0x1F ;
			uint32_t address; 
			uint32_t relative_address, reg, resm; 
			uint64_t va, vb, vam, vbm, result;
			string operand;
			e = state->get_register(src1, &va);
			e = state->get_register(src2, &vb);
			vam = va >> 31; 
			vbm = vb >> 31; 
	        // This is R-type
				if(log_level >= 2){
					fprintf(state -> get_file_handler(), "R-Type : dst=%u, src1=%u, src2=%u, shift=%u, function=%u.\n  instr=%08x\n",
						dst, src1, src2, shift, function, instruction
					);
				}
			switch(function){
				
				case 0x00:
					operand = "sll";
					if (log_level > 0){
						fprintf (state -> get_file_handler(), "sll %u, %u, %u.\n", dst, src2, shift);
					}  
					e = state->get_register(src2, &result); 
					if (e == mips_Success)
						result = result << shift; 
					break; 
				
				case 0x10: 
					if (src1 | src2 | shift == 0)
						trans_high_low("mfhi", address, dst, state); 
					break;
			
				case 0x11: 
					if(src2 | dst | shift == 0)
						trans_high_low("mthi", address, src1, state); 
					break; 
				
				case 0x12: 
					if(src1 | src2 | dst == 0)
						trans_high_low("mflo", address, dst, state);
					break; 
			
				case 0x13: 
					if(src2 | dst | shift == 0)
						trans_high_low("mtlo", address, src1, state);
					break; 
					
				case 0x09: 
					if(src2 | shift == 0){
						if(log_level > 0){
							if(dst == 31)
								fprintf(state->get_file_handler(), "jalr %u .\n", src1);
							else
								fprintf(state->get_file_handler(), "jalr %u, %u.\n", dst, src1);
						}
						if(src1 == dst){
							e = mips_ExceptionInvalidInstruction; 
						}
						else{
							result = state->get_pcNext(); 
							e = state->get_register(src1, &memory_jump);
						}
					}
					break; 
				
			case 0x08: 
				if(src2 | dst | shift == 0){
					if (log_level > 0)
						fprintf (state -> get_file_handler(), "jr %u.\n", src1); 
					e = mips_cpu_get_register(state, src1, &reg);
					if (reg % 4 != 0)
						e = mips_ExceptionInvalidAddress; 
				}
				break; 
			
			case 0x03: 
				if (log_level > 0)
					fprintf (state -> get_file_handler(), "sra %u, %u, %u.\n", dst, src2, shift); 
				uint32_t shift_val, sign;
				e = state -> get_register(src2, &shift_val);
				if(e == mips_Success){
					result = shift_val >> shift; 
					sign = shift_val >> 31; 
					if (sign)
						result = ((0xFFFFFFFF << (31 - shift)) | result);
				}
				break;
			
			case 0x14: 
				if(shift | dst == 0){
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "mult %u, %u, %u.\n", dst, src1, src2);
					if (state -> get_register(src1, &va) == mips_Success && state -> get_register(src2, &vb) == mips_Success){
						result = va*vb;
						state->set_HI_register(result >> 32);
						state->set_LO_register(result);
					}
				}
				break;
			
			case 0x07: 
				if (log_level > 0)
					fprintf (state -> get_file_handler(), "srav %u, %u, %u.\n", dst, src2, src1);
				if(state->get_register(src2, &va) == mips_Success && state->get_register(src1, &vb) == mips_Success){
					result = va >> (vb & 0x1F); 
					if (vam)
						result = ((0xFFFFFFFF << (31 - vb)) | result);
					e = state->set_register(dst, result);
				}
				break; 
			
			case 0x04: 
				pShift("sllv", state, dst, src2, src1); 
				break; 
			
			case 0x06:
				pShift("srlv", state, dst, src2, src1); 
				break;
			
			case 0x02:
				if (log_level > 0)
					fprintf (state -> get_file_handler(), "srl %u, %u, %u.\n", dst, src2, shift);
				e = state->get_register(src2, &reg); 
				if (e == mips_Success)
					result = reg >> shift; 
					e = state->set_register(dst, result);
				break;
			
			case 0x20:
				if(shift == 0){
					if (log_level > 0)
						fprintf(state -> get_file_handler(), "add %u, %u, %u.\n", dst, src1, src2);
					result = va+vb;
					resm = result >> 31;    
					if(vam == vbm && vam != resm)
						e = mips_ExceptionArithmeticOverflow;
					}
				break;
			
			case 0x19:
			case 0x18: 
				result = va*vb; 
				state->set_HI_register(result << 32); 
				state->set_LO_register(result); 
				break; 
				/*logging level to be added*/
			
			case 0x2A: 
				if (log_level > 0)
					fprintf(state -> get_file_handler(), "slt %u, %u, %u.\n", dst, src1, src2);
				if (va < vb)
					result = 1; 
				else
					result = 0;
				break;
			
			case 0x16:
				if (log_level > 0)
					fprintf(state -> get_file_handler(), "sub %u, %u, %u.\n", dst, src1, src2);
				result = va - vb; 
				if((vam != vbm) && (vbm == (result >> 31)))
					e = mips_ExceptionArithmeticOverflow;
				break;
				
			case 0x17:
				if (log_level > 0)
					fprintf(state -> get_file_handler(), "subu %u, %u, %u.\n", dst, src1, src2);
				result = va - vb; 
				break; 
			
			case 0x1B: 
				if (log_level > 0)
					fprintf(state -> get_file_handler(), "xor %u, %u, %u.\n", dst, src1, src2);
	            result = va ^ vb;
				break;
			
			case 0x24:
				if(shift == 0){
					if (log_level > 0)
						fprintf(state -> get_file_handler(), "and %u, %u, %u.\n", dst, src1, src2);
					result = va & vb;
				}
				break;
			
			case 0x25:
				if (log_level > 0)
					fprintf(state -> get_file_handler(), "or %u, %u, %u.\n", dst, src1, src2);
	            result = va | vb; 
				break;
			
			case 0x2B:
				if (log_level > 0)
	                fprintf(state -> get_file_handler(), "stlu %u, %u, %u.\n", dst, src1, src2);
	            if (va < vb)
					result = 1;
				else
					result = 0;
				break; 

	        case 0x21: 
				if(shift == 0){ 
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "addu %u, %u, %u.\n", dst, src1, src2);
					result = va + vb;
				}
				break; 
			default:; 
			}
			if (e == mips_Success)
				e = state->set_register(dst, result);
			state->set_pc(state->get_pcNext());
			state->set_pcNext(state->get_pcNext() + 4);
			return e; 	

	}

	mips_error decode_I_instruction(uint32_t instruction, mips_cpu_h state, uint32_t opcode){
			unsigned log_level = state->get_logLevel();
			uint32_t src_register, dest_register, data_i, address, relative_address, result; 
			uint32_t src_data, dest_data;
			uint16_t shift_left, half_word;
			uint8_t data_from_mem[4];
			mips_error e = state->get_register(src_register, &src_data);
			uint8_t memory_jump = 4; 
			src_register = (instruction >> 21) & 0x1F;
			dest_register = (instruction >> 16) & 0x1F; 
			data_i = instruction & 0x0000FFFF;
			if(log_level >= 2){
	            fprintf(state -> get_file_handler(), "I-Type : dst=%u, src1=%u, src2=%u, shift=%u,  instr=%08x\n",
	                dest_register, src_register, data_i, instruction
	            );
	        }
			
			switch(opcode){
				case 0x06: 
				case 0x07: 
				case 0x08: 
					result = data_i << 2;
					if ((result >> 31) == 1)
						result |= 0xFFFF0000; 
					break; 
				default:; 
			}
			
			switch(opcode){
				uint32_t offset, total, byte, data_m, res, resm; 
				case 0x06: 
					if (log_level > 0)
						fprintf(state -> get_file_handler(), "BLEZ %u, %u.\n", src_register, data_i);
					if((src_data >> 31 == 1)|(src_data == 0)){
						memory_jump = res; 
						if(state -> get_pcNext() >> 31 == 1 | res % 4 != 0 )
							return mips_ExceptionInvalidAddress;
					}	
					break; 
				
				case 0x07: 
					if(dest_register == 0){
						if(log_level > 0)
							fprintf(state -> get_file_handler(), "BGTZ %u, %u.\n", src_register, data_i);
						if(src_data >> 31 == 0 && !src_data)
							memory_jump = res;   
						if(state -> get_pcNext() >> 31 == 1 | res % 4 != 0)
							return mips_ExceptionInvalidAddress;
						}
					break;
						
				case 0x0A: 
				case 0x0B:
					if(src_data < data_i)
						 e = state->set_register(dest_register, 1); 
					else 
						 e = state->set_register(dest_register, 0);
					if(log_level > 0){
						if (opcode == 0x0A)
								fprintf(state -> get_file_handler(), "SLTI &u, %u.\n", dest_register, src_register, data_i);
						else
								fprintf(state -> get_file_handler(), "SLTIU &u, %u.\n", dest_register, src_register, data_i);
					}
					break;
			
				case 0x09:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "addiu %u, %u, %u.\n", dest_register, src_register, data_i);
					e = state->get_register(src_register, &src_data);
					if (data_i >> 15) 
						data_i |= 0xFFFF0000;
					e = state->set_register(dest_register, src_data + data_i);   
					break;
			
				case 0x08:
	                if(log_level > 0)
						fprintf(state -> get_file_handler(), "addi %u, %u, %u.\n", dest_register, src_register, data_i);
	                data_m= data_i >> 15;  
					if (data_m)
						data_i |= 0xFFFF0000;
	                res = src_data + data_i;
	                resm = res >> 31;
	                if ( ((src_data >> 31) == data_m) && ((src_data >> 31) != resm))
	                	 return mips_ExceptionArithmeticOverflow;
	                else 
	                	e = state->set_register(dest_register, res); 
					break;
			
				case 0x0C:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "andi %u, %u, %u.\n", dest_register, src_register, data_i);
					e = state->set_register(dest_register, src_data & data_i);
					break;

				case 0x0D:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "ori %u, %u, %u.\n", dest_register, src_register, data_i); 
					e = state->get_register(src_register, &src_data);
					e = state->set_register(dest_register, src_data | data_i);  
					break;

				case 0x0E:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "xori %u, %u, %u.\n", dest_register, src_register, data_i);
					e = state->set_register(dest_register, src_data^data_i);
					break;
					
				case 0x2B:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "sw %u, offset(%u)", dest_register, src_register);
					e = state->get_register(dest_register, &dest_data);
					e = state->get_register(src_register, &src_data);
					data_from_mem[0] = result >> 24;
					data_from_mem[1] = result >> 16;
					data_from_mem[2] = result >> 8;
					data_from_mem[3] = result;
					data_i += value;
	                if (data_i % 4 != 0)
	                    return mips_ExceptionInvalidAlignment;
					e = mips_mem_write(
								state -> get_memory(),
								data_i,
								4,
								data_from_mem
								);	
					break;
				
				case 0x0F:
					if(src_register == 0){
						if(log_level > 0)
							fprintf(state -> get_file_handler(), "lui %u, %u, %u.\n", dest_register, data_i);
					 e = state->set_register(dest_register, data_i << 16);
					}
					break; 
					
				
				case 0x19:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "lh %u, %u, %u.\n", dest_register, src_register, data_i);
					offset = data_i;   
					address = src_data + offset; 
					relative_address = address - (address % 4);
					if (data_i >> 15)
						dest_data = 0xFFFF0000 | uint32_t(data_i);
					if (address & 0x00000001 == 1){
						return mips_ExceptionInvalidAddress; 
					}
					else{
						e = mips_mem_read(
									state -> get_memory(),
									relative_address,
									4,
									data_from_mem
									);
						if (!data_from_mem){
							res = ((uint16_t)data_from_mem[0] << 8)|
							      ((uint16_t)data_from_mem[1]<<0);
						}	
						else{
							res = ((uint16_t)data_from_mem[2] << 8)|
								  ((uint16_t)data_from_mem[3]<<0);
						} 
						dest_data = res;
						if (res >> 1 == 1) dest_data = (0xFFFF0000 | uint32_t(res)); 
						e = state->set_register(dest_register, dest_data);
					}
					break;
			
				case 0x28:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "sb %u, %u(%u).\n", dest_register, data_i, src_register);
					e = state->get_register(src_register, &src_data);
					e = state->get_register(dest_register, &dest_data);
					total = data_i + src_data; 
					byte = total % 4;  
					data_from_mem[byte] = dest_data;
				    e = mips_mem_write(
									state -> get_memory(),
									total - byte, 
									4, 
									data_from_mem
								   );
					break;
			
				case 0x25:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "lhu %u, %u, %u.\n", dest_register, src_register, data_i);
					dest_data = data_i;  
					if (data_i >> 15)
						dest_data = 0xFFFF0000 | data_i;
					if ((src_register + dest_data) & 0x00000001 == 1){
						return mips_ExceptionInvalidAddress; 
					}
					else{
						 e = mips_mem_read(
										state -> get_memory(),
										src_register + dest_data,
										4,
										data_from_mem
										);
						dest_data = to_big(data_from_mem);  
						e = state->set_register(dest_register, dest_data);
					}
					break;
			
				case 0x23:
					if(log_level > 0)
						fprintf(state -> get_file_handler(), "lw %u, %u, %u.\n", dest_register, src_register, data_i);      
					e = state->get_register(src_register, &src_data);
					e = mips_mem_read(
									state -> get_memory(),		//!< Handle to target memory
									value + data_i,	//!< Byte address to start transaction at
									4,	//!< Number of bytes to transfer
									data_from_mem//!< Receives the target bytes
									);
					e = state-> set_register(dest_register, ((((uint16_t)data_from_mem[0])<<8)
					|
					(((uint16_t)data_from_mem[1]))));
					break;
			
			case 0x29:
				if(log_level > 0)
					fprintf(state -> get_file_handler(), "sh %u, %u(%u).\n", dest_register, data_i, src_register); 
				
				e = state->get_register(src_register, &src_data);
				src_data += data_i;
				
				e = state->get_register(dest_register, &dest_data);
				half_word = 0x0000FFFF & dest_data;
				uint8_t a[2]; 
				a[0] = (half_word >>8) & 0xFF;
				a[1] = (half_word >> 8) & 0xFF;
				if(src_data % 2 !=0){
					return mips_ExceptionInvalidAlignment;
				}
				else{
					mips_error e = mips_mem_write(
					state->get_memory(),
					src_data,
					2,
					a
					); 
				}
				break; 
					
			case 0x04:
				if(log_level > 0)
	                fprintf(state -> get_file_handler(), "beq %u, %u, %u.\n", dest_register, src_register, data_i);
				e = mips_cpu_get_register(state, dest_register, &src_data);
				e = mips_cpu_get_register(state, src_register, &dest_data);  
				if(src_data == dest_data){
					res = data_i << 2;
					if (res >> 31)
						res = 0xFFFF0000 | uint32_t(res);
					memory_jump =   res; 
					if(state -> get_pcNext() >> 31 | res % 4 )
						return mips_ExceptionInvalidAddress;
				}
				break;
				
			case 0x01: 
				e = mips_cpu_get_register(state, src_register, &src_data);
				shift_left = data_i << 2; 
				res = shift_left; 
				if(shift_left >> 15)
					res |= 0xFFFF0000; 
				switch(dest_register){
					case 0x01: 
						if(log_level > 0)
							fprintf (state -> get_file_handler(), "BGEZ %u, %u.\n", src_register, data_i);
						if(shift_left >> 15 == 1 && src_data >> 31 == 0)
							memory_jump = 0xFFFF0000| shift_left;
						else if((shift_left >> 15) & (src_data >> 31) == 0)
							memory_jump = shift_left;
						break;
					
					case 0x11: 
						if(log_level > 0)
							fprintf (state -> get_file_handler(), "BGEZAL %u, %u.\n", src_register, data_i);
						e = state->set_register(31, state->get_pcNext() + 4); 
						if (!src_data >> 31)
							memory_jump = res; 
						break; 
					
					case 0x00: 
						if(log_level > 0)
							fprintf (state -> get_file_handler(), "BLTZ %u, %u.\n", src_register, data_i);
						if (src_data >> 31)
							memory_jump = res; 
						break;
						
					case 0x16: 
						if(log_level > 0)	
							fprintf(state -> get_file_handler(), "BLTZAL %u, %u.\n", src_register, data_i);	
						e = state->set_register(31, state->get_pcNext() + 4);
						if (src_data >> 31)
							memory_jump = res; 
						break; 
					default:;	
				}
			break;
						
			case 0x05:
				if(log_level > 0)
	                fprintf(state -> get_file_handler(), "bne %u, %u, %u.\n", dest_register, src_register, data_i);
				e = mips_cpu_get_register(state, dest_register, &dest_data);
				e = mips_cpu_get_register(state, src_register, &src_data);
				if(src_data != dest_data){
					uint16_t shift_left = data_i << 2;
					if (shift_left >> 15 == 1)
						res = 0xFFFF0000 | uint32_t(shift_left);
					else
						res = uint32_t(shift_left);
					memory_jump =  res; 
				}
				break;

				case 0x20: 
					load_byte(state, "lb", dest_register, src_register, data_i);
					break;

				case 0x21:
					load_byte(state, "lbu", dest_register, src_register, data_i);
					break; 
				default:;
			}
			state -> set_pc(state->get_pcNext());
			state -> set_pcNext((state->get_pcNext()) + memory_jump); 
			return e; 
		}

	mips_error mips_cpu_step(
		mips_cpu_h state	//! Valid (non-empty) handle to a CPU
	)
	{
		uint32_t instruction, opcode;
	    uint8_t buffer[4];  
	    mips_error e=
		mips_mem_read(
	        state->get_memory(),		//!< Handle to target memory
	        state->get_pc(),	//!< Byte address to start transaction at
	        4,	//!< Number of bytes to transfer
	        buffer	//!< Receives the target bytes
	    );
	    
	    if(!e)
	        return e;
	    
		e = state -> set_register(0, 0);
	    instruction = to_big(buffer);
	    opcode =  (instruction>>26) & 0x3F;
		if(opcode == 0x02 || opcode == 0x03)
			e = decode_J_instruction(instruction, state, opcode);
		else if(opcode == 0x00)
			e = decode_R_instruction(instruction, state, opcode);
		else
			e = decode_I_instruction(instruction, state, opcode);
		return e; 
		}