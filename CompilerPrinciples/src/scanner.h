/**
 * @file scanner.h
 * @brief scanner defines and function exports
 *
 * @details
 *  The header file for the generated lex.yy.c file,
 *  exports some functions
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#ifndef SCANNER_H_
#define SCANNER_H_


#include <stdio.h>

/// @brief Maximum length for identifiers
#define MAX_IDENTIFIER_LENGTH 255

// flex exports
extern char *yytext;
extern "C" {
    int yylex();
}

/// @brief start parsing from file
/// @param fp the file pointer
void yyrestart(FILE *fp);

/// @brief initialize the scanner
/// @note defined in scanner.l
void initScanner();

/// @brief get then nth error line
/// @param n the line number
/// @return the line content
const char *getNthLine(int n);

#endif // SCANNER_H_
