 /**
  * @file scanner.l
  * @brief The lex file to generate scanner
  *
  * @details
  * Scan tokens from stdin.
  *
  * @date 2018-06-11
  * @author Sixu Hu
 **/

%{
#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "utility.h"
#include "errors.h"
#include "parser.h"
#include "list.h"
#include "printer.h"

#define TAB_SIZE 4

/// @brief current line number and column number
static int curLineNum, curColNum;

/// @brief save all lines for error reporting
List<const char*> savedLines;

/// @brief function called before every actions
static void commonAction();

// install the handle
#define YY_USER_ACTION commonAction();

/// @brief output the scanned token and its corresponding type
/// @param type the type of the token, translated to string directly
#define OUTPUT(type) LexPrinter::getDefaultPrinter().printToken(yytext, #type);

%}

 /* Starting states (stackable) */
%s NORMAL
%x COPY COMMENTS
%option stack

 /* Definitions */
integerHex          (0[Xx][0-9a-fA-F]+)
integer             ([0-9]+)
exponent            ([Ee][-+]?{integer})
double              ({integer}"."[0-9]*{exponent}?)
stringBegin         (\"[^"\n]*)
string              ({stringBegin}\")
identifier          ([a-zA-Z][a-zA-Z_0-9]*)
operator            ([-+/*%=.,;!<>()[\]{}])
commentBegin        ("/*")
commentEnd          ("*/")
commentSingleLine   ("//"[^\n]*)

 /* body ------------------------------------------------------------------- */

%%

 /* Keywords */
"void"          { OUTPUT(tokenVoid       ) return tokenVoid;        }
"int"           { OUTPUT(tokenInt        ) return tokenInt;         }
"double"        { OUTPUT(tokenDouble     ) return tokenDouble;      }
"bool"          { OUTPUT(tokenBool       ) return tokenBool;        }
"string"        { OUTPUT(tokenString     ) return tokenString;      }
"null"          { OUTPUT(tokenNull       ) return tokenNull;        }
"class"         { OUTPUT(tokenClass      ) return tokenClass;       }
"extends"       { OUTPUT(tokenExtends    ) return tokenExtends;     }
"this"          { OUTPUT(tokenThis       ) return tokenThis;        }
"interface"     { OUTPUT(tokenInterface  ) return tokenInterface;   }
"implements"    { OUTPUT(tokenImplements ) return tokenImplements;  }
"while"         { OUTPUT(tokenWhile      ) return tokenWhile;       }
"for"           { OUTPUT(tokenFor        ) return tokenFor;         }
"if"            { OUTPUT(tokenIf         ) return tokenIf;          }
"else"          { OUTPUT(tokenElse       ) return tokenElse;        }
"return"        { OUTPUT(tokenReturn     ) return tokenReturn;      }
"break"         { OUTPUT(tokenBreak      ) return tokenBreak;       }
"New"           { OUTPUT(tokenNew        ) return tokenNew;         }
"NewArray"      { OUTPUT(tokenNewArray   ) return tokenNewArray;    }
"Print"         { OUTPUT(tokenPrint      ) return tokenPrint;       }
"ReadInteger"   { OUTPUT(tokenReadInteger) return tokenReadInteger; }
"ReadLine"      { OUTPUT(tokenReadLine   ) return tokenReadLine;    }

 /* Operators */
"<="            { OUTPUT(tokenLessEqual   ) return tokenLessEqual;   }
">="            { OUTPUT(tokenGreaterEqual) return tokenGreaterEqual;}
"=="            { OUTPUT(tokenEqual       ) return tokenEqual;       }
"!="            { OUTPUT(tokenNotEqual    ) return tokenNotEqual;    }
"&&"            { OUTPUT(tokenAnd         ) return tokenAnd;         }
"||"            { OUTPUT(tokenOr          ) return tokenOr;          }
{operator}      { OUTPUT(tokenOperator    ) return yytext[0];        }
"[]"            { OUTPUT(tokenDims        ) return tokenDims;        }

 /* Constants */
"true"|"false"  {
    OUTPUT(tokenBoolConstant)
    yylval.boolConstant = (yytext[0] == 't');
    return tokenBoolConstant;
}
{integer}       {
    OUTPUT(tokenIntConstant)
    yylval.integerConstant = strtol(yytext, NULL, 10);
    return tokenIntConstant;
}
{integerHex}    {
    OUTPUT(tokenIntConstant)
    yylval.integerConstant = strtol(yytext, NULL, 16);
    return tokenIntConstant;
}
{double}        {
    OUTPUT(tokenDoubleConstant)
    yylval.doubleConstant = atof(yytext);
    return tokenDoubleConstant;
}
{string}        {
    OUTPUT(tokenStringConstant)
    yylval.stringConstant = strdup(yytext);
    return tokenStringConstant;
}
{stringBegin}   { ReportError::UntermString(&yylloc, yytext); }

 /* Identifiers */
{identifier}    {
    OUTPUT(tokenIdentifier)
    if (strlen(yytext) > MAX_IDENTIFIER_LENGTH)
        ReportError::LongIdentifier(&yylloc, yytext);
    strncpy(yylval.identifier, yytext, MAX_IDENTIFIER_LENGTH);
    yylval.identifier[MAX_IDENTIFIER_LENGTH] = '\0';
    return tokenIdentifier;
}

 /* comments */
{commentBegin}          { BEGIN(COMMENTS); }
<COMMENTS>{commentEnd}  { BEGIN(NORMAL);   }
<COMMENTS><<EOF>>       {
    ReportError::UntermComment();
    return 0;
}
<COMMENTS>.         { /* ignore everything in comments */ }
{commentSingleLine} { /* ignore sigleline comments */     }

 /* copy line into savedLines */
<COPY>.*        {
    char curLine[512];
    //strncpy(curLine, yytext, sizeof(curLine));
    savedLines.Append(strdup(yytext));
    curColNum = 1;
    yy_pop_state();
    // push back all tokens
    yyless(0);
}

 /* new line or tab */
<*>\n           {
    curLineNum++; curColNum = 1;
    if (YYSTATE == COPY)
        savedLines.Append("");
    else
        yy_push_state(COPY);
}
<*>[\t]         { curColNum += TAB_SIZE - curColNum%TAB_SIZE + 1; }

 /* ignore spaces */
[ ]+            { }

 /* end of the document */
<COPY><<EOF>>   { yy_pop_state(); }

 /* Default (error) */
. { ReportError::UnrecogChar(&yylloc, yytext[0]); }

%%


/* functions -------------------------------------------------------------- */

void initScanner() {
    BEGIN(NORMAL);
    curLineNum = 1;
    curColNum = 1;
    yy_flex_debug = false;
    yy_push_state(COPY); // copy first line at start
}

static void commonAction() {
   yylloc.first_line = curLineNum;
   yylloc.first_column = curColNum;
   yylloc.last_column = curColNum + yyleng - 1;
   // yyleng is the length of the token
   curColNum += yyleng;
}

const char *getNthLine(int num) {
   if (num <= 0 || num > savedLines.NumElements())
       return NULL;
   return savedLines.Nth(num-1);
}

