#ifndef ASSEMBLER_REPLACEMENT_H_INCLUDED
#define ASSEMBLER_REPLACEMENT_H_INCLUDED

#include "common.h"

/* translate all the tokens to machine code*/
/* assuming the file already passed stage 0&1&2 */
/* file structure:	|data_len(uint32_t)|data[data_len]|commands|EOF| */
error_t assembler_replacement(const char *src, const char *dst, Error *error);


#endif // ASSEMBLER_REPLACEMENT_H_INCLUDED
