/**
 * @file parser.h
 * @brief parser definitions and function exports.
 *
 * @details
 *  The header file for the generated y.tab.c file,
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#ifndef PARSER_H_
#define PARSER_H_

// yylval types and definitions
#include "scanner.h"
#include "list.h"
#include "ast.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "ast_stmt.h"

// include yylval, but forbid to re-include it in y.tab.c
#ifndef YYBISON
#include "y.tab.h"
#endif

/// @brief initialize parser
void initParser();

/// @brief start to parse tokens
int yyparse();

#endif // PARSER_H_
