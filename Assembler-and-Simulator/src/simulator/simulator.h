#ifndef SIMULATOR_H_INCLUDED
#define SIMULATOR_H_INCLUDED

#include "common.h"
#include "simulator.h"
/* memory */
extern int16_t *mem;
/* Z A B C D E F G */
extern int16_t reg[8];
/* stack register */
extern uint32_t CS, DS, ES, SS;
/* program register */
extern uint32_t PC, IR;
/* program status word */
extern uint16_t PSW;
/* control on overflow flag*/
#define SETO	(PSW|=0x0001)
#define UNSETO	(PSW&=0xFFFE)
#define GETO	(PSW&0x0001)
/* control on compare flag*/
#define SETC	(PSW|=0x0002)
#define UNSETC	(PSW&=0xFFFD)
#define GETC	((PSW&0x02)>>1)
/* the the nth reg in the command */
#define REG0	(reg[((IR & 0x07000000)>>24)])
#define REG1	(reg[((IR & 0x00F00000)>>20)])
#define REG2	(reg[((IR & 0x000F0000)>>16)])
#define	ADDR	((IR & 0x00FFFFFF))
#define IMMD	((int16_t)(IR & 0x0000FFFF))
#define IMMD_MAX	((int16_t)(0x7FFF))

/* judge if a number is a negative number */
#define ISNEG(reg)  (((reg)&0x80000000)>>31)

error_t simulator_init(void);

error_t simulator(const char *path, Error *error);
#endif // SIMULATOR_H_INCLUDED
