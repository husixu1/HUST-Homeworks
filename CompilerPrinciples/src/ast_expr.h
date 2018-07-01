/**
 * @file ast_expr.h
 * @brief ast expression node(call, new,...) class declaration
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#ifndef AST_EXPR_H_
#define AST_EXPR_H_

#include "ast.h"
#include "ast_stmt.h"
#include "list.h"

class NamedType; // for new
class Type;      // for NewArray
class ClassDecl; // for This
class Location;


class Expr : public Stmt {
public:
    Expr(yyltype loc) : Stmt(loc) {
        result = NULL;
    }
    Expr() : Stmt() {
        result = NULL;
    }
    void check() {
        checkAndComputeResultType();
    }
    virtual Type *checkAndComputeResultType() = 0;
    Location *result;
    Location *GetResult() {
        return result;
    }
};

/* This node type is used for those places where an expression is optional.
 * We could use a NULL pointer, but then it adds a lot of checking for
 * NULL. By using a valid, but no-op, node, we save that trouble */
class EmptyExpr : public Expr {
public:
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg) {
        result = NULL;
    }
};

class IntConstant : public Expr {
protected:
    int value;

public:
    IntConstant(yyltype loc, int val);
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class DoubleConstant : public Expr {
protected:
    double value;

public:
    DoubleConstant(yyltype loc, double val);
    Type *checkAndComputeResultType();
};

class BoolConstant : public Expr {
protected:
    bool value;

public:
    BoolConstant(yyltype loc, bool val);
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class StringConstant : public Expr {
protected:
    char *value;

public:
    StringConstant(yyltype loc, const char *val);
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class NullConstant: public Expr {
public:
    NullConstant(yyltype loc) : Expr(loc) {}
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class Operator : public AstNode {
protected:
    char tokenString[4];

public:
    Operator(yyltype loc, const char *tok);
    friend std::ostream &operator<<(std::ostream &out, Operator *o) {
        return out << o->tokenString;
    }
    const char *str() {
        return tokenString;
    }
};

class CompoundExpr : public Expr {
protected:
    Operator *op;
    Expr *left, *right; // left will be NULL if unary

public:
    CompoundExpr(Expr *lhs, Operator *op, Expr *rhs); // for binary
    CompoundExpr(Operator *op, Expr *rhs);             // for unary
    void ReportErrorForIncompatibleOperands(Type *lhs, Type *rhs);
    bool EitherOperandIsError(Type *lhs, Type *rhs);
    void generate(CodeGenerator *cg);
};

class ArithmeticExpr : public CompoundExpr {
public:
    ArithmeticExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs, op, rhs) {}
    ArithmeticExpr(Operator *op, Expr *rhs) : CompoundExpr(op, rhs) {}
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class RelationalExpr : public CompoundExpr {
public:
    RelationalExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs, op, rhs) {}
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class EqualityExpr : public CompoundExpr {
public:
    EqualityExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs, op, rhs) {}
    const char *GetPrintNameForAstNode() {
        return "EqualityExpr";
    }
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class LogicalExpr : public CompoundExpr {
public:
    LogicalExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs, op, rhs) {}
    LogicalExpr(Operator *op, Expr *rhs) : CompoundExpr(op, rhs) {}
    const char *GetPrintNameForAstNode() {
        return "LogicalExpr";
    }
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class AssignExpr : public CompoundExpr {
public:
    AssignExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs, op, rhs) {}
    const char *GetPrintNameForAstNode() {
        return "AssignExpr";
    }
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

/// @brief lvalue node class
/// @note this is not a terminative node, FieldAccess/ArrayAccess is
class LValue : public Expr {
public:
    LValue(yyltype loc) : Expr(loc) {}
    void generate(CodeGenerator *cg);
    virtual void generateWithoutDereference(CodeGenerator *cg) {}
};

class This : public Expr {
protected:
    ClassDecl *enclosingClass;

public:
    This(yyltype loc) : Expr(loc), enclosingClass(NULL)  {}
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class ArrayAccess : public LValue {
protected:
    Expr *base, *subscript;

public:
    ArrayAccess(yyltype loc, Expr *base, Expr *subscript);
    Type *checkAndComputeResultType();
    void generateWithoutDereference(CodeGenerator *cg);
};

/* Note that field access is used both for qualified names
 * base.field and just field without qualification. We don't
 * know for sure whether there is an implicit "this." in
 * front until later on, so we use one node type for either
 * and sort it out later. */
class FieldAccess : public LValue {
protected:
    /// @brief the base of the field
    /// @note NULL for no base (local/global var)
    Expr *base;

    /// @brief the filed to access (var/member id)
    Identifier *field;

public:
    /// @brief constructor
    /// @param base null for no base
    /// @param field the field identifier
    FieldAccess(Expr *base, Identifier *field);
    Type *checkAndComputeResultType();
    void generateWithoutDereference(CodeGenerator *cg);
};

/* Like field access, call is used both for qualified base.field()
 * and unqualified field().  We won't figure out until later
 * whether we need implicit "this." so we use one node type for either
 * and sort it out later. */
class Call : public Expr {
protected:
    /// @brief the base of the field
    /// @note NULL for no base
    Expr *base;
    Identifier *field;
    List<Expr *> *actuals;

public:
    Call(yyltype loc, Expr *base, Identifier *field, List<Expr *> *args);
    Decl *GetFnDecl();
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class NewExpr : public Expr {
protected:
    NamedType *cType;

public:
    NewExpr(yyltype loc, NamedType *clsType);
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class NewArrayExpr : public Expr {
protected:
    Expr *size;
    Type *elemType;

public:
    NewArrayExpr(yyltype loc, Expr *sizeExpr, Type *elemType);
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class ReadIntegerExpr : public Expr {
public:
    ReadIntegerExpr(yyltype loc) : Expr(loc) {}
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};

class ReadLineExpr : public Expr {
public:
    ReadLineExpr(yyltype loc) : Expr (loc) {}
    Type *checkAndComputeResultType();
    void generate(CodeGenerator *cg);
};


#endif // AST_EXPR_H_
