#include "simulator.h"
#include "simulator_op_bitcalculate.h"
#include "simulator_op_control.h"
#include "simulator_op_data.h"
#include "simulator_op_logic.h"
#include "simulator_op_math.h"
#include "simulator_op_others.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

//============
#include "assembler.h"
extern Op op_set[32];
//============
/* memory */
int16_t *mem;
/* Z A B C D E F G */
int16_t reg[8];
/* stack register */
uint32_t CS, DS, ES, SS;
/* program register */
uint32_t PC, IR;
/* program status word */
uint16_t PSW;

/* distribute spaces and initialize the registers */
error_t	simulator_init(void){
	mem	=	(int16_t *)malloc(sizeof(int16_t)*(1<<24));
	if(!mem)
		return error_malloc;
	PC	=	0;
	IR	=	0;
	PSW	=	0;
	return no_error;
}
/* free the spaces of the simulator*/
error_t simulator_deinit(void){
	free(mem);
	return no_error;
}
error_t simulator(const char *path, Error *error){
	error	=	error;
	FILE	*file	=	fopen(path, "rb");

	/* read the size of the data*/
	fread(&ES, sizeof(uint32_t), 1, file);
	/* set the pointer to the program part */
	fseek(file, sizeof(int16_t)*ES+sizeof(uint32_t),SEEK_SET);
	/* read the program into CS stack and set DS */
	for(CS=0, DS=0; fread(&IR,sizeof(uint32_t),1,file); DS+=2){
		mem[DS]		=	((IR & 0xFFFF0000)>>16);
		mem[DS+1]	=	(IR & 0xFFFF);
	}
	fseek(file,sizeof(uint32_t),SEEK_SET);
	/* load the predefined data */
	for(uint32_t i=0; i<ES; i++)
		fread(&mem[DS+i], sizeof(int16_t), 1, file);
	ES	+=	(DS+1);
	SS	=	(1<<24)-1;
	fclose(file);

	/* run the simulator */
	for(;;PC+=2){
		IR	=	((mem[PC]<<16) | mem[PC+1]);
        switch((IR & 0xF8000000)>>27){
			case 0:		HLT();	    goto halt;
			case 1:		JMP();		break;
			case 2:		CJMP();		break;
			case 3: 	OJMP();		break;
			case 4: 	CALL();		break;
			case 5:		RET();		break;

			case 6:		PUSH();		break;
			case 7:		POP();		break;

			case 8:		LOADB();	break;
 			case 9:		LOADW();	break;
 			case 10:	STOREB();	break;
 			case 11:	STOREW();	break;
 			case 12:	LOADI();	break;
 			case 13:	NOP();		break;

 			case 14:    IN();       break;
 			case 15:    OUT();      break;

 			case 16:	ADD();		break;
 			case 17:	ADDI();		break;
 			case 18:	SUB();		break;
 			case 19:	SUBI();		break;
 			case 20:	MUL();		break;
 			case 21:	DIV();		break;

 			case 22:	AND();		break;
 			case 23:	OR();		break;
			case 24:	NOR();		break;
 			case 25:	NOTB();		break;
 			case 26:	SAL();		break;
 			case 27:	SAR();		break;

 			case 28:	EQU();		break;
 			case 29:	LT();		break;
 			case 30:	LTE();		break;
 			case 31:	NOTC();		break;
         }
	}
	halt:
	return no_error;
}
