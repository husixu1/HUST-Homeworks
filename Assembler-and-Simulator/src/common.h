#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>

typedef char error_t;
#define no_error						0x00
#define error_open_file					0x01
/* grammar error*/
#define error_gramma_label_not_first	0x02
#define error_gramma_declare_incomplete	0x03
#define error_gramma_duplicate_symbol	0x04
#define error_token_not_get				0x05
#define error_gramma_declare_is_keyword	0x06
#define error_gramma_lack_symbol		0x07
#define error_gramma_duplicate_command	0x08
#define error_gramma_unidentified		0x09
#define error_gramma_too_less_argument	0x0A
#define error_gramma_too_much_argument	0x0B
#define error_gramma_declare_is_digit	0x0C
#define error_gramma_reg_illegal		0x0D

#define error_malloc					0x0E
#define error_stack_overflow			0x0F

#define error_how_could_it_be			0x10
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef short			int16_t;

#define MAX_ERROR_STRLEN	200
typedef struct _Error{
	/* the error line or error message */
	char error_line[MAX_ERROR_STRLEN];
	/* point to the position when grammar error occurs */
	char error_pos;
}Error;

/* the length of data stack*/
#define MAX_DATA_SPACE	(1<<23)

#endif // COMMON_H_INCLUDED
