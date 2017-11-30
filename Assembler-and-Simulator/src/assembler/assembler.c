#include "assembler.h"
#include "assembler_pretreatment.h"
#include "assembler_replacement.h"
#include "hash.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

/* the operator set */
const Op op_set[OP_NUM]	=	{
	{"HLT",		(0<<27),	o5_p27			},	{"JMP",		(1<<27),	o5_p3_a24		},
	{"CJMP",	(2<<27),	o5_p3_a24		},	{"OJMP",	(3<<27),	o5_p3_a24		},
	{"CALL",	(4<<27),	o5_p3_a24		},	{"RET",		(5<<27),	o5_p27			},
	{"PUSH",	(6<<27),	o5_r3_p24		},	{"POP",		(7<<27),	o5_r3_p24		},
	{"LOADB",	(8<<27),	o5_r3_a24		},	{"LOADW",	(9<<27),	o5_r3_a24		},
	{"STOREB",	(10<<27),	o5_r3_a24		},	{"STOREW",	(11<<27),	o5_r3_a24		},
	{"LOADI",	(12<<27),	o5_r3_p8_i16	},	{"NOP",		(13<<27),	o5_p27			},
	{"IN",		(14<<27),	o5_r3_p16_t8	},	{"OUT",		(15<<27),	o5_r3_p16_t8	},
	{"ADD",		(16<<27),	o5_r3_r4_r4_p16	},	{"ADDI",	(17<<27),	o5_r3_p8_i16	},
	{"SUB",		(18<<27),	o5_r3_r4_r4_p16	},	{"SUBI",	(19<<27),	o5_r3_p8_i16	},
	{"MUL",		(20<<27),	o5_r3_r4_r4_p16	},	{"DIV",		(21<<27),	o5_r3_r4_r4_p16	},
	{"AND",		(22<<27),	o5_r3_r4_r4_p16	},	{"OR",		(23<<27),	o5_r3_r4_r4_p16	},
	{"NOR",		(24<<27),	o5_r3_r4_r4_p16	},	{"NOTB",	(25<<27),	o5_r3_r4_p20	},
	{"SAL",		(26<<27),	o5_r3_r4_r4_p16	},	{"SAR",		(27<<27),	o5_r3_r4_r4_p16	},
	{"EQU",		(28<<27),	o5_r3_r4_p20	},	{"LT",		(29<<27),	o5_r3_r4_p20	},
	{"LTE",		(30<<27),	o5_r3_r4_p20	},	{"NOTC",	(31<<27),	o5_p27			}
};

/* the hash table that stores the operator value and formate (key is the name) */
/* HASH_MASK is big enough to ensure there's no collide in the hash table */
Op op_hash[HASH_MASK];

error_t assembler_init(void){
	memset(op_hash,0,sizeof(Op)*HASH_MASK);
	/* initialize the op_hash table */
	for(int i=0; i<OP_NUM; i++)
		op_hash[hash_32(op_set[i].name, HASH_SEED) & HASH_MASK]	=	op_set[i];
	return no_error;
}

/* the main assembler function */
error_t assembler(const char *src, const char *dst, Error *error){
	error_t error_type;
	char *temp1	=	(char *)malloc(sizeof(char)*(strlen(dst)+3));
	char *temp2	=	(char *)malloc(sizeof(char)*(strlen(dst)+6));
	strcpy(temp1,dst);
	strcpy(temp2,dst);
	/* do the pretreatment stage 0 */
	if((error_type=assembler_pretreatment_stage0(src, strcat(temp1,"_s0") ))!=no_error)
		return error_type;
	/* do the pretreatment stage 1 */
	if((error_type=assembler_pretreatment_stage1(temp1, strcat(temp2,"_s1"), error))!=no_error)
		return error_type;
	/* do the replacement */
	if((error_type=assembler_replacement(temp2, dst, error))!=no_error)
		return error_type;
	return no_error;
}


/* get the Nth(start from 0) token on buffer */
/* buffer must end with \n\0 */
error_t __get_nth_token(const char *buffer, char *buffer_sub, char n){
	/* initialize the format string */
	char *fmt	=	(char *)malloc(sizeof(char)*4*(n+1));
	fmt[0]		=	'\0';
	for(int i=0; i<n; i++)
		strcat(fmt, "%*s ");
	strcat(fmt,"%s");
	/* get the nth token */
	if(sscanf(buffer,fmt,buffer_sub)!=1)
		return	error_token_not_get;
	return no_error;
}


