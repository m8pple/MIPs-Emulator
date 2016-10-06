#include "mips.h"
#include "functions.h"
#include <iostream>
using namespace std;
int main()
{
	FILE *fp;
	fp = fopen("test.txt", "w");
    mips_mem_h mem=mips_mem_create_ram(4096, 4);
    mips_cpu* cpu = new mips_cpu(); 
    cpu -> mips_cpu_create(mem);
    mips_error e=mips_cpu_set_debug_level(cpu, 4, fp);
    
	if(e!=mips_Success){
        fprintf(stderr, "mips_cpu_set_debug_level : failed.\n");
        exit(1);
    }
    
    mips_test_begin_suite();
    // 1 - Setup an instruction in ram
    // addu r3, r4, r5

e = cpu->set_register(6, 0x80000000);
e = cpu->set_register(8, 0x80000000); 

 uint32_t instr =
        (0ul << 26) // opcode = 0
        |
        (4ul << 21) // srca = r4
        |
        (5ul << 16) // srcb = r5
        |
        (19ul << 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (42ul << 0);
		
	slt(mem, cpu, instr, 0);
	
	instr =
		((8ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (45ul << 0))
        ;  
	e=cpu->set_register(9, 10);
	addi(mem, cpu, instr, 55);
	
	 instr =
		((4ul << 26) // opcode = 0
        |
		(9ul << 21) // srca = r4
		|
		(19ul << 16) // srcb = r5
		|
        (0x00000002ul << 0)) // dst = r3
        ; 
	 e=cpu->set_register(9, 10);
	 e=cpu->set_register(19, 10);		
	 beq(mem, cpu, instr, 28);
 
	  instr =
		((4ul << 26) // opcode = 0
        |
		(9ul << 21) // srca = r4
		|
		(19ul << 16) // srcb = r5
		|
        (0x00000002ul << 0)) // dst = r3
        ; 
	 e=cpu->set_register(9, 10);
	 e=cpu->set_register(19, 11);		
	 beq(mem, cpu, instr, 36);

	instr =
        (7ul << 26) // opcode = 0
        |
        (6ul << 21) // srca = r4
        |
        (0ul << 16) // srcb = r5
        |
        (3ul<< 0) // dst = r0
        ;
	e = cpu->set_register(6, 1);
	
	bgtz(mem, cpu, instr, 52 );	

	instr =
        (1ul << 26) // opcode = 0
        |
        (6ul << 21) // srca = r4
        |
        (0ul << 16) // srcb = r5
        |
        (3ul<< 0) // dst = r0
        ;
	e = cpu->set_register(6, -1);
	
	bltz(mem, cpu, instr, 68);	
	
	instr =
        (1ul << 26) // opcode = 0
        |
        (6ul << 21) // srca = r4
        |
        (0ul << 16) // srcb = r5
        |
        (3ul<< 0) // dst = r0
        ;
	e = cpu->set_register(6, 2);
	
	bltz(mem, cpu, instr, 76);
	
		

	instr =
		((14ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (16ul << 0))
        ;  	
	e=cpu->set_register(9, 10);
	XORI(mem, cpu, instr, 26);
	 
	  instr =
	   ((35ul << 26) // opcode = 0
        |
       (9ul << 21) // srca = r4
       |
        (19ul << 16) // srcb = r5
        |
        (8ul << 0)) // dst = r3
        ;  
	  uint8_t rec[4];
		rec[0] = 0xFF; 
		rec[1] = 0x00;
		rec[2] = 0x00; 
		rec[3] = 0x00;

		e = mips_mem_write(
			mem, 
			12, 
			4, 
			rec
		);
	 e=cpu->set_register(9, 4);
	lw(mem, cpu, instr, 0xFF00);
	 
	instr =
		(0ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (10ul << 16) // srcb = r5
        |
        (11ul << 11) // dst = r3
        |
        (0ul << 6) // shift = 0
        |
        (0x25 << 0);		
 
    // 2 - put register values in cpu
    e=cpu->set_register(9, 40);
    e=cpu->set_register(10, 50);
    OR(mem, cpu, instr, 40| 50);

	 instr =
		(9ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (10ul << 0) // dst = r3
        ;  
    e=cpu->set_register(9, 40);
	
    addiu(mem, cpu, instr, 50);
 
   	instr =
        (0ul << 26) // opcode = 0
        |
        (22ul << 21) // srca = r4
        |
        (23ul << 16) // srcb = r5
        |
        (24ul << 11) // dst = r3
        |
        (0ul << 6) // shift = 0
        |
        (0x22 << 0);
    
    // 2 - put register values in cpu
    e=cpu->set_register(22, 50);
    e=cpu->set_register(23, 40);
    subu(mem, cpu, instr, 10);  
    
	// 1 - Setup an instruction in ram
	instr =
        (0ul << 26) // opcode = 0
        |
        (25ul << 21) // srca = r4
        |
        (27ul << 16) // srcb = r5
        |
        (28ul << 11) // dst = r3
        |
        (0ul << 6) // shift = 0
        |
        (0x26 << 0);
    
    // 2 - put register values in cpu
    e=cpu->set_register(25, 1);
    e=cpu->set_register(27, 1);
    XOR(mem, cpu, instr, 0);    
    
    // 1 - Setup an instruction in ram
    // addu r3, r4, r5
   	instr =
        (0ul << 26) // opcode = 0
        |
        (14ul << 21) // srca = r4
        |
        (15ul << 16) // srcb = r5
        |
        (16ul << 11) // dst = r3
        |
        (0ul << 6) // shift = 0
        |
        (0x24 << 0);
 	
    // 2 - put register values in cpu
    e=cpu->set_register(14, 40);
    e=cpu->set_register(15, 50);
    AND(mem, cpu, instr, (40 & 50));
    
   	instr =
        (0ul << 26) // opcode = 0
        |
        (4ul << 21) 
        |
        (5ul << 16) // srca = r5
        |
        (3ul << 11) // dst = r3
        |
        (0ul << 6) // shift = 3
        |
        (0x04 << 0);
    
    
    // 2 - put register val
	e=cpu->set_register(4, 1);
    e=cpu->set_register( 5, 2);
    sllv(mem, cpu, instr, 4);

   	instr =
        (0ul << 26) // opcode = 0
        |
        (4ul << 21) // srca = r4
        |
        (5ul << 16) // srcb = r5
        |
        (3ul << 11) // dst = r3
        |
        (0ul << 6) // shift = 0
        |
        (0x21 << 0);
    
    // 2 - put register values in cpu
    e=cpu->set_register( 4, 40);
    e=cpu->set_register( 5, 50);
	addu(mem, cpu, instr, 90);   
    
    // 1 - Setup an instruction in ram
    instr =
        (0ul << 26) // opcode = 0
        |
        (4ul << 21) // srca = r4
        |
        (5ul << 16) // srcb = r5
        |
        (19ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x20 << 0);

    e=cpu->set_register( 4, 0xFFFFFFFF);
    e=cpu->set_register( 5, 0xFFFFFFFF);
    add(mem, cpu, instr, 0xFFFFFFFE);
        
    instr =
        (0ul << 26) // opcode = 0
        |
        (4ul << 21) // srca = r4
        |
        (5ul << 16) // srcb = r5
        |
        (0ul << 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x21 << 0);
    
    // 2 - put register values in cpu
    e=cpu->set_register( 4, 4294967296);
    e=cpu->set_register( 5, 4294967296);
	addu(mem, cpu, instr, 0);
	
	    
    // 1 - Setup an instruction in ram
    // addu r3, r4, r5
    instr =
        (43ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (4ul << 0) // dst = r0
        ;
    
    // 2 - put register values in cpu
    e= cpu->set_register( 19, 28); 
    sw (mem, cpu,  instr, 28); 
	 
	 uint32_t res = 0xA4218926;
	 uint8_t buffer[4]; 
    buffer[0]=(res >>24)&0xFF;
    buffer[1]=(res >>16)&0xFF;
    buffer[2]=(res >>8)&0xFF;
    buffer[3]=(res >>0)&0xFF;
	e = mips_mem_write(
		mem, 
		32,
		4, 
		buffer
	);
	instr =
        (32ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (4ul << 0)
;		// dst = r0
	e = cpu->set_register( 9, 29);
   LB(mem, cpu, instr, 33);
   
    res = 0xA4FF89FF; 
    buffer[0]=(res >>24)&0xFF;
    buffer[1]=(res >>16)&0xFF;
    buffer[2]=(res >>8)&0xFF;
    buffer[3]=(res >>0)&0xFF;
	e = mips_mem_write(
		mem, 
		32,
		4, 
		buffer
	);
	instr =
        (32ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (4ul << 0)
;		// dst = r0
	e = cpu->set_register( 9, 29);
   LB(mem, cpu, instr, -1);
   
   instr =
        (36ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (4ul << 0)
;		// dst = r0
	e = cpu->set_register( 9, 29);
   LBU(mem, cpu, instr, 0xFF);
   
   instr =
        (40ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (4ul << 0)
;		
	e = cpu->set_register( 9, 25);
	e = cpu->set_register( 19, 10);
	SB(mem, cpu, instr, 10);
	
	instr = 
		(0ul << 26)
		 |
        (6ul << 21) // srca = r4
        |
        (8ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x22 << 0);
   e = cpu->set_register( 6, -8);
   e = cpu->set_register( 8, -2);
   sub(mem, cpu, instr, -6);
   
     instr =
		((15ul << 26) // opcode = 0
        |
        (0ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (0xFFFFul << 0))
        ;  
	lui(mem, cpu, instr, 0xFFFF0000);

   instr = 
		(0ul << 26)
		 |
        (6ul << 21) // srca = r4
        |
        (8ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x22 << 0);
   e = cpu->set_register( 6, -2);
   e = cpu->set_register( 8, -2);
   sub(mem, cpu, instr, 0);
   
   instr = 
		(5ul << 26)
		 |
        (9ul << 21) // srca = r4
        |
        (10ul << 16) // srcb = r5
        |
        (0ul << 0); // shift = 0
        
   e = cpu->set_register( 9, -4);
   e = cpu->set_register( 10, -4);
   uint32_t x; 
   x = cpu->get_pc(); 
   bne(mem, cpu, instr, x + 8);
 
   instr = 
		(5ul << 26)
		 |
        (6ul << 21) // srca = r4
        |
        (8ul << 16) // srcb = r5
        |
        (2ul << 0); // shift = 0
   x = cpu->get_pc();
   e = cpu->set_register( 6, 1);
   e = cpu->set_register( 8, 1);
   bne(mem, cpu, instr, x + 8);
   
   instr = 
		(33ul << 26)
		 |
        (6ul << 21) // srca = r4
        |
        (8ul << 16) // srcb = r5
        |
        (2ul << 0); // shift = 0
  e = cpu->set_register( 6, 56); 
    res = 0x89261234;
    buffer[0]=(res >>24)&0xFF;
    buffer[1]=(res >>16)&0xFF;
    buffer[2]=(res >>8)&0xFF;
    buffer[3]=(res >>0)&0xFF;
  e = mips_mem_write(
  mem, 
  56, 
  4, 
  buffer
  ); 
  lh(mem, cpu, instr, 0x1234);

 instr =
        (0ul << 26) // opcode = 0
        |
        (6ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x2B << 0);
e = cpu->set_register( 6, 0xFFFFFFFF);
e = cpu->set_register( 7, 0x00000002);
SLTU(mem, cpu, instr, 0);

instr = 
		(0ul << 26) // opcode = 0
        |
        (6ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x2A << 0);
slt(mem, cpu, instr, 1);


instr = 
		(0ul << 26) // opcode = 0
        |
        (0ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (3ul << 6) // shift = 0
        |
        (0x03 << 0);
e = cpu->set_register( 7, 64); 
SRA(mem, cpu, instr, 8);

instr = 
		(0ul << 26) // opcode = 0
        |
        (0ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (3ul << 6) // shift = 0
        |
        (0x03 << 0);
e = cpu->set_register( 7, -64); 
SRA(mem, cpu, instr, -8);

instr = 
		(0ul << 26) // opcode = 0
        |
        (0ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x03 << 0);
e = cpu->set_register( 7, 10); 
SRA(mem, cpu, instr, 10);


instr = 
		(0ul << 26) // opcode = 0
        |
        (8ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x07 << 0);
	e = cpu->set_register( 8, -1);
	e = cpu->set_register( 7, 0xFFFFFFFF); 
	SRAV(mem, cpu, instr, -1);
	
instr = 
		(0ul << 26) // opcode = 0
        |
        (8ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x07 << 0);
	e = cpu->set_register( 8, -1);
	e = cpu->set_register(7, 0xF0000000); 
	SRAV(mem, cpu, instr, -1);
	
instr = 
		(0ul << 26) // opcode = 0
        |
        (8ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x07 << 0);
	e = cpu->set_register( 8, -1);
	e = cpu->set_register( 7, 0x0F000000); 
	SRAV(mem, cpu, instr, 0);
	
	instr = 
		(0ul << 26) // opcode = 0
        |
        (0ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (24ul << 6) // shift = 0
        |
        (0x02 << 0);
	e = cpu->set_register( 7, 0x0F000000); 
	SRL(mem, cpu, instr, 15);

	instr = 
		(0ul << 26) // opcode = 0
        |
        (0ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (45ul << 6) // shift = 0
        |
        (0x02 << 0);
	e = cpu->set_register( 7, -1); 
	SRL(mem, cpu, instr, 0x7FFFF);
	
	
instr = 
		(0ul << 26) // opcode = 0
        |
        (0ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (10ul<< 11) // dst = r0
        |
        (28ul << 6) // shift = 0
        |
        (0x02 << 0);
	e = cpu->set_register( 7, -2); 
	SRL(mem, cpu, instr, 15);
	uint8_t recg[4]; recg[0] = 0; recg[1] = 0; recg[2] = 0; recg[3] = 0; 
	uint32_t pc1 = cpu->get_pc();
	e = mips_mem_write(
		mem,
		pc1 + 4,
		4,
		recg
	);
	
	instr = 
	(2ul << 26)
	|
	(10ul << 0);
	J(mem, cpu, instr, 0x00000028);
	
e=cpu->set_register( 6, 0x0000000A);
e= cpu->set_register( 7, 0x0000000A);

 instr =
        (0ul << 26) // opcode = 0
        |
        (6ul << 21) // srca = r4
        |
        (7ul << 16) // srcb = r5
        |
        (0ul<< 11) // dst = r0
        |
        (0ul << 6) // shift = 0
        |
        (0x20 << 0);

add(mem, cpu, instr, 0);


instr =
		((8ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (45ul << 0))
        ;  
	e=cpu->set_register( 9, -10);
	addi(mem, cpu, instr, 35);
	
instr =
		((8ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (45ul << 0))
        ;  
	e=cpu->set_register( 9, -90);
	addi(mem, cpu, instr, -45);
	
	instr =
		((8ul << 26) // opcode = 0
        |
        (9ul << 21) // srca = r4
        |
        (19ul << 16) // srcb = r5
        |
        (45ul << 0))
        ;  
	e=cpu->set_register( 9, -2);
	addi(mem, cpu, instr, 43);
	
	
	instr = 
	(2ul << 26)
	|
	(12ul << 0);
	J(mem, cpu, instr, 0x00000030);
	mips_test_end_suite();
    
    return 0;

	}

void slt(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){   
	mips_error e; 
	uint32_t pc = cpu->get_pc();
	int testId = mips_test_begin_test("slt");
	uint8_t buffer[4]; 
    buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
     e=mips_mem_write(
         mem,	        //!< Handle to target memory
         pc,	            //!< Byte address to start transaction at
        4,	            //!< Number of bytes to transfer
         buffer	        //!< Receives the target bytes
    );
	

if(e!=mips_Success){
       fprintf(cpu->get_file_handler(), "mips_mem_write : failed.\n");
        exit(1);
    }
    e=mips_cpu_step(cpu);
		 if(e!=mips_Success){
			fprintf(cpu->get_file_handler(), "mips_cpu_step : failed.\n");
			exit(1);
		}
    // 4 -Check the result
	uint32_t got;
	uint32_t address = ((instr >> 11) & 0x1F);
	e=cpu->get_register( address, &got); 
	int passed = got == val;
	 mips_test_end_test(testId, passed, "r0 <> 0");
}

void addi(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	int testId = mips_test_begin_test("addi");
	mips_error e; 
	uint32_t pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	         //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
	 
    if(e!=mips_Success){
		fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
   
    // 3 - step CPU
    int passed; 
	e=mips_cpu_step(cpu);
	 if (e == mips_ExceptionArithmeticOverflow){ 
		fprintf(cpu->get_file_handler(), "overflow detected!.\n");
		passed = 1; 
	}
    if((e!=mips_Success) && (e!=mips_ExceptionArithmeticOverflow)){
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }
	if (e != mips_ExceptionArithmeticOverflow){
    uint32_t got;
    // 4 -Check the result
	uint32_t address = ((instr >> 16) & 0x1F);
    e=cpu->get_register(address, &got);
     passed = got == val;
    }
    mips_test_end_test(testId, passed, "50 + 50 != 90");
}

void beq(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("beq"); 
	uint32_t pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);		
		if(e!=mips_Success){
			fprintf(stderr, "mips_mem_write : failed.\n");
			exit(1);
		}
    
    // 3 - step CPU
   e = mips_cpu_step(cpu);
   e = mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }  
    //4 -Check the result
	uint32_t address, address1, reg1, reg2, res;
	address = (instr >> 21) & 0x1F;
	 e = cpu->get_register(address, &reg1);
	address1 = (instr >> 16) & 0x1F; 
	e = cpu->get_register(address1, &reg2);
	uint16_t val2 = ((instr >> 0) & 0xFFFF);
	uint16_t shift = val2 << 2;
    if((val2 >> 15) == 1)
		res = 0xFFFF0000 | uint32_t(shift);
	else
		res = uint32_t(shift);
	uint32_t got;
	if (reg1 == reg2){
	 got = pc + 4 +res; 
	}
	else{
		got = pc + 8; 
	}
	 int passed = got == val;
     mips_test_end_test(testId, passed, "50 + 50 != 90");
}

void lw(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("lw");
	uint32_t pc = cpu->get_pc();	 
    uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint32_t got;
	uint32_t address = ((instr >> 16) & 0x1F);
	 e=cpu->get_register(address, &got);
	 
	int passed = got == val;
    
    mips_test_end_test(testId, passed, "50 + 50 != 90");
}

void OR(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("or");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint32_t address = ((instr >> 11) & 0x1F);
	uint32_t got;
	e=cpu->get_register(address, &got);
	 int passed = got == val;
	  mips_test_end_test(testId, passed, "50 + 50 != 90");
}
void addu(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("addu");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint32_t address = ((instr >> 11) & 0x1F);
	uint32_t got;
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	mips_test_end_test(testId, passed, "50 + 50 != 90");
}  
  void add(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("add");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		int passed;
		
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
		}
    // 3 - step CPU
   e=mips_cpu_step(cpu);

   if (e == mips_ExceptionArithmeticOverflow){ 
			fprintf(cpu->get_file_handler(), "overflow detected!.\n");
			passed = 1; 
		}
    if(e!=mips_Success && e != mips_ExceptionArithmeticOverflow){ 
		fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	if (e == mips_Success){
		uint32_t address = ((instr >> 11) & 0x1F);
		uint32_t got;
		e=cpu->get_register(address, &got); 
		passed = got == val;
	}
	mips_test_end_test(testId, passed, "50 + 50 != 90");
} 

void addiu(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("addiu");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint32_t address = ((instr >> 16) & 0x1F);
	uint32_t got;
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	mips_test_end_test(testId, passed, "50 + 50 != 90");
}    

void XOR(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("xor");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint32_t address = ((instr >> 11) & 0x1F);
	uint32_t got;
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	mips_test_end_test(testId, passed, "50 + 50 != 90");
}

void AND(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("and");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint32_t address = ((instr >> 11) & 0x1F);
	uint32_t got;
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	mips_test_end_test(testId, passed, "50 + 50 != 90");
}



void subu(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e;
	int testId = mips_test_begin_test("subu");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint32_t address = ((instr >> 11) & 0x1F);
	uint32_t got;
	e=cpu->get_register( address, &got); 
	int passed = got == val;
	mips_test_end_test(testId, passed, "50 + 50 != 90");
}

void sllv(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("sllv");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint32_t address = ((instr >> 11) & 0x1F);
	uint32_t got;
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	mips_test_end_test(testId, passed, "50 + 50 != 90");
}

void sw(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("sw");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    
    // 3 - step CPU
   e=mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	uint8_t m[4];
	uint32_t a = ((instr >> 21) & 0x1F);
	uint32_t x;
	 e = cpu->get_register( a, &x);
	uint32_t dat = ((instr >> 0) &0xFFFF); 
	uint32_t effective_address = x + dat; 
	e=mips_mem_read(mem, effective_address, 4, m);
    uint32_t got = (((uint32_t)m[0])<<24)
        |
        (((uint32_t)m[1])<<16)
        |
        (((uint32_t)m[2])<<8)
        |
        (((uint32_t)m[3])<<0);
	int passed = got == val;
	mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void XORI(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	int testId = mips_test_begin_test("xori");
	mips_error e; 
	uint32_t pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	         //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
	 
    if(e!=mips_Success){
		fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
   
    // 3 - step CPU
    e=mips_cpu_step(cpu);
    if((e!=mips_Success)){
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }
    uint32_t got;
    // 4 -Check the result
	uint32_t address = ((instr >> 16) & 0x1F);
    e=cpu->get_register( address, &got);
    int passed = got == val;
    
    mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void LB(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	int testId = mips_test_begin_test("lb");
	mips_error e; 
	uint32_t pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	         //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
	 
    if(e!=mips_Success){
		fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
   
    // 3 - step CPU
    e=mips_cpu_step(cpu);
    if((e!=mips_Success)){
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }
    uint32_t got;
    // 4 -Check the result
	uint32_t address = ((instr >> 16) & 0x1F);
    e=cpu->get_register( address, &got);
    int passed = got == val;
    
    mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void SB(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	int testId = mips_test_begin_test("sb");
	mips_error e;
    uint32_t pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	         //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
	 
    if(e!=mips_Success){
		fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
    e=mips_cpu_step(cpu);
    if((e!=mips_Success)){
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }
	uint32_t src1 = (instr >> 21) & 0x1F;
	uint16_t src2 = (instr >> 0) & 0xFFFF;
	int32_t dat; 
	if((src2 >> 15) == 1)
		dat = int32_t(src2);
	else
		dat = uint32_t(src2);
	uint32_t got;
	e = cpu->get_register( src1, &got);
	uint32_t y = dat + got;
	uint32_t mod = y % 4;
	uint8_t data[4];
	e = mips_mem_read(
	mem,
	y - mod,
	4, 
	data
	); 
     got = data[mod];
    int passed = got == val;
    mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void sub(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("sub");
	uint32_t pc = cpu->get_pc();
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
		int passed;
		
		if(e!=mips_Success){
        fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
		}
    // 3 - step CPU
   e=mips_cpu_step(cpu);
   if (e == mips_ExceptionArithmeticOverflow){ 
			fprintf(cpu->get_file_handler(), "overflow detected!.\n");
			passed = 1; 
		}
    if(e!=mips_Success && e != mips_ExceptionArithmeticOverflow){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    } 
	if (e == mips_Success){
		uint32_t address = ((instr >> 11) & 0x1F);
		uint32_t got;
		e=cpu->get_register(address, &got); 
		passed = got == val;
	}
	mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void bne(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
	int testId = mips_test_begin_test("bne"); 
	uint32_t pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);		
		uint8_t d[4]; d[0] = 0; d[1] = 0; d[2] = 0; d[3] = 0; 
	e = mips_mem_write(
	mem, 
	pc + 4,
	4, 
	d
	);
		if(e!=mips_Success){
			fprintf(stderr, "mips_mem_write : failed.\n");
			exit(1);
		}
    
    // 3 - step CPU
   e = mips_cpu_step(cpu);
   e = mips_cpu_step(cpu);
    if(e!=mips_Success){
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }  
    //4 -Check the result
	uint32_t address;
	uint32_t reg1; 
	uint32_t reg2;
	address = (instr >> 21) & 0x1F;
	 e = cpu->get_register(address, &reg1);
	address = (instr >> 16) & 0x1F;
	e = cpu->get_register( address, & reg2);
	uint16_t val2 = ((instr >> 0) & 0xFFFF);
	uint16_t shift = val2 << 2;
	
	uint32_t res;
    if((val2 >> 15) == 1){
		res = 0xFFFF0000 | uint32_t(shift);
	}
	else{
		res = uint32_t(shift);
	}
	uint32_t got;
	if (reg1 != reg2){ 
	 got = pc + 4 +res;
	}
	else{
		got = pc + 8; 
	} 
	 int passed = got == val;
     mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void lh(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e;
    int testId = mips_test_begin_test("lh");
	uint32_t pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	         //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
	 
    if(e!=mips_Success){
		fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
   
    // 3 - step CPU
    e=mips_cpu_step(cpu);
    if((e!=mips_Success)){
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }
    uint32_t got;
    // 4 -Check the result
	uint32_t address = ((instr >> 16) & 0x1F);
    e=cpu->get_register( address, &got);
    int passed = got == val;
	mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void LBU(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
		int testId = mips_test_begin_test("lbu");
	mips_error e; 
    uint32_t pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
		e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	         //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);
	 
    if(e!=mips_Success){
		fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
   
    // 3 - step CPU
    e=mips_cpu_step(cpu);
    if((e!=mips_Success)){
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }
    uint32_t got;
    // 4 -Check the result
	uint32_t address = ((instr >> 16) & 0x1F);
    e=cpu->get_register( address, &got);
	int passed = got == val;
    mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void SLTU(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e;
    uint32_t pc = cpu->get_pc();
	int testId = mips_test_begin_test("sltu");
	uint8_t buffer[4]; 
    buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
  e=mips_mem_write(
     mem,	        //!< Handle to target memory
     pc,	            //!< Byte address to start transaction at
     4,	            //!< Number of bytes to transfer
     buffer	        //!< Receives the target bytes
   );
	

if(e!=mips_Success){
       fprintf(cpu->get_file_handler(), "mips_mem_write : failed.\n");
        exit(1);
    }
    e=mips_cpu_step(cpu);
		 if(e!=mips_Success){
			fprintf(cpu->get_file_handler(), "mips_cpu_step : failed.\n");
			exit(1);
		}
    // 4 -Check the result
	uint32_t got;
	uint32_t address = ((instr >> 11) & 0x1F);
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	 mips_test_end_test(testId, passed, "r0 <> 0");
	}
	
void SRA(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	mips_error e; 
    uint32_t pc = cpu->get_pc();
	int testId = mips_test_begin_test("sra");
	uint8_t buffer[4]; 
    buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    e=mips_mem_write(
                    mem,	        //!< Handle to target memory
                    pc,	            //!< Byte address to start transaction at
                    4,	            //!< Number of bytes to transfer
                    buffer	        //!< Receives the target bytes
                );
	

if(e!=mips_Success){
       fprintf(cpu->get_file_handler(), "mips_mem_write : failed.\n");
        exit(1);
    }
    e=mips_cpu_step(cpu);
		 if(e!=mips_Success){
			fprintf(cpu->get_file_handler(), "mips_cpu_step : failed.\n");
			exit(1);
		}
    // 4 -Check the result
	uint32_t got;
	uint32_t address = ((instr >> 11) & 0x1F);
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	 mips_test_end_test(testId, passed, "r0 <> 0");
	}
	
void SRAV(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){ 
	uint32_t pc = cpu->get_pc();
	int testId = mips_test_begin_test("srav");
	uint8_t buffer[4]; 
    buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
  mips_error e=mips_mem_write(
     mem,	        //!< Handle to target memory
     pc,	            //!< Byte address to start transaction at
     4,	            //!< Number of bytes to transfer
     buffer	        //!< Receives the target bytes
   );
	

if(e!=mips_Success){
       fprintf(cpu->get_file_handler(), "mips_mem_write : failed.\n");
        exit(1);
    }
    e=mips_cpu_step(cpu);
		 if(e!=mips_Success){
			fprintf(cpu->get_file_handler(), "mips_cpu_step : failed.\n");
			exit(1);
		}
    // 4 -Check the result
	uint32_t got;
	uint32_t address = ((instr >> 11) & 0x1F);
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	 mips_test_end_test(testId, passed, "r0 <> 0");
	}
	
	void SRL(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
		uint32_t pc = cpu->get_pc();
		int testId = mips_test_begin_test("srl");
		uint8_t buffer[4]; 
		buffer[0]=(instr>>24)&0xFF;
		buffer[1]=(instr>>16)&0xFF;
		buffer[2]=(instr>>8)&0xFF;
		buffer[3]=(instr>>0)&0xFF;
	mips_error e=mips_mem_write(
		mem,	        //!< Handle to target memory
		pc,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
	);
	
if(e!=mips_Success){
       fprintf(cpu->get_file_handler(), "mips_mem_write : failed.\n");
        exit(1);
    }
 e=mips_cpu_step(cpu);
		 if(e!=mips_Success){
			fprintf(cpu->get_file_handler(), "mips_cpu_step : failed.\n");
			exit(1);
		}
    // 4 -Check the result
	uint32_t address = ((instr >> 11) & 0x1F);
	uint32_t got;
	e=cpu->get_register(address, &got); 
	int passed = got == val;
	 mips_test_end_test(testId, passed, "r0 <> 0");
	}
	
	void J(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){ 
		uint32_t pc = cpu->get_pc(); 
		int testId = mips_test_begin_test("j");
		uint8_t buffer[4]; 
		buffer[0]=(instr>>24)&0xFF;
		buffer[1]=(instr>>16)&0xFF;
		buffer[2]=(instr>>8)&0xFF;
		buffer[3]=(instr>>0)&0xFF;
	mips_error e=mips_mem_write(
		mem,	        //!< Handle to target memory
		pc,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
	);
	
if(e!=mips_Success){
       fprintf(cpu->get_file_handler(), "mips_mem_write : failed.\n");
        exit(1);
    }
    e=mips_cpu_step(cpu);
	e=mips_cpu_step(cpu);
	if(e!=mips_Success){
			fprintf(cpu->get_file_handler(), "mips_cpu_step : failed.\n");
			exit(1);
	}
	pc = cpu->get_pc();
    // 4 -Check the result  
	int passed = pc == val;
	 mips_test_end_test(testId, passed, "r0 <> 0");
	}
	
void JALR(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
		uint32_t pc;  
		int testId = mips_test_begin_test("jalr");
			pc = cpu->get_pc();
		
		uint32_t imp = pc + 4; 
		uint8_t buffer[4]; 
		buffer[0]=(instr>>24)&0xFF;
		buffer[1]=(instr>>16)&0xFF;
		buffer[2]=(instr>>8)&0xFF;
		buffer[3]=(instr>>0)&0xFF;
	 mips_error e=mips_mem_write(
		mem,	        //!< Handle to target memory
		pc,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
	);
	
if(e!=mips_Success){
       fprintf(cpu->get_file_handler(), "mips_mem_write : failed.\n");
        exit(1);
    }
	e=mips_cpu_step(cpu);
	e=mips_cpu_step(cpu);
	if(e!=mips_Success){
			fprintf(cpu->get_file_handler(), "mips_cpu_step : failed.\n");
			exit(1);
	}
    pc = cpu->get_pc();
    // 4 -Check the result  
		uint32_t rd = ((instr>> 11 ) & 0x1F);
		int passed;
		uint32_t get_reg; 
		e = cpu->get_register( rd, &get_reg);
		uint32_t pc1;
		pc1 = cpu->get_pc();
		 
	if((pc1 == val) && (get_reg == imp)){
		passed = 1;
	}
	else{
		passed = 0; 
	}
	 mips_test_end_test(testId, passed, "r0 <> 0");
	}
	
	void bgtz(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
		 uint32_t pc; 
	 int testId = mips_test_begin_test("bgtz"); 
	pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
		mips_error e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);		
		if(e!=mips_Success){
			fprintf(stderr, "mips_mem_write : failed.\n");
			exit(1);
		}
		
	uint8_t d[4]; d[0] = 0; d[1] = 0; d[2] = 0; d[3] = 0; 
	e = mips_mem_write(
	mem, 
	pc + 4,
	4, 
	d
	);
    
    // 3 - step CPU
   e = mips_cpu_step(cpu);
   e = mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }  
    //4 -Check the result
	uint32_t address;
	uint32_t reg1; 
	address = (instr >> 21) & 0x1F; 
	 e = cpu->get_register(address, &reg1); 
	uint16_t val2 = ((instr >> 0) & 0xFFFF);
	uint16_t shift = val2 << 2;
	
	uint32_t res;
    if((val2 >> 15) == 1){
		res = 0xFFFF0000 | uint32_t(shift);
	}
	else{
		res = uint32_t(shift);
	}
	uint32_t got; 
	if (((reg1  >> 31) == 0) && ((reg1) != 0)){  
		got = pc + 4 +res;  
		}
	else{
		got = pc + 8; 
	} 
	 int passed = got == val;
     mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void bltz(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
		uint32_t pc; 
	 int testId = mips_test_begin_test("bltz"); 
	 pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
		mips_error e=mips_mem_write(
			mem,	        //!< Handle to target memory
			pc,	            //!< Byte address to start transaction at
			4,	            //!< Number of bytes to transfer
			buffer	        //!< Receives the target bytes
		);		
		if(e!=mips_Success){
			fprintf(stderr, "mips_mem_write : failed.\n");
			exit(1);
		}
    
    // 3 - step CPU
	uint8_t d[4]; d[0] = 0; d[1] = 0; d[2] = 0; d[3] = 0; 
	e = mips_mem_write(
	mem, 
	pc + 4,
	4, 
	d
	);
   e = mips_cpu_step(cpu);
   e = mips_cpu_step(cpu);
    if(e!=mips_Success){ 
        fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }  
    //4 -Check the result
	uint32_t address;
	uint32_t reg1; 
	address = (instr >> 21) & 0x1F; 
	 e = cpu->get_register(address, &reg1); 
	uint16_t val2 = ((instr >> 0) & 0xFFFF);
	uint16_t shift = val2 << 2;
	
	uint32_t res;
    if((val2 >> 15) == 1){
		res = 0xFFFF0000 | uint32_t(shift);
	}
	else{
		res = uint32_t(shift);
	}
	uint32_t got; 
	if (((reg1 >> 31) == 1) && ((reg1) != 0)){ 
		got = pc + 4 +res; 
	}
	else{
		got = pc + 8; 
	} 
	 int passed = got == val;
     mips_test_end_test(testId, passed, "50 + 50 != 90");
	}
	
	void lui(mips_mem_h mem, mips_cpu* cpu, uint32_t instr, uint32_t val){
	int testId = mips_test_begin_test("lui");
	uint32_t pc; 
	pc = cpu->get_pc(); 
	uint8_t buffer[4];
	buffer[0]=(instr>>24)&0xFF;
    buffer[1]=(instr>>16)&0xFF;
    buffer[2]=(instr>>8)&0xFF;
    buffer[3]=(instr>>0)&0xFF;
    
	mips_error e=mips_mem_write(
		mem,	        //!< Handle to target memory
		pc,	         //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
	);
	 
    if(e!=mips_Success){
		fprintf(stderr, "mips_mem_write : failed.\n");
        exit(1);
    }
   
    // 3 - step CPU
    int passed; 
	e=mips_cpu_step(cpu);
    if((e!=mips_Success)){
		fprintf(stderr, "mips_cpu_step : failed.\n");
        exit(1);
    }
    uint32_t got;
    // 4 -Check the result
	uint32_t address = ((instr >> 16) & 0x1F);
    e = cpu -> get_register(address, &got);
     passed = got == val;
    mips_test_end_test(testId, passed, "50 + 50 != 90");	
	}
	