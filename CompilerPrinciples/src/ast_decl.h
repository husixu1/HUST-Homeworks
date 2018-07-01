/**
 * @file ast_decl.h
 * @brief ast declarations node (variable, function, class, interface)
 *  class declaration
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#ifndef AST_DECL_H_
#define AST_DECL_H_

#include "ast.h"
#include "list.h"
#include "ast_stmt.h"

class Type;
class NamedType;
class Identifier;
class Stmt;
class FnDecl;
class InterfaceDecl;
class Location;

class Decl : public AstNode {
protected:
    Identifier *id;

public:
    Decl(Identifier *name);
    friend std::ostream &operator<<(std::ostream &out, Decl *d) {
        return out << d->id;
    }
    Identifier *GetId() {
        return id;
    }
    const char *getName() {
        return id->getName();
    }

    virtual bool ConflictsWithPrevious(Decl *prev);

    virtual bool isVarDecl() {
        return false;    // jdz: could use typeid/dynamic_cast for these
    }
    virtual bool isClassDecl() {
        return false;
    }
    virtual bool isInterfaceDecl() {
        return false;
    }
    virtual bool isFnDecl() {
        return false;
    }
    virtual bool isMethodDecl() {
        return false;
    }
    virtual bool isIvarDecl() {
        return false;
    }
    int offset;// used for methods and ivars only
    void SetOffset(int off) {
        offset = off;
    }
    int GetOffset() {
        return offset;
    }
};

class VarDecl : public Decl {
protected:
    Type *type;

public:
    VarDecl(Identifier *name, Type *type);
    void check();
    Type *GetDeclaredType() {
        return type;
    }
    bool isVarDecl() {
        return true;
    }
    bool isIvarDecl();
    Location *rtLoc;
    virtual bool isReference() {
        return false;
    }
    void generate(CodeGenerator *cg);
};

class ClassDecl : public Decl {
protected:
    List<Decl *> *members;
    NamedType *extends;
    List<NamedType *> *implements;
    Type *cType;
    List<InterfaceDecl *> *convImp;
    List<const char *> *vtable;
    int nextIvarOffset;

public:
    ClassDecl(Identifier *name, NamedType *extends,
            List<NamedType *> *implements, List<Decl *> *members);
    void check();
    bool isClassDecl() {
        return true;
    }
    Scope *PrepareScope();

    /// @brief check if this class can be converted to type
    /// @param type the type to convert to
    /// @return if the convert can be processed successfully
    bool isCompatibleWith(Type *type);

    bool Implements(Type *intf);
    Type *GetDeclaredType() {
        return cType;    //  used by "this"
    }
    const char *GetClassName() {
        return id->getName();
    }
    void generate(CodeGenerator *cg);
    void AddMethod(FnDecl *d, Decl *p);
    void AddIvar(VarDecl *d, Decl *p);
    void AddField(Decl *d);
    int GetClassSize() {
        return nextIvarOffset;
    }
};

class InterfaceDecl : public Decl {
protected:
    List<Decl *> *members;

public:
    InterfaceDecl(Identifier *name, List<Decl *> *members);
    void check();
    bool isInterfaceDecl() {
        return true;
    }
    Scope *PrepareScope();
    bool ClassMeetsObligation(Scope *s);
};

class FnDecl : public Decl {
protected:
    List<VarDecl *> *formals;
    Type *returnType;
    Stmt *body;

public:
    FnDecl(Identifier *name, Type *returnType, List<VarDecl *> *formals);
    void SetFunctionBody(Stmt *b);
    void check();
    bool isFnDecl() {
        return true;
    }
    bool isMethodDecl();
    bool ConflictsWithPrevious(Decl *prev);
    bool MatchesPrototype(FnDecl *other);
    Type *GetReturnType() {
        return returnType;
    }
    List<VarDecl *> *GetFormals() {
        return formals;
    }
    const char *GetFunctionLabel();
    void generate(CodeGenerator *cg);
};

#endif // AST_DECL_H_
