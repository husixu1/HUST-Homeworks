#include"simulator.h"
#include"simulator_op_bitcalculate.h"

void AND(void){
    REG0	=	REG1 & REG2;
}
void OR(void){
	REG0	=	REG1 | REG2;
}
void NOR(void){
	REG0	=	REG1 ^ REG2;
}
void NOTB(void){
	REG0	=	~REG1;
}
void SAL(void){
	REG0	=	REG1<<REG2;
}
void SAR(void){
	REG0	=	REG1>>REG2;
}
