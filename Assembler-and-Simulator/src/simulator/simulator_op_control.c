#include "simulator.h"
#include "simulator_op_control.h"

/* the implementation of control command */
inline void HLT(void){
	return;
}
inline void JMP(void){
	PC	=	(IR&0x00FFFFFF)-2;
}
inline void CJMP(void){
	PC	=	GETC?(IR&0x00FFFFFF)-2:PC;
}
inline void OJMP(void){
	PC	=	GETO?(IR&0x00FFFFFF)-2:PC;
}
inline void CALL(void){
	for(int i=0; i<8; ES++, i++)
		mem[ES]	=	reg[i];
	mem[ES]	=	PSW;	ES++;
	mem[ES]	=	(PC&0xFFFF0000)>>16;	ES++;
	mem[ES]	=	(PC&0x0000FFFF);		ES++;
	PC	=	(IR&0x00FFFFFF)-2;
}
inline void RET(void){
	ES--;	PC	=	mem[ES];
	ES--;	PC	|=	(mem[ES]<<16);
	ES--;	PSW	=	mem[ES];
	for(int i=0; i<8; i++)
		ES--, reg[7-i]	=	mem[ES];
}
