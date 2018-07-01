/**
 * @file ast_stmt.cpp
 * @brief implemetation of statements
 *
 * @date 2018-06-14
 * @author Sixu Hu
**/

#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "scope.h"
#include "errors.h"
#include "codegen.h"


// use default printer for this file
static AstPrinter &printer = AstPrinter::getDefaultPrinter();

Program::Program(List<Decl *> *d) {
    ASSERT(d != NULL);
    (decls = d)->SetParentAll(this);
}

void Program::check() {
    nodeScope = new Scope("[Program]");
    decls->DeclareAll(nodeScope);
    decls->checkAll();
}

void Program::generate() {
    // if the main function is found
    bool found = false;
    for (int i = 0; i < decls->NumElements(); i++) {
        Decl *d = decls->Nth(i);

        // try to find the main function
        if (!strcmp(d->getName(), "main") && d->isFnDecl()) {
            found = true;
            break;
        }
    }

    // return if the main function is not found
    if (!found) {
        ReportError::NoMainFound();
        return;
    }

    // print to file
    printer.print("Program");

    // generate all code using code generator cg (push instructions into cg.code)
    CodeGenerator *cg = new CodeGenerator();
    decls->generateAll(cg);

    // do final code generation using cg if no error occurred
    if (ReportError::NumErrors() == 0)
        cg->generateFinalCode();
}

StmtBlock::StmtBlock(List<VarDecl *> *d, List<Stmt *> *s) {
    ASSERT(d != NULL && s != NULL);
    (decls = d)->SetParentAll(this);
    (stmts = s)->SetParentAll(this);
}
void StmtBlock::check() {
    nodeScope = new Scope("[Statement-Block]");
    decls->DeclareAll(nodeScope);
    decls->checkAll();
    stmts->checkAll();
}
void StmtBlock::generate(CodeGenerator *cg) {
    printer.print("Statement-Block");
    decls->generateAll(cg, false);
    stmts->generateAll(cg);
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) {
    ASSERT(t != NULL && b != NULL);
    (test = t)->SetParent(this);
    (body = b)->SetParent(this);
}

void ConditionalStmt::check() {
    if (!test->checkAndComputeResultType()->isCompatibleWith(Type::boolType))
        ReportError::TestNotBoolean(test);
    body->check();
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) {
    ASSERT(i != NULL && t != NULL && s != NULL && b != NULL);
    (init = i)->SetParent(this);
    (step = s)->SetParent(this);
}
void ForStmt::generate(CodeGenerator *cg) {
    printer.print("For-Statement");
    printer.printHelper.push_back(1);

    init->generate(cg);
    char *topLoop = cg->NewLabel();
    afterLoopLabel = cg->NewLabel();
    cg->genLabel(topLoop);

    test->generate(cg);
    cg->genIfZ(test->result, afterLoopLabel);
    body->generate(cg);

    printer.printHelper.back() = 0;
    step->generate(cg);
    cg->genGoto(topLoop);
    cg->genLabel(afterLoopLabel);

    printer.printHelper.pop_back();
}

void WhileStmt::generate(CodeGenerator *cg) {
    printer.print("While-Statement");
    printer.printHelper.push_back(1);

    char *topLoop = cg->NewLabel();
    afterLoopLabel = cg->NewLabel();
    cg->genLabel(topLoop);
    test->generate(cg);
    cg->genIfZ(test->result, afterLoopLabel);

    printer.printHelper.back() = 0;
    body->generate(cg);
    cg->genGoto(topLoop);
    cg->genLabel(afterLoopLabel);

    printer.printHelper.pop_back();
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) {
    ASSERT(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody)
        elseBody->SetParent(this);
}
void IfStmt::check() {
    ConditionalStmt::check();
    if (elseBody)
        elseBody->check();
}
void IfStmt::generate(CodeGenerator *cg) {
    printer.print("If-Statement");
    printer.printHelper.push_back(1);

    test->generate(cg);
    char *afterElse, *elseL = cg->NewLabel();
    cg->genIfZ(test->result, elseL);

    if (!elseBody)
        printer.printHelper.back() = 0;
    body->generate(cg);

    if (elseBody) {
        afterElse = cg->NewLabel();
        cg->genGoto(afterElse);
    }
    cg->genLabel(elseL);
    if (elseBody) {
        elseBody->generate(cg);
        cg->genLabel(afterElse);
    }

    printer.printHelper.pop_back();
}


void BreakStmt::check() {
    if (!FindSpecificParent<LoopStmt>())
        ReportError::BreakOutsideLoop(this);
}
void BreakStmt::generate(CodeGenerator *cg) {
    printer.print("Break-Statement");
    LoopStmt *enclosingLoop = FindSpecificParent<LoopStmt>();
    cg->genGoto(enclosingLoop->GetLoopExitLabel());
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) {
    ASSERT(e != NULL);
    (expr = e)->SetParent(this);
}
void ReturnStmt::check() {
    Type *got = expr->checkAndComputeResultType();
    Type *expected =  FindSpecificParent<FnDecl>()->GetReturnType();
    if (!got->isCompatibleWith(expected))
        ReportError::ReturnMismatch(this, got, expected);
}
void ReturnStmt::generate(CodeGenerator *cg) {
    printer.print("Return statement");
    printer.printHelper.push_back(0);

    expr->generate(cg);
    cg->genReturn(expr->result);

    printer.printHelper.pop_back();
}

PrintStmt::PrintStmt(List<Expr *> *a) {
    ASSERT(a != NULL);
    (args = a)->SetParentAll(this);
}

void PrintStmt::check() {
    for (int i = 0; i < args->NumElements(); i++) {
        Type *t = args->Nth(i)->checkAndComputeResultType();
        if (t->isEquivalentTo(Type::errorType))
            continue;
        if (!(t->isEquivalentTo(Type::intType) || t->isEquivalentTo(Type::stringType) || t->isEquivalentTo(Type::boolType)))
            ReportError::PrintArgMismatch(args->Nth(i), i + 1, t);
    }
}

void PrintStmt::generate(CodeGenerator *cg) {
    printer.print("Print-Statement");
    printer.printHelper.push_back(1);

    for (int i = 0; i < args->NumElements(); i++) {
        if(i == args->NumElements() - 1)
            printer.printHelper.back() = 0;

        Expr *arg = args->Nth(i);
        Type *argType = arg->checkAndComputeResultType();
        arg->generate(cg);
        BuiltIn b = PrintInt;
        if (argType->isEquivalentTo(Type::stringType))
            b = PrintString;
        else if (argType->isEquivalentTo(Type::boolType))
            b = PrintBool;
        cg->genBuiltInCall(b, arg->result);
    }

    printer.printHelper.pop_back();
}


