/**
 * @file ast_decl.cc
 * @brief implementations of ast declaration classes
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "printer.h"
#include "scope.h"
#include "errors.h"
#include "scanner.h"
#include "codegen.h"

// use default printer for this file
static AstPrinter &astPrinter = AstPrinter::getDefaultPrinter();
static ScopePrinter &scopePrinter = ScopePrinter::getDefaultPrinter();

Decl::Decl(Identifier *n) : AstNode(*n->GetLocation()) {
    ASSERT(n != NULL);
    (id = n)->SetParent(this);

    offset = -555;
}

bool Decl::ConflictsWithPrevious(Decl *prev) {
    ReportError::DeclConflict(this, prev);
    return true;
}

VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    ASSERT(n != NULL && t != NULL);
    (type = t)->SetParent(this);
}

void VarDecl::check() {
    type->check();
    if (type->isError())
        type = Type::errorType;
}
bool VarDecl::isIvarDecl() {
    return dynamic_cast<ClassDecl *>(parent) != NULL;
}
void VarDecl::generate(CodeGenerator *cg) {
    astPrinter.print(string("Variable-Declaration : ") + string(getName()));
    if (dynamic_cast<Program *>(parent)) {
        //[husixu] if this variable is a global variable
        rtLoc = cg->genGlobalVariable(getName());
    } else if (!dynamic_cast<ClassDecl *>(parent)) {
        //[husixu] if this variable is not a member (thus a local variable)
        rtLoc = cg->genLocalVariable(getName());
    }
}

ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<NamedType *> *imp, List<Decl *> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    ASSERT(n != NULL && imp != NULL && m != NULL);
    extends = ex;
    if (extends)
        extends->SetParent(this);
    (implements = imp)->SetParentAll(this);
    (members = m)->SetParentAll(this);
    cType = new NamedType(n);
    cType->SetParent(this);
    convImp = NULL;
    vtable = new List<const char *>;
    nextIvarOffset = 4;
}

void ClassDecl::check() {
    // FIXME: add parent members
    if (extends) {
        ClassDecl *pd = dynamic_cast<ClassDecl *>(parent->FindDecl(extends->GetId()));
        List<Decl *> *pm = pd->members;
        for (int i = 0; i < pm->NumElements(); i++)
            ;//AddField(pm->Nth(i), NULL);
    }

    // check if all base class is declared
    if (extends && !extends->isClass()) {
        ReportError::IdentifierNotDeclared(extends->GetId(), lookingForClass);
        extends = NULL;
    }

    // check if all base interface is declared
    for (int i = 0; i < implements->NumElements(); i++) {
        NamedType *in = implements->Nth(i);
        if (!in->isInterface()) {
            ReportError::IdentifierNotDeclared(in->GetId(), lookingForInterface);
            implements->RemoveAt(i--);
        }
    }

    // check all members
    PrepareScope();
    members->checkAll();

    // check if all interfaces implemented
    for (int i = 0; i < convImp->NumElements(); i++) {
        if (!convImp->Nth(i)->ClassMeetsObligation(nodeScope))
            ReportError::InterfaceNotImplemented(this, implements->Nth(i));
    }
}

// This is not done very cleanly. I should sit down and sort this out. Right now
// I was using the copy-in strategy from the old compiler, but I think the link to
// parent may be the better way now.
Scope *ClassDecl::PrepareScope() {
    using namespace std;

    if (nodeScope)
        return nodeScope;

    nodeScope = new Scope(string("[Class] ") + string(id->getName()));
    if (extends) {
        ClassDecl *ext = dynamic_cast<ClassDecl *>(parent->FindDecl(extends->GetId()));
        if (ext)
            nodeScope->CopyFromScope(ext->PrepareScope(), this);
    }
    convImp = new List<InterfaceDecl *>;
    for (int i = 0; i < implements->NumElements(); i++) {
        NamedType *in = implements->Nth(i);
        InterfaceDecl *id = dynamic_cast<InterfaceDecl *>(in->FindDecl(in->GetId()));
        if (id) {
            nodeScope->CopyFromScope(id->PrepareScope(), NULL);
            convImp->Append(id);
        }
    }
    for (int i = 0; i < members->NumElements(); i++) {
        AddField(members->Nth(i));
    }
    members->DeclareAll(nodeScope);
    return nodeScope;
}


bool ClassDecl::isCompatibleWith(Type *other) {
    if (Implements(other))
        return true;
    if (cType->isEquivalentTo(other))
        return true;
    // recurse to base class
    return (extends && extends->isCompatibleWith(other));
}

bool ClassDecl::Implements(Type *other) {
    if (!other->isNamedType())
        return false;
    Decl *toMatch = dynamic_cast<NamedType *>(other)->GetDeclForType();
    if (!convImp)
        PrepareScope(); //jdz hack
    for (int i = 0; i < convImp->NumElements(); i++) {
        InterfaceDecl *id = convImp->Nth(i);
        if (id == toMatch)
            return true;
    }
    return false;
}
void ClassDecl::generate(CodeGenerator *cg) {
    astPrinter.print(string("Class-Declaration : ") + string(getName()));
    // generateAll does the push-back job automatically
    members->generateAll(cg);
    cg->genVTable(getName(), vtable);
}

void ClassDecl::AddField(Decl *decl) {
    Decl *prev = nodeScope->find(decl->GetId());
    if (decl->isVarDecl())
        AddIvar(dynamic_cast<VarDecl *>(decl), prev);
    else
        AddMethod(dynamic_cast<FnDecl *>(decl), prev);
}

// used when a new ivar/method is declared in a class to
// assign decl offset field
void ClassDecl::AddIvar(VarDecl *decl, Decl *prev) {
    decl->SetOffset(nextIvarOffset);
    nextIvarOffset += 4;  // all variables are 4 bytes for code gen
}

void ClassDecl::AddMethod(FnDecl *decl, Decl *inherited) {
    if (inherited) {
        int methodOffset = inherited->GetOffset();
        decl->SetOffset(methodOffset);
        if (vtable->NumElements() <= methodOffset) {
            while (vtable->NumElements() < methodOffset)
                vtable->Append(NULL);
            vtable->Append(decl->GetFunctionLabel());
        } else {
            vtable->RemoveAt(methodOffset);
            vtable->InsertAt(decl->GetFunctionLabel(), methodOffset);
        }
    } else {
        decl->SetOffset(vtable->NumElements());
        vtable->Append(decl->GetFunctionLabel());
    }
}


InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl *> *m) : Decl(n) {
    ASSERT(n != NULL && m != NULL);
    (members = m)->SetParentAll(this);
}

void InterfaceDecl::check() {
    PrepareScope();
    members->checkAll();
}

Scope *InterfaceDecl::PrepareScope() {
    using namespace std;

    if (nodeScope)
        return nodeScope;
    nodeScope = new Scope(string("[Interface] ") + string(id->getName()));
    members->DeclareAll(nodeScope);
    return nodeScope;
}
bool InterfaceDecl::ClassMeetsObligation(Scope *s) {
    for (int i = 0; i < members->NumElements(); i++) {
        FnDecl *m = dynamic_cast<FnDecl *>(members->Nth(i));
        FnDecl *found = dynamic_cast<FnDecl *>(s->find(m->GetId()));
        if (!found || !found->MatchesPrototype(m) || found->GetParent() == this)
            return false;
    }
    return true;
}

FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl *> *d) : Decl(n) {
    ASSERT(n != NULL && r != NULL && d != NULL);
    (returnType = r)->SetParent(this);
    (formals = d)->SetParentAll(this);
    body = NULL;
}

void FnDecl::SetFunctionBody(Stmt *b) {
    (body = b)->SetParent(this);
}

void FnDecl::check() {
    using namespace std;

    returnType->check();
    if (body) {
        nodeScope = new Scope(string("[Function] ") + string(id->getName()));
        formals->DeclareAll(nodeScope);
        formals->checkAll();
        body->check();
    }
}

bool FnDecl::ConflictsWithPrevious(Decl *prev) {
    // special case error for method override
    if (isMethodDecl() && prev->isMethodDecl() && parent != prev->GetParent()) {
        if (!MatchesPrototype(dynamic_cast<FnDecl *>(prev))) {
            ReportError::OverrideMismatch(this);
            return true;
        }
        return false;
    }
    ReportError::DeclConflict(this, prev);
    return true;
}

bool FnDecl::isMethodDecl() {
    return dynamic_cast<ClassDecl *>(parent) != NULL || dynamic_cast<InterfaceDecl *>(parent) != NULL;
}

bool FnDecl::MatchesPrototype(FnDecl *other) {
    if (!returnType->isEquivalentTo(other->returnType))
        return false;
    if (formals->NumElements() != other->formals->NumElements())
        return false;
    for (int i = 0; i < formals->NumElements(); i++)
        if (!formals->Nth(i)->GetDeclaredType()->isEquivalentTo(other->formals->Nth(i)->GetDeclaredType()))
            return false;
    return true;
}

void FnDecl::generate(CodeGenerator *cg) {
    astPrinter.print(string("Function-Declaration : ") + string(getName()));
    if (body) {
        astPrinter.printHelper.push_back(0);
        cg->genLabel(GetFunctionLabel());
        cg->genBeginFunc(this);
        body->generate(cg);
        cg->genEndFunc();
        astPrinter.printHelper.pop_back();
    }
}

const char *FnDecl::GetFunctionLabel() {
    ClassDecl *cd;
    if ((cd = dynamic_cast<ClassDecl *>(parent)) != NULL) { // if parent is a class, this is is a method
        char buffer[MAX_IDENTIFIER_LENGTH * 2 + 4];
        sprintf(buffer, "_%s.%s", cd->getName(), id->getName());
        return strdup(buffer);
    } else if (strcmp(id->getName(), "main")) {
        char buffer[strlen(id->getName()) + 2];
        sprintf(buffer, "_%s", id->getName());
        return strdup(buffer);
    } else
        return id->getName();
}


