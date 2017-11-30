#include "assembler.h"
#include "assembler_pretreatment.h"
#include "assembler_replacement.h"
#include "common.h"
#include "assembler.h"
#include "hash.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* get the register name according to its name */
static error_t __get_reg_value(const char *name, uint32_t *value);

#define	__return_error(error_code)	\
{strcpy(error->error_line,buffer);\
error->error_pos	=	(char)(strstr(buffer,buffer_sub)-buffer);\
return	error_code;}

/* translate all the tokens to machine code*/
/* assuming the file already passed stage 0&1&2 */
error_t assembler_replacement(const char *src, const char *dst, Error *error){
	/* open necessary files */
	FILE *file_src	=	fopen(src, "r");
	FILE *file_dst	=	fopen(dst, "wb");
	if(!file_src || !file_dst)
		return error_open_file;
	/* the string used for analyze */
	char buffer[MAX_BUF_LEN];
	char buffer_sub[MAX_BUF_LEN];
	/* store the formatted machine code */
	uint32_t	code;
	Op			op_temp;
	uint32_t	reg_value;

	pc_addr++;
	/* write the size of the data */
	fwrite(&pc_addr, sizeof(uint32_t), 1, file_dst);
	/* dump the predefined data into file */
	for(uint32_t i=0; i<pc_addr; i++)
		fwrite(&data_stack[i], sizeof(int16_t), 1, file_dst);
	/* translate and write the commands */
	for(; fgets(buffer, MAX_BUF_LEN, file_src);){
        __get_nth_token(buffer, buffer_sub, 0);
        op_temp = op_hash[hash_32(buffer_sub,HASH_SEED) & HASH_MASK];
        code	=	op_temp.value;
        /* different code format */
        switch(op_temp.format){
			case o5_p27:
				break;
			case o5_p3_a24:
				__get_nth_token(buffer, buffer_sub, 1);
				code	|=	atoi(buffer_sub);
				break;
			case o5_r3_a24:
				__get_nth_token(buffer, buffer_sub, 1);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<24);

				__get_nth_token(buffer, buffer_sub, 2);
				code	|=	(uint32_t)atoi(buffer_sub);
				break;			case o5_r3_p16_t8:

				__get_nth_token(buffer, buffer_sub, 1);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<24);

				__get_nth_token(buffer, buffer_sub, 2);
				code	|=	(uint32_t)(atoi(buffer_sub)) & 0xFF;
				break;
			case o5_r3_p24:
				__get_nth_token(buffer, buffer_sub, 1);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<24);
				break;			case o5_r3_p8_i16:
				__get_nth_token(buffer, buffer_sub, 1);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<24);

				__get_nth_token(buffer, buffer_sub, 2);
				code	|=	(uint32_t)(atoi(buffer_sub)) & 0xFFFF;
				break;
			case o5_r3_r4_p20:
				__get_nth_token(buffer, buffer_sub, 1);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<24);

				__get_nth_token(buffer, buffer_sub, 2);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<20);
				break;
			case o5_r3_r4_r4_p16:
				__get_nth_token(buffer, buffer_sub, 1);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<24);

				__get_nth_token(buffer, buffer_sub, 2);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<20);

				__get_nth_token(buffer, buffer_sub, 3);
				if(__get_reg_value(buffer_sub, &reg_value)==error_gramma_reg_illegal)
					__return_error(error_gramma_reg_illegal);
				code	|=	(reg_value<<16);
				break;
        }
        fwrite(&code, sizeof(uint32_t), 1, file_dst);
	}
	fclose(file_src);
	fclose(file_dst);
	return no_error;
}

static error_t __get_reg_value(const char *name, uint32_t *value){
	if(strlen(name)!=1)
		return error_gramma_reg_illegal;
	else if('A' <=name[0] && name[0] <= 'G'){
		*value	=	(uint32_t)(name[0]-'A'+1);
		return no_error;
	}
	else if(name[0] == 'Z'){
		*value	=	0;
		return no_error;
	}
	else
		return error_gramma_reg_illegal;
}
