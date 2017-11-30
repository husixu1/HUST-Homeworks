#include "main.h"
#include "assembler.h"
#include "simulator.h"
#include "hash.h"
#include "common.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MASK 0x03FF

error_t p_error(error_t error_type);

int main(int argc, char *argv[]){
	/* do the pretreatment */
	if(argc==1){
		show_help();
		return 0;
	}
	if(argv[1][0]!='-' || strlen(argv[1])!=2){
		show_help();
		return 0;
	}

	char a[1000];
	char b[1000];

	switch(argv[1][1]){
		/* Assembler (compiler) */
		case 'c':
			/* read the src and dst file name*/
			if(argc!=3 && argc!=4){
				printf("USAGE : asm-sim -c <src> [dst]\n");
				break;
			}
			strcpy(a,argv[2]);
			/* if no dst name */
			if(argc==3){
				strcpy(b,a);
				strcat(b,".s");
			}
			/* if has dst name */
			else
				strcpy(b,argv[3]);

			assembler_init();

			Error error;
			/* do the compile job and output the error */
			if(p_error(assembler(a, b, &error)) != no_error)
				printf("%-*s\n",error.error_pos,"^~~~here\n");

			break;
		/* Simulator */
		case 's':
			if(argc!=3){
				printf("USAGE : asm-sim -s <file> \n");
				break;
			}
			strcpy(b,argv[2]);

			simulator_init();
			if(p_error(simulator(b, &error))!=no_error)
				printf("%-*s\n",error.error_pos,"^~~~here\n");
			break;
		/* help */
		case 'h':
			show_help();
			break;
	}

	return	0;
}


void show_help(void){
	printf(
	"      _/_/        _/_/_/_/  _/_/_/_/_/             _/_/_/_/  _/_/_/  _/_/_/_/_/\n"
	"     _/ _/       _/        _/  _/  _/             _/          _/    _/  _/  _/ \n"
	"    _/  _/      _/        _/  _/  _/             _/          _/    _/  _/  _/  \n"
	"   _/   _/     _/_/_/_/  _/  _/  _/   _/_/_ /   _/_/_/_/    _/    _/  _/  _/   \n"
	"  _/_/_/_/          _/  _/  _/  _/                   _/    _/    _/  _/  _/    \n"
	" _/     _/         _/  _/  _/  _/                   _/    _/    _/  _/  _/     \n"
	"_/      _/  _/_/_/_/  _/  _/  _/             _/_/_/_/  _/_/_/  _/  _/  _/      \n"
	"\n USAGE: asm-sim <COMMAND> [arg1] [arg2] \n"
	"\n COMMAND:\n"
	"\n -h : show this help doc\n"
	"\n -c : compile the assembler file to binary file \n"
	"      arg1 : the source asm file\n"
	"      arg2 : the dst file\n"
	"      if arg2 is not specified, the assembler will create arg1.s\n"
	"\n -s ： loade a binary file and simulate it\n"
	"      arg1 : the file to excute\n"
	"\nemail ： husixu1@hotmail.com\n"
	"github ： github.com/husixu1\n\n");

}

/* output the errors */
inline error_t p_error(error_t error_type){
	switch(error_type){
		/* common error */
		case no_error						:
			printf("no error occurred.\n");
			return error_type;
		case error_open_file				:
			printf("cannot open file.\n");
			break;
		/* grammar error*/
		case error_gramma_label_not_first	:
			printf("label must be the first of a command:\n");
			break;
		case error_gramma_declare_incomplete:
			printf("declaration incomplete:\n");
			break;
		case error_gramma_duplicate_symbol	:
			printf("duplicate symbol:\n");
			break;
		case error_token_not_get			:
			printf("incomplete command:\n");
			break;
		case error_gramma_declare_is_keyword:
			printf("declaration can not be a keyword:\n");
			break;
		case error_gramma_lack_symbol		:
			printf("incomplete command:\n");
			break;
		case error_gramma_duplicate_command	:
			printf("there should be no command after a declaration:\n");
			break;
		case error_gramma_unidentified		:
			printf("unidentified command:\n");
			break;
		case error_gramma_too_less_argument	:
			printf("too less argument:\n");
			break;
		case error_gramma_too_much_argument	:
			printf("too many argument：\n");
			break;
		case error_gramma_declare_is_digit	:
			printf("a declaration cannot start with a digit:\n");
			break;
		case error_gramma_reg_illegal		:
			printf("illegal register:\n");
			break;
		/* memory error */
		case error_malloc					:
			printf("no more spaces for mallocing!\n");
			break;
		case error_stack_overflow			:
			printf("stack overflow.\n");
			break;
		case error_how_could_it_be			:
			printf("unknown error, please contact the author.\n");
			break;
	}
	return error_type;
}
