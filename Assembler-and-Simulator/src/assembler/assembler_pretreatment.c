#include "assembler.h"
#include "assembler_pretreatment.h"
#include "hash.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* the hash table of label address */
Addr *addr_table[HASH_MASK];
/* the temporary data stack */
int16_t data_stack[MAX_DATA_SPACE];
/* store the current reading address (metric: 2Byte) (for :LABEL) */
uint32_t pc_addr	=	0;

/* function that stores the label or declaration in hash table*/
static error_t __hash_store(const char *symbol, const uint32_t addr);

#define	__return_error(error_code)	\
{strcpy(error->error_line,buffer);\
error->error_pos	=	(char)(strstr(buffer,buffer_sub)-buffer);\
return	error_code;}

/* delete all the comments and empty lines int the source file */
error_t assembler_pretreatment_stage0(const char *src, const char *dst){
	/* opening necessary files */
	FILE *file_src	=	fopen(src, "r");
	FILE *file_dst	=	fopen(dst, "w");
	if(!file_src || !file_dst)
		return error_open_file;
	/* for analyze the line */
	char	buffer[MAX_BUF_LEN];
	char	buffer_sub[MAX_BUF_LEN];
	char	*temp;
	for(;fgets(buffer, MAX_BUF_LEN, file_src);){
		if((temp=strchr(buffer, '#'))!=NULL){
			*temp	=	'\n';
			*++temp	=	'\0';
		}
		if(__get_nth_token(buffer, buffer_sub, 0)==error_token_not_get);
		else
			fputs(buffer, file_dst);
	}
	fclose(file_src);
    fclose(file_dst);
	return	no_error;
}
/* substitute the BYTE, WORD declaration and :LABEL */
/* returning temp_dst(the temp file of pre-treatment */
error_t assembler_pretreatment_stage1(const char *src, const char *dst, Error *error){
	/* initialize the address table */
	for(int i=0; i<HASH_MASK; i++){
		addr_table[i]		=	malloc(sizeof(Addr));
		addr_table[i]->next	=	NULL;
	}
	/* initialize the temp data stack */
	memset(data_stack,0,sizeof(int16_t)*MAX_DATA_SPACE);

	/* open necessary files */
	FILE *file_src	=	fopen(src, "r");
	FILE *file_dst	=	fopen(dst, "w");
	if(!file_src || !file_dst)
		return error_open_file;

	/* store the reading line */
	char buffer[MAX_BUF_LEN];
	/* analyze the reading line */
	char buffer_sub[MAX_BUF_LEN];

	/* store the current data distribution address (metric: 2Byte) (relative addr, start from DS) */
	uint32_t data_addr	=	0;
	/* mark if this line has a label */
	char flag_label		=	0;

	/*** stage one, find the labels and declarations ***/
	for(; fgets(buffer, MAX_BUF_LEN, file_src)!=NULL; pc_addr+=2){
		flag_label	=	0;
		/* if is the empty line */
		if(__get_nth_token(buffer, buffer_sub, 0)==error_token_not_get){
			pc_addr-=2;
			continue;	/* next line */
		}
		/* switch(the first token) */
		/* if this line *has* a label */
		if(buffer_sub[strlen(buffer_sub)-1]==':'){
			/* store the label into hash table */
			buffer_sub[strlen(buffer_sub)-1]	=	'\0';
			/* if find duplicate symbol */
			if(__hash_store(buffer_sub, pc_addr)==error_gramma_duplicate_symbol)
				__return_error(error_gramma_duplicate_symbol);
			/* get the next token */
			if(__get_nth_token(buffer, buffer_sub, 1)==error_token_not_get){
				pc_addr-=2;
				continue;
			}
			flag_label	=	1;
		}
		/* if this line is a byte or world declaration */
		if(strcmp(buffer_sub,"BYTE")==0 || strstr(buffer,"WORD")){
			/* if there noting next */
			if(__get_nth_token(buffer, buffer_sub, flag_label+1)==error_token_not_get)
				__return_error(error_gramma_lack_symbol);

			/* distribute data space */
			int space	=	0;
			char *temp	=	strstr(buffer, buffer_sub);
			/* make sure the symbol is legal */
			if(isdigit(*temp))
				__return_error(error_gramma_declare_is_digit);
			int i=0;
			for(; isalnum(*temp) || *temp=='_'; temp++, i++)
				buffer_sub[i]	=	*temp;
			buffer_sub[i]='\0';
			/* if the symbol is a keyword */
			if(strcmp(op_hash[hash_32(buffer_sub, HASH_SEED) & HASH_MASK].name,buffer_sub)==0)
				__return_error(error_gramma_declare_is_keyword);
			/* store the name and continue */
			if(__hash_store(buffer_sub, data_addr)==error_gramma_duplicate_symbol)
				__return_error(error_gramma_duplicate_symbol);

			/* if it is not a set */
			if(__get_nth_token(temp, buffer_sub, 0)==error_token_not_get){
				/* mark that it's not a set */
				data_stack[data_addr]	=	1;
				space = 1;
				goto end;
			}
			temp	=	strstr(buffer, buffer_sub);
			/* otherwise */
			if(*temp=='['){
				temp++;
				for(; isspace(*temp); temp++);
				for(; isdigit(*temp); temp++)
					space	=	space*10 + (*temp) - '0';
				for(; isspace(*temp); temp++);
				if(*temp != ']')
					__return_error(error_gramma_declare_incomplete);
				temp++;
				/* mark that it's a set */
				data_stack[data_addr]	=	space;
			}

			/* if not set the value */
			if(__get_nth_token(temp, buffer_sub, 0)==error_token_not_get){
				goto end;
			}

			temp	=	strstr(buffer, buffer_sub);
			if(*temp=='='){
				temp++;
				for(; isspace(*temp); temp++);
				/* if it is a set */
				if(*temp=='{'){
					int counter	=	0;
					uint32_t data;
					char fmt[MAX_BUF_LEN]	=	"%d";
					for(;sscanf(temp,fmt,&data)==1 && counter<=space ;counter++){
						data_stack[data_addr+counter+1]	=	data;
						fmt[0]	=	'\0';
						for(int i=0; i<counter; i++)
							strcat(fmt,"%*d");
						strcat(fmt,"%d");
					}
					/* make the pointer be out of the set */
					for(; *temp!='}'; temp++);
					temp++;
				}
				/* if it is a string */
				else if(*temp=='"'){
					int counter	=	0;
					temp++;
					for(; *temp!='"' && counter <= space; temp++,counter++)
						data_stack[data_addr+counter+1]	=	*temp;
					temp++;
				}
				/* if it's a number */
				else if(isdigit(*temp)){
					space	=	1;
					sscanf(temp, "%hd", &data_stack[data_addr+1]);
					for(; isdigit(*temp); temp++);
				}
				else
					__return_error(error_gramma_too_less_argument);
			}
			else
				__return_error(error_gramma_too_much_argument);

			end:
			/* distribute space(relative addr ) */
			data_addr+=(space+1);
			if(data_addr>=MAX_DATA_SPACE)
				return	error_stack_overflow;

			/* if there's still token after the declaration and it's not comment */
			if(	__get_nth_token(temp, buffer_sub, 0)==no_error){
				__return_error(error_gramma_duplicate_command);
			}

			pc_addr	-=	2;
			continue;	/* next line */
		}

		/* if is a keyword (test if the number of token is right ) */
		if(strcmp(op_hash[hash_32(buffer_sub, HASH_SEED) & HASH_MASK].name,buffer_sub)==0){
			int num=0, counter=0;
			switch(op_hash[hash_32(buffer_sub, HASH_SEED) & HASH_MASK].format){
				case o5_p27:
					num	=	0;
					break;
				case o5_p3_a24:
				case o5_r3_p24:
					num	=	1;
					break;
				case o5_r3_a24:
				case o5_r3_p16_t8:
				case o5_r3_p8_i16:
				case o5_r3_r4_p20:
					num	=	2;
					break;
				case o5_r3_r4_r4_p16:
					num	=	3;
					break;
			}
			/* make sure the number of argument is correct */
			for(counter=0; counter<num; counter++)
				if(__get_nth_token(buffer, buffer_sub, flag_label+counter+1)==error_token_not_get)
					__return_error(error_gramma_too_less_argument);
			if(__get_nth_token(buffer, buffer_sub,flag_label+counter+1)==no_error)
				__return_error(error_gramma_too_much_argument);
			continue;
		}
		/* it is noting */
		else
			__return_error(error_gramma_unidentified);
	}
	/*** stage two, replace all the symbols with address, write into temp_dst ***/
	rewind(file_src);
	for(;fgets(buffer, MAX_BUF_LEN, file_src);){
		flag_label	=	0;
		/* ignore empty line */
		if(__get_nth_token(buffer, buffer_sub, 0)==error_token_not_get)
			continue;
		/* has label */
		if(buffer_sub[strlen(buffer_sub)-1]==':'){
			flag_label	=	1;
			/* get next token */
			if(__get_nth_token(buffer, buffer_sub, 1)==error_token_not_get)
				continue;
		}
		/* ignore declaration */
		if(strcmp(buffer_sub,"BYTE")==0 || strcmp(buffer_sub,"WORD")==0)
			continue;
		/* replace label and declarations */
		if(strcmp(op_hash[hash_32(buffer_sub,HASH_SEED) & HASH_MASK].name, buffer_sub)==0){
			Addr	*temp;
			int i=0;
			/* print the operator into temp_dst */
			i	=	flag_label?1:0;
			__get_nth_token(buffer, buffer_sub, i);
			fprintf(file_dst,"\t%s",buffer_sub);
			/* print the next argument */
			for(i=0; __get_nth_token(buffer, buffer_sub, i+flag_label+1)==no_error; i++){
				temp=addr_table[hash_32(buffer_sub,HASH_SEED) & HASH_MASK];
				/* try to find the symbol in hash (whatever it is) */
				for(temp=temp->next; temp!=NULL; temp=temp->next){
					if(strcmp(temp->symbol,buffer_sub)==0)
						break;
				}
				if(temp==NULL)
					fprintf(file_dst, "\t%s", buffer_sub);
				else
					fprintf(file_dst, "\t%u", temp->addr);
			}
			fprintf(file_dst, "\n");
		}
		else
			return error_how_could_it_be;
	}
	fclose(file_src);
	fclose(file_dst);
	Addr *temp, *temp_shadow;
	for(int i=0; i<HASH_MASK; i++){
		temp		=	addr_table[i];
		temp_shadow	=	temp;
		for(;temp!=NULL;){
			temp_shadow	=	temp;
			temp		=	temp->next;
			free(temp_shadow);
		}
	}
	return no_error;
}

static error_t __hash_store(const char *symbol, const uint32_t addr){
	Addr *addr_temp	=	addr_table[hash_32(symbol, HASH_SEED) & HASH_MASK];
	for(; addr_temp->next!=NULL; addr_temp=addr_temp->next)
		/* find the duplicate */
		if(!strcmp(addr_temp->next->symbol,symbol))
			return error_gramma_duplicate_symbol;
	/* store into the table */
	addr_temp->next		=	malloc(sizeof(Addr));
	addr_temp			=	addr_temp->next;
	strcpy(addr_temp->symbol,symbol);
	addr_temp->addr		=	addr;
	addr_temp->next		=	NULL;
	return no_error;
}

