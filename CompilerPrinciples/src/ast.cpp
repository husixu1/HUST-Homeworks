/* File: ast.cc
 * ------------
 */

#include "ast.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>
#include <stdio.h>
#include "errors.h"
#include "scope.h"
#include "printer.h"

AstNode::AstNode(yyltype loc) {
    location = new yyltype(loc);
    parent = NULL;
    nodeScope = NULL;
}

AstNode::AstNode() {
    location = NULL;
    parent = NULL;
    nodeScope = NULL;
}

Decl *AstNode::FindDecl(Identifier *idToFind, lookup l) {
    Decl *mine;

    if (!nodeScope)
        PrepareScope();

    // if belongs to a scope
    if (nodeScope && (mine = nodeScope->find(idToFind)))
        return mine;

    // find declare in the parent node recursively
    if (l == kDeep && parent)
        return parent->FindDecl(idToFind, l);
    return NULL;
}

Identifier::Identifier(yyltype loc, const char *n) : AstNode(loc) {
    name = strdup(n);
    cached = NULL;
}

Decl *Identifier::GetDeclRelativeToBase(Type *baseType) {
    if (!cached) {
        if (!baseType)
            // no field
            cached = FindDecl(this);
        else if (!baseType->isNamedType())
            // class and interface
            return NULL;
        else {
            // others
            Decl *cd = dynamic_cast<NamedType *>(baseType)->GetDeclForType();
            cached = (cd ? cd->FindDecl(this, kShallow) : NULL);
        }
    }
    return cached;
}

