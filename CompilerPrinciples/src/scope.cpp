/**
 * @file scope.cpp
 * @brief scope implementations
 *
 * @date 2018-06-12
 * @author Sixu Hu
**/

#include "scope.h"
#include "ast_decl.h"
#include "list.h"
#include <iostream>
#include <string>

Scope::Scope(const std::string &info) {
    ScopePrinter::getDefaultPrinter().printScopeStart(info);

    static Scope *init = (this);
    table = new Hashtable<Decl *>;
}

Decl *Scope::find(Identifier *id) {
    return table->find(id->getName());
}

bool Scope::Declare(Decl *decl) {
    using namespace std;

    string type;
    // print scope info
    type = (decl->isClassDecl()     ? "(Class)"     : type);
    type = (decl->isInterfaceDecl() ? "(Interface)" : type);
    type = (decl->isFnDecl()        ? "(function)"  : type);
    type = (decl->isMethodDecl()    ? "(Method)"    : type);
    type = (decl->isVarDecl()       ? "(Variable)"  : type);
    type = (decl->isIvarDecl()      ? "(Member)"    : type);

    ScopePrinter::getDefaultPrinter().print(type + " " + string(decl->getName()));

    Decl *prev = table->find(decl->getName());
    debug("scope", "Line %d declaring %s (prev? %p)\n", decl->GetLocation()->first_line, decl->getName(), prev);
    if (prev && decl->ConflictsWithPrevious(prev)) // throw away second, keep first
        return false;
    table->insert(decl->getName(), decl);
    return true;
}

void Scope::CopyFromScope(Scope *other, ClassDecl *addTo) {
    Iterator<Decl *> iter = other->table->GetIterator();
    Decl *decl;
    while ((decl = iter.next()) != NULL) {
        table->insert(decl->getName(), decl);
        if (addTo)
            addTo->AddField(decl);
    }
}
