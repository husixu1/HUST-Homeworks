#ifndef ASSEMBLER_PRETREATMENT_H_INCLUDED
#define ASSEMBLER_PRETREATMENT_H_INCLUDED

#include "common.h"
#include <stdio.h>


/* struct that stores the label/variable and its address */
typedef struct _Addr{
    char            is_set;		/* if the symbol is a declaration, mark if it's a set (2 for label)*/
	char			symbol[MAX_BUF_LEN];
	uint32_t		addr;
	struct _Addr	*next;
}Addr;

error_t assembler_pretreatment_stage0(const char *src, const char *dst);
error_t assembler_pretreatment_stage1(const char *src, const char *dst, Error *error);

/* the temporary data stack */
/* the structure of data stack: |space|val|space|val|...| */
extern int16_t data_stack[MAX_DATA_SPACE];
/* store the current reading address (metric: 2Byte) (for :LABEL) */
extern uint32_t pc_addr;

#endif // ASSEMBLER_PRETREATMENT_H_INCLUDED
