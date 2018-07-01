/* File: errors.cc
 * ---------------
 * Implementation for error-reporting class.
 */

#include "errors.h"
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
using namespace std;

#include "scanner.h" // for getNthLine
#include "ast_type.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "ast_decl.h"

int ReportError::numErrors = 0;

void ReportError::UnderlineErrorInLine(const char *line, yyltype *pos) {
    if (!line) return;
    cerr << line << endl;
    for (int i = 1; i <= pos->last_column; i++)
        cerr << (i >= pos->first_column ? '^' : ' ');
    cerr << endl;
}



void ReportError::OutputError(yyltype *loc, string msg) {
    numErrors++;
    fflush(stdout); // make sure any buffered text has been output
    if (loc) {
        cerr << endl << "*** Error line " << loc->first_line << "." << endl;
        UnderlineErrorInLine(getNthLine(loc->first_line), loc);
    } else
        cerr << endl << "*** Error." << endl;
    cerr << "*** " << msg << endl << endl;
}


void ReportError::Formatted(yyltype *loc, const char *format, ...) {
    va_list args;
    char errbuf[2048];

    va_start(args, format);
    vsprintf(errbuf,format, args);
    va_end(args);
    OutputError(loc, errbuf);
}

void ReportError::UntermComment() {
    OutputError(NULL, "Input ends with unterminated comment");
}

void ReportError::InvalidDirective(int linenum) {
    yyltype ll = {0, linenum, 0, 0};
    OutputError(&ll, "Invalid # directive");
}

void ReportError::LongIdentifier(yyltype *loc, const char *ident) {
    stringstream s;
    s << "Identifier too long: \"" << ident << "\"";
    OutputError(loc, s.str());
}

void ReportError::UntermString(yyltype *loc, const char *str) {
    stringstream s;
    s << "Unterminated string constant: " << str;
    OutputError(loc, s.str());
}

void ReportError::UnrecogChar(yyltype *loc, char ch) {
    stringstream s;
    s << "Unrecognized char: '" << ch << "'" ;
    OutputError(loc, s.str());
}

void ReportError::DeclConflict(Decl *decl, Decl *prevDecl) {
    stringstream s;
    s << "Declaration of '" << decl << "' here conflicts with declaration on line "
      << prevDecl->GetLocation()->first_line;
    OutputError(decl->GetLocation(), s.str());
}

void ReportError::OverrideMismatch(Decl *fnDecl) {
    stringstream s;
    s << "Method '" << fnDecl << "' must match inherited type signature";
    OutputError(fnDecl->GetLocation(), s.str());
}

void ReportError::InterfaceNotImplemented(Decl *cd, Type *interfaceType) {
    stringstream s;
    s << "Class '" << cd << "' does not implement entire interface '" << interfaceType << "'";
    OutputError(interfaceType->GetLocation(), s.str());
}

void ReportError::IdentifierNotDeclared(Identifier *ident, Reason whyNeeded) {
    stringstream s;
    static const char *names[] =  {"type", "class", "interface", "variable", "function"};
    ASSERT(whyNeeded >= 0 && whyNeeded <= sizeof(names)/sizeof(names[0]));
    s << "No declaration found for "<< names[whyNeeded] << " '" << ident << "'";
    OutputError(ident->GetLocation(), s.str());
}

void ReportError::IncompatibleOperands(Operator *op, Type *lhs, Type *rhs) {
    stringstream s;
    s << "Incompatible operands: " << lhs << " " << op << " " << rhs;
    OutputError(op->GetLocation(), s.str());
}

void ReportError::IncompatibleOperand(Operator *op, Type *rhs) {
    stringstream s;
    s << "Incompatible operand: " << op << " " << rhs;
    OutputError(op->GetLocation(), s.str());
}

void ReportError::ThisOutsideClassScope(This *th) {
    OutputError(th->GetLocation(), "'this' is only valid within class scope");
}

void ReportError::BracketsOnNonArray(Expr *baseExpr) {
    OutputError(baseExpr->GetLocation(), "[] can only be applied to arrays");
}

void ReportError::SubscriptNotInteger(Expr *subscriptExpr) {
    OutputError(subscriptExpr->GetLocation(), "Array subscript must be an integer");
}

void ReportError::NewArraySizeNotInteger(Expr *sizeExpr) {
    OutputError(sizeExpr->GetLocation(), "Size for NewArray must be an integer");
}

void ReportError::NumArgsMismatch(Identifier *fnIdent, int numExpected, int numGiven) {
    stringstream s;
    s << "Function '"<< fnIdent << "' expects " << numExpected << " argument" << (numExpected==1?"":"s")
      << " but " << numGiven << " given";
    OutputError(fnIdent->GetLocation(), s.str());
}

void ReportError::ArgMismatch(Expr *arg, int argIndex, Type *given, Type *expected) {
  stringstream s;
  s << "Incompatible argument " << argIndex << ": " << given << " given, " << expected << " expected";
  OutputError(arg->GetLocation(), s.str());
}

void ReportError::ReturnMismatch(ReturnStmt *rStmt, Type *given, Type *expected) {
    stringstream s;
    s << "Incompatible return: " << given << " given, " << expected << " expected";
    OutputError(rStmt->GetLocation(), s.str());
}

void ReportError::FieldNotFoundInBase(Identifier *field, Type *base) {
    stringstream s;
    s << base << " has no such field '" << field <<"'";
    OutputError(field->GetLocation(), s.str());
}

void ReportError::InaccessibleField(Identifier *field, Type *base) {
    stringstream s;
    s  << base << " field '" << field << "' only accessible within class scope";
    OutputError(field->GetLocation(), s.str());
}

void ReportError::PrintArgMismatch(Expr *arg, int argIndex, Type *given) {
    stringstream s;
    s << "Incompatible argument " << argIndex << ": " << given
        << " given, int/bool/string expected";
    OutputError(arg->GetLocation(), s.str());
}

void ReportError::TestNotBoolean(Expr *expr) {
    OutputError(expr->GetLocation(), "Test expression must have boolean type");
}

void ReportError::BreakOutsideLoop(BreakStmt *bStmt) {
    OutputError(bStmt->GetLocation(), "break is only allowed inside a loop");
}

void ReportError::NoMainFound() {
    OutputError(NULL, "Linker: function 'main' not defined");
}

void yyerror(const char *msg) {
    ReportError::Formatted(&yylloc, "%s", msg);
}
