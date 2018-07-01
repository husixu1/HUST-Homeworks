/**
 * @file ast_type.h
 * @brief type class node definitions
 *
 * @details
 * Type class is used to store type informations: base Type is for builtin type,
 * NamedType for class/interface, ArrayType is for arrays.
 *
 * @date 2018-06-12
 * @author Sixu Hu
**/

#ifndef AST_TYPE_H_
#define AST_TYPE_H_


#include "ast.h"
#include "list.h"
#include <iostream>


/// @brief type information node for built-in types
class Type : public AstNode {
protected:
    /// @brief name of the type
    char *typeName;

public :
    /// @brief type constants
    /// @note needs allocate space outside class so cannot be const
    static Type *intType, *doubleType, *boolType, *voidType,
           *nullType, *stringType, *errorType;

    Type(yyltype loc) : AstNode(loc) {}
    Type(const char *str);

    virtual void PrintToStream(std::ostream &out) {
        out << typeName;
    }
    friend std::ostream &operator<<(std::ostream &out, Type *t) {
        t->PrintToStream(out);
        return out;
    }
    virtual bool isEquivalentTo(Type *other) {
        return this == other;
    }
    virtual bool isArrayType() {
        return false;
    }
    virtual bool isNamedType() {
        return false;
    }
    virtual bool isCompatibleWith(Type *other);
    virtual bool isNumeric() {
        return this == Type::intType || this == Type::doubleType;
    }
    virtual bool isError() {
        return false;
    }
    virtual Type *LesserType(Type *other);

    /// @brief do nothing
    void generate(CodeGenerator *cg) {}
};

/// @brief type node class for class and interface
class NamedType : public Type {
protected:
    /// @brief the class/interface identifier
    Identifier *id;

    /// @brief the associated declaration (only one declaration for a NamedType)
    Decl *cachedDecl;

    /// @brief is it an error type
    bool _isError;

public:
    /// @brief constructor
    /// @param identifier the identifier for this type
    NamedType(Identifier *identifier);

    void PrintToStream(std::ostream &out) {
        out << id;
    }
    void check();
    Decl *GetDeclForType();
    bool isInterface();
    bool isClass();
    Identifier *GetId() {
        return id;
    }
    bool isEquivalentTo(Type *other);
    bool isNamedType() {
        return true;
    }
    bool isCompatibleWith(Type *other);
    bool isError() {
        return _isError;
    }
};

class ArrayType : public Type {
protected:
    /// @brief type of the element
    Type *elemType;

public:
    ArrayType(yyltype loc, Type *elemType);

    void PrintToStream(std::ostream &out) {
        out << elemType << "[]";
    }
    void check();
    bool isEquivalentTo(Type *other);
    bool isArrayType() {
        return true;
    }
    Type *GetArrayElemType() {
        return elemType;
    }
};


#endif // AST_TYPE_H_
