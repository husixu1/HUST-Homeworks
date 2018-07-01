/* File: ast_type.cc
 * -----------------
 * Implementation of type node classes.
 */
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>

#include "errors.h"
#include "codegen.h"

/* Class constants
 * ---------------
 * These are public constants for the built-in base types (int, double, etc.)
 * They can be accessed with the syntax Type::intType. This allows you to
 * directly access them and share the built-in types where needed rather that
 * creates lots of copies.
 */

Type *Type::intType    = new Type("int");
Type *Type::doubleType = new Type("double");
Type *Type::voidType   = new Type("void");
Type *Type::boolType   = new Type("bool");
Type *Type::nullType   = new Type("null");
Type *Type::stringType = new Type("string");
Type *Type::errorType  = new Type("error");

Type::Type(const char *n) {
    ASSERT(n);
    typeName = strdup(n);
}

Type *Type::LesserType(Type *other) {
    if (this == Type::errorType || other == Type::errorType)
        return Type::errorType;
    if (other == NULL)
        return this;
    if (isCompatibleWith(other))
        return other;
    if (other->isCompatibleWith(this))
        return this;
    return NULL;
}


bool Type::isCompatibleWith(Type *other) {
    if (this == errorType || other == errorType)
        return true;
    if ((this == nullType) && (other->isNamedType()))
        return true;
    return isEquivalentTo(other);
}

NamedType::NamedType(Identifier *i) : Type(*i->GetLocation()) {
    ASSERT(i != NULL);
    (id = i)->SetParent(this);
    cachedDecl = NULL;
    _isError = false;
}

void NamedType::check() {
    if (!GetDeclForType()) {
        _isError = true;
        ReportError::IdentifierNotDeclared(id, lookingForType);
    }
}
Decl *NamedType::GetDeclForType() {
    if (!cachedDecl && !_isError) {
        Decl *declForName = FindDecl(id);
        if (declForName && (declForName->isClassDecl() || declForName->isInterfaceDecl()))
            cachedDecl = declForName;
    }
    return cachedDecl;
}

bool NamedType::isInterface() {
    Decl *d = GetDeclForType();
    return (d && d->isInterfaceDecl());
}

bool NamedType::isClass() {
    Decl *d = GetDeclForType();
    return (d && d->isClassDecl());
}

bool NamedType::isEquivalentTo(Type *other) {
    NamedType *ot = dynamic_cast<NamedType *>(other);
    return ot && strcmp(id->getName(), ot->id->getName()) == 0;
}

bool NamedType::isCompatibleWith(Type *other) {
    // if this type is the same with others
    if (isEquivalentTo(other))
        return true;

    // if this type or other type is error type
    if (other == errorType || _isError)
        return true;

    // try casting type to class type
    NamedType *ot = dynamic_cast<NamedType *>(other);
    if (!ot)
        return false;
    if (ot->_isError)
        return true;

    // get the class declaration of this type and compare it with other
    ClassDecl *cd = dynamic_cast<ClassDecl *>(GetDeclForType());
    return cd && cd->isCompatibleWith(other);
}


ArrayType::ArrayType(yyltype loc, Type *et) : Type(loc) {
    ASSERT(et != NULL);
    (elemType = et)->SetParent(this);
}

void ArrayType::check() {
    elemType->check();
}

bool ArrayType::isEquivalentTo(Type *other) {
    ArrayType *o = dynamic_cast<ArrayType *>(other);
    return (o && elemType->isEquivalentTo(o->elemType));
}

