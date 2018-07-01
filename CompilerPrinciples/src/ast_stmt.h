/**
 * @file ast_stmt.h
 * @brief statement node classes
 *
 * @details
 *  detailed introduction
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#ifndef AST_STMT_H_
#define AST_STMT_H_

#include "list.h"
#include "ast.h"

class Decl;
class VarDecl;
class Expr;

class Program : public AstNode {
protected:
    List<Decl *> *decls;

public:
    Program(List<Decl *> *declList);
    void check();
    void generate();
};

class Stmt : public AstNode {
public:
    Stmt() : AstNode() {}
    Stmt(yyltype loc) : AstNode(loc) {}
};

class StmtBlock : public Stmt {
protected:
    List<VarDecl *> *decls;
    List<Stmt *> *stmts;

public:
    StmtBlock(List<VarDecl *> *variableDeclarations, List<Stmt *> *statements);
    void check();

    void generate(CodeGenerator *cg);
};


class ConditionalStmt : public Stmt {
protected:
    Expr *test;
    Stmt *body;

public:
    ConditionalStmt(Expr *testExpr, Stmt *body);
    void check();
};

class LoopStmt : public ConditionalStmt {
protected:
    const char *afterLoopLabel;
public:
    LoopStmt(Expr *testExpr, Stmt *body)
        : ConditionalStmt(testExpr, body) {}
    const char *GetLoopExitLabel() {
        return afterLoopLabel;
    }
};

class ForStmt : public LoopStmt {
protected:
    Expr *init, *step;

public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);
    void generate(CodeGenerator *cg);
};

class WhileStmt : public LoopStmt {
public:
    WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}

    void generate(CodeGenerator *cg);
};

class IfStmt : public ConditionalStmt {
protected:
    Stmt *elseBody;

public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
    void check();

    void generate(CodeGenerator *cg);
};

class BreakStmt : public Stmt {
public:
    BreakStmt(yyltype loc) : Stmt(loc) {}
    void check();

    void generate(CodeGenerator *cg);
};

class ReturnStmt : public Stmt {
protected:
    Expr *expr;

public:
    ReturnStmt(yyltype loc, Expr *expr);
    void check();

    void generate(CodeGenerator *cg);
};

class PrintStmt : public Stmt {
protected:
    List<Expr *> *args;

public:
    PrintStmt(List<Expr *> *arguments);
    void check();

    void generate(CodeGenerator *cg);
};


#endif // AST_STMT_H_
