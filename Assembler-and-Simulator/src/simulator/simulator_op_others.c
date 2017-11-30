#include "simulator.h"
#include "simulator_op_others.h"
#include <stdio.h>

void PUSH(void){
	mem[SS--]	=	REG0;
}
void POP(void){
	REG0	=	mem[++SS];
}
void IN(void){
	REG0	=	getchar();
}
void OUT(void){
	putchar(REG0);
}
