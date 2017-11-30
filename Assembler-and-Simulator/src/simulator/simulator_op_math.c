#include "simulator.h"
#include "simulator_op_math.h"

inline void ADD(void){
    UNSETO;
    REG0	=	REG1+REG2;
    /* if the sign of two number is the same and overflows */
    if( !(ISNEG(REG1)^ISNEG(REG2)) && (ISNEG(REG0)^ISNEG(REG1)) )
        SETO;
}
inline void ADDI(void){
    UNSETO;
    uint16_t temp   =   REG0;
    REG0	+=	IMMD;
    if(ISNEG(REG0)^ISNEG(temp))
        SETO;
}
inline void SUB(void){
	UNSETO;
    REG0    =   REG1-REG2;
    /* if the sign of the two number is different and overflows */
    if((ISNEG(REG1)^ISNEG(REG2) && (ISNEG(REG0)^ISNEG(REG1))) )
        SETO;
}
inline void SUBI(void){
    UNSETO;
    uint16_t temp   =   REG0;
    REG0	-=	IMMD;
    if(ISNEG(REG0)^ISNEG((-temp)))
        SETO;
}
inline void MUL(void){
    UNSETO;
    REG0    =   REG1*REG2;
    if(REG1!=0 && REG0/REG1 != REG2)
        SETO;
}
inline void DIV(void){
    UNSETO;
    if(REG2==0){
        SETO;
        return;
    }
    REG0	=	REG1/REG2;
}
