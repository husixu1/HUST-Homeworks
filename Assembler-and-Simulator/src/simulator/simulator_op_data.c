#include "simulator.h"
#include "simulator_op_data.h"

/* reg[7] is the 'G' register ,replace the relative addr to absolute addr */
void LOADB(void){
	/* if it's not a set or the length of the set is 1 */
	if(mem[ADDR+DS]==1)
		REG0	=	mem[ADDR+DS+1] & 0xFF;
	/* otherwise if it's a set */
    REG0	=	mem[ADDR+reg[7]+DS+1] & 0xFF;
}
void LOADW(void){
	/* if it's not a set or the length of the set is 1 */
	if(mem[ADDR+DS]==1)
		REG0	=	mem[ADDR+DS+1];
	/* otherwise if it's a set */
	REG0	=	mem[ADDR+reg[7]+DS+1];
}
void STOREB(void){
	if(mem[ADDR+DS]==1)
		mem[ADDR+DS+1]	=	REG0 & 0xFF;
	mem[ADDR+reg[7]+DS+1]	=	REG0 & 0xFF;
}
void STOREW(void){
	if(mem[ADDR+DS]==1)
		mem[ADDR+DS+1]	=	REG0;
	mem[ADDR+reg[7]+DS+1]	=	REG0;
}
void LOADI(void){
	REG0	=	IMMD&0xFF;
}
void NOP(void){
    return;
}
