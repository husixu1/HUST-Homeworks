/**
 * @file main.cc
 * @brief the main file for the decaf compiler
 *
 * @details
 * pass the program via commandline.
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#include <string.h>
#include <stdio.h>
#include "utility.h"
#include "errors.h"
#include "parser.h"
#include "printer.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    // initialize printers
    LexPrinter::getDefaultPrinter().openFile("tokens.out");
    AstPrinter::getDefaultPrinter().openFile("ast.out");
    ScopePrinter::getDefaultPrinter().openFile("scope.out");
    ScopePrinter::getDefaultPrinter().setLevel(1);
    TacPrinter::getDefaultPrinter().openFile("tac.out");

    // scope guard should have a name otherwise it's treated as right-value
    ScopeGuard fileGuard([=](){
        LexPrinter::getDefaultPrinter().closeFile();
        AstPrinter::getDefaultPrinter().closeFile();
        ScopePrinter::getDefaultPrinter().printTail();
        ScopePrinter::getDefaultPrinter().closeFile();
        TacPrinter::getDefaultPrinter().closeFile();
    });

    parseArguments(argc, argv);
    initScanner();
    initParser();
    yyparse();

    return (ReportError::NumErrors() == 0 ? 0 : -1);
}
