#include "simulator.h"
#include "simulator_op_logic.h"

void EQU(void){
	(REG0 == REG1)?SETC:UNSETC;
}
void LT(void){
	(REG0 < REG1)?SETC:UNSETC;
}
void LTE(void){
	(REG0 <= REG1)?SETC:UNSETC;
}
void NOTC(void){
	GETC?UNSETC:SETC;
}
