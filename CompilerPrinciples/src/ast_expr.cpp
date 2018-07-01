/**
 * @file ast_expr.cc
 * @brief ast expression node class implementation
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>
#include <sstream>

#include "errors.h"
#include "codegen.h"

// use default printer for this file
static AstPrinter &printer = AstPrinter::getDefaultPrinter();

Type *EmptyExpr::checkAndComputeResultType() {
    return Type::voidType;
}

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}
Type *IntConstant::checkAndComputeResultType() {
    return Type::intType;
}
void IntConstant::generate(CodeGenerator *cg) {
    printer.print(string("Int-Constant : ") + std::to_string(value));
    result = cg->genLoadConstant(value);
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}
Type *DoubleConstant::checkAndComputeResultType() {
    return Type::doubleType;
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}
Type *BoolConstant::checkAndComputeResultType() {
    return Type::boolType;
}
void BoolConstant::generate(CodeGenerator *cg) {
    printer.print(string("Bool-Constant : ") + (value ? string("true") : string("false")));
    result = cg->genLoadConstant(value);
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    ASSERT(val != NULL);
    value = strdup(val);
}
Type *StringConstant::checkAndComputeResultType() {
    return Type::stringType;
}
void StringConstant::generate(CodeGenerator *cg) {
    printer.print(string("String-Constant : ") + string(value));
    result = cg->genLoadConstant(value);
}
Type *NullConstant::checkAndComputeResultType() {
    return Type::nullType;
}
void NullConstant::generate(CodeGenerator *cg) {
    printer.print("Null-Constant");
    result = cg->genLoadConstant(0);
}

Operator::Operator(yyltype loc, const char *tok) : AstNode(loc) {
    ASSERT(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}
CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r)
    : Expr(concatLocation(l->GetLocation(), r->GetLocation())) {
    ASSERT(l != NULL && o != NULL && r != NULL);
    (op = o)->SetParent(this);
    (left = l)->SetParent(this);
    (right = r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r)
    : Expr(concatLocation(o->GetLocation(), r->GetLocation())) {
    ASSERT(o != NULL && r != NULL);
    left = NULL;
    (op = o)->SetParent(this);
    (right = r)->SetParent(this);
}
void CompoundExpr::ReportErrorForIncompatibleOperands(Type *lhs, Type *rhs) {
    if (!lhs) { //unary op
        ReportError::IncompatibleOperand(op, rhs);
    } else { // binary op
        ReportError::IncompatibleOperands(op, lhs, rhs);
    }
}

bool CompoundExpr::EitherOperandIsError(Type *lhs, Type *rhs) {
    return (lhs && lhs == Type::errorType) || rhs == Type::errorType;
}


void CompoundExpr::generate(CodeGenerator *cg) {
    printer.print("Compound-Expression");
    printer.printHelper.push_back(1);
    ASSERT(left);
    left->generate(cg);

    printer.printHelper.back() = 0;
    right->generate(cg);
    result = cg->genBinaryOp(op->str(), left->result, right->result);

    printer.printHelper.pop_back();
}

Type *GetResultType(Type *lhs, Type *rhs) {
    Type *lesser = rhs;
    if (lhs)
        lesser = lesser->LesserType(lhs);
    if (!lesser || !lesser->isNumeric())
        return Type::errorType;
    return lesser;
}

Type *ArithmeticExpr::checkAndComputeResultType() {
    Type *lType = left ? left->checkAndComputeResultType() : NULL, *rType = right->checkAndComputeResultType();
    if (EitherOperandIsError(lType, rType))
        return Type::errorType;
    if (GetResultType(lType, rType) == Type::errorType)
        ReportErrorForIncompatibleOperands(lType, rType);
    return GetResultType(lType, rType);
}
void ArithmeticExpr::generate(CodeGenerator *cg) {
    printer.print("Arithmetic-Expression");
    printer.printHelper.push_back(0);
    if (left){
        CompoundExpr::generate(cg);
    } else {
        right->generate(cg);
        Location *zero = cg->genLoadConstant(0);
        result = cg->genBinaryOp("-", zero, right->result);
    }
    printer.printHelper.pop_back();
}

Type *RelationalExpr::checkAndComputeResultType() {
    Type *lhs = left->checkAndComputeResultType(), *rhs = right->checkAndComputeResultType();
    if (EitherOperandIsError(lhs, rhs))
        return Type::boolType;
    if (!lhs->isEquivalentTo(rhs) || !lhs->isNumeric())
        ReportErrorForIncompatibleOperands(lhs, rhs);
    return Type::boolType;
}
void RelationalExpr::generate(CodeGenerator *cg) {
    printer.print("Relation-Expression");
    printer.printHelper.push_back(1);
    left->generate(cg);

    printer.printHelper.back() = 0;
    right->generate(cg);

    printer.printHelper.pop_back();

    if (!strcmp(op->str(), "<")) {
        result = cg->genBinaryOp("<", left->result, right->result);
    } else if (!strcmp(op->str(), ">")) {
        result = cg->genBinaryOp("<", right->result, left->result);
    } else if (!strcmp(op->str(), "<=")) {
        Location *less = cg->genBinaryOp("<", left->result, right->result);
        Location *eq  = cg->genBinaryOp("==", left->result, right->result);
        result =  cg->genBinaryOp("||", less, eq);
    } else if (!strcmp(op->str(), ">=")) {
        Location *less = cg->genBinaryOp("<", right->result, left->result);
        Location *eq  = cg->genBinaryOp("==", right->result, left->result);
        result =  cg->genBinaryOp("||", less, eq);
    }
}

Type *EqualityExpr::checkAndComputeResultType() {
    Type *lhs = left->checkAndComputeResultType(), *rhs = right->checkAndComputeResultType();
    if (!lhs->isCompatibleWith(rhs) && !rhs->isCompatibleWith(lhs))
        ReportErrorForIncompatibleOperands(lhs, rhs);
    return Type::boolType;
}
void EqualityExpr::generate(CodeGenerator *cg) {
    printer.print("Relation-Expression");
    printer.printHelper.push_back(1);
    left->generate(cg);

    printer.printHelper.back() = 0;
    right->generate(cg);

    printer.printHelper.pop_back();

    if (left->checkAndComputeResultType() == Type::stringType)
        result = cg->genBuiltInCall(StringEqual, left->result, right->result);
    else
        result = cg->genBinaryOp("==", left->result, right->result);
    if (!strcmp(op->str(), "!=")) {
        Location *zero = cg->genLoadConstant(0);
        result = cg->genBinaryOp("==", result, zero);
    }
}

Type *LogicalExpr::checkAndComputeResultType() {
    Type *lhs = left ? left->checkAndComputeResultType() : NULL, *rhs = right->checkAndComputeResultType();
    if ((lhs && !lhs->isCompatibleWith(Type::boolType)) ||
            (!rhs->isCompatibleWith(Type::boolType)))
        ReportErrorForIncompatibleOperands(lhs, rhs);
    return Type::boolType;
}
void LogicalExpr::generate(CodeGenerator *cg) {
    printer.print("Logical-Expression");
    printer.printHelper.push_back(0);
    if (left)
        CompoundExpr::generate(cg);
    else {
        right->generate(cg);
        Location *zero = cg->genLoadConstant(0);
        result = cg->genBinaryOp("==", right->result, zero);
    }
    printer.printHelper.pop_back();
}

Type *AssignExpr::checkAndComputeResultType() {
    Type *lhs = left->checkAndComputeResultType(), *rhs = right->checkAndComputeResultType();
    if (!rhs->isCompatibleWith(lhs)) {
        ReportErrorForIncompatibleOperands(lhs, rhs);
        return Type::errorType;
    }
    return lhs;
}
void AssignExpr::generate(CodeGenerator *cg) {
    printer.print("Assign-Expression");

    printer.printHelper.push_back(1);
    dynamic_cast<LValue *>(left)->generateWithoutDereference(cg); //sad, but if want to be compound....

    printer.printHelper.back() = 0;
    right->generate(cg);

    if (left->result->isReference()) {
        cg->genStore(left->result->GetBase(), right->result, left->result->GetRefOffset());
    } else
        cg->genAssign(left->result, right->result);
    result = left->result;
    printer.printHelper.pop_back();
}
void LValue::generate(CodeGenerator *cg) {
    printer.print("L-Value");
    printer.printHelper.push_back(0);
    generateWithoutDereference(cg);
    if (result->isReference())
        result = cg->genLoad(result->GetBase(), result->GetRefOffset());
    printer.printHelper.pop_back();
}
Type *This::checkAndComputeResultType() {
    if (!enclosingClass)
        enclosingClass = FindSpecificParent<ClassDecl>();
    if (!enclosingClass)
        ReportError::ThisOutsideClassScope(this);
    if (!enclosingClass)
        return Type::errorType;
    return enclosingClass->GetDeclaredType();
}

void This::generate(CodeGenerator *cg) {
    printer.print("This-Pointer");
    if (!result)
        result = CodeGenerator::ThisPtr;
}



ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base = b)->SetParent(this);
    (subscript = s)->SetParent(this);
}
Type *ArrayAccess::checkAndComputeResultType() {
    Type *baseT = base->checkAndComputeResultType();
    if ((baseT != Type::errorType) && !baseT->isArrayType())
        ReportError::BracketsOnNonArray(base);
    if (!subscript->checkAndComputeResultType()->isCompatibleWith(Type::intType))
        ReportError::SubscriptNotInteger(subscript);
    return baseT->isArrayType() ? dynamic_cast<ArrayType *>(baseT)->GetArrayElemType() : Type::errorType;
}

void ArrayAccess::generateWithoutDereference(CodeGenerator *cg)  {
    printer.print("Array-Acces");
    printer.printHelper.push_back(1);
    base->generate(cg);

    printer.printHelper.back() = 0;
    subscript->generate(cg);

    printer.printHelper.pop_back();
    result = cg->genSubscript(base->result, subscript->result);
}

FieldAccess::FieldAccess(Expr *b, Identifier *f)
    : LValue(b ? concatLocation(b->GetLocation(), f->GetLocation()) : * f->GetLocation()) {
    ASSERT(f != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base)
        base->SetParent(this);
    (field = f)->SetParent(this);
}


Type *FieldAccess::checkAndComputeResultType() {
    // the base of the field
    Type *baseType = base ? base->checkAndComputeResultType() : NULL;
    // find the field(var/member)'s declaration in base's scope
    Decl *ivar = field->GetDeclRelativeToBase(baseType);

    // add implicit "this" if there's no base
    if (ivar && ivar->isIvarDecl() && !base) {
        base = new This(*field->GetLocation());
        base->SetParent(this);
        baseType = base->checkAndComputeResultType();
    }

    // if there's base
    if (base) {
        if (baseType == Type::errorType)
            return Type::errorType;
        else if (!ivar || !ivar->isVarDecl()) {
            ReportError::FieldNotFoundInBase(field, baseType);
            return Type::errorType;
        } else {
            ClassDecl *enclosingClass = FindSpecificParent<ClassDecl>(); // check cur scope for compatibility
            Type *withinClass = (enclosingClass ? enclosingClass->GetDeclaredType() : NULL);
            if (ivar && (!withinClass || !withinClass->isCompatibleWith(baseType))) {
                ReportError::InaccessibleField(field, baseType);
                return Type::errorType;
            }
        }
    } else if (!ivar || !ivar->isVarDecl()) {
        // report error when field is not found
        ReportError::IdentifierNotDeclared(field, lookingForVariable);
        return Type::errorType;
    }
    return ivar ? (dynamic_cast<VarDecl *>(ivar))->GetDeclaredType() : Type::errorType;
}

void FieldAccess::generateWithoutDereference(CodeGenerator *cg) {
    printer.print(string("Field-Acces : ") + string(field->getName()));
    checkAndComputeResultType(); // need to ensure check called to get base set
    Decl *fd = field->GetDeclRelativeToBase(base ? base->checkAndComputeResultType() : NULL);
    if (base) {
        printer.printHelper.push_back(0);
        base->generate(cg);
        result = new Location(base->result, fd->GetOffset());
        printer.printHelper.pop_back();
    } else
        result = dynamic_cast<VarDecl *>(fd)->rtLoc;
}

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr *> *a) : Expr(loc)  {
    ASSERT(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base)
        base->SetParent(this);
    (field = f)->SetParent(this);
    (actuals = a)->SetParentAll(this);
}
// special-case code for length() on arrays... sigh.
Type *Call::checkAndComputeResultType() {
    Type *baseType = base ? base->checkAndComputeResultType() : NULL;
    FnDecl *fd = dynamic_cast<FnDecl *>(field->GetDeclRelativeToBase(baseType));
    if (fd && fd->isMethodDecl() && !base) { // add implicit "this"
        base = new This(*field->GetLocation());
        base->SetParent(this);
        baseType = base->checkAndComputeResultType();
    }
    List<Type *> aTypes;
    for (int i = 0; i < actuals->NumElements(); i++)
        aTypes.Append(actuals->Nth(i)->checkAndComputeResultType());
// jdz cascade, above loop checks actuals before function confirmed.
// what about excess actuals? what if function doesn't exist at all?
    if (baseType && baseType->isArrayType() && strcmp(field->getName(), "length") == 0) {
        if (actuals->NumElements() != 0)
            ReportError::NumArgsMismatch(field, 0, actuals->NumElements());
        return Type::intType;
    }
    if (baseType == Type::errorType) {
        return Type::errorType;
    }
    if (baseType && !fd) { // had receiver, but no field in receiver (not class, wrong name, etc.)
        ReportError::FieldNotFoundInBase(field, baseType);
        return Type::errorType;
    } else if (!fd) { // no base, bad function
        ReportError::IdentifierNotDeclared(field, lookingForFunction);
        return Type::errorType;
    }

    List<VarDecl *> *formals = fd->GetFormals();
    if (formals->NumElements() != actuals->NumElements()) {
        ReportError::NumArgsMismatch(field, formals->NumElements(), actuals->NumElements());
    }
    for (int i = 0; i < formals->NumElements(); i++) {
        if (i >= actuals->NumElements())
            break;
        Type *at = aTypes.Nth(i);
        if (!at->isCompatibleWith(formals->Nth(i)->GetDeclaredType()))
            ReportError::ArgMismatch(actuals->Nth(i), i + 1, at,
                    formals->Nth(i)->GetDeclaredType());
    }
    return fd->GetReturnType();
}

void Call::generate(CodeGenerator *cg) {
    printer.print(string("Call : ") + string(field->getName()));

    Type *baseType = base ? base->checkAndComputeResultType() : NULL;
    if (baseType && baseType->isArrayType()) { // assume length() (i.e. semantically correct)
        printer.printHelper.push_back(0);
        base->generate(cg);
        result = cg->genArrayLen(base->result);
        printer.printHelper.pop_back();
        return;
    }

    printer.printHelper.push_back(1);
    List<Location *> l;
    for (int i = 0; i < actuals->NumElements(); i++) {
        if(i == actuals->NumElements() - 1 && !base)
            printer.printHelper.back() = 0;
        actuals->Nth(i)->generate(cg);
        l.Append(actuals->Nth(i)->result);
    }
    printer.printHelper.pop_back();

    Type *resultType = checkAndComputeResultType(); // force base to get set
    FnDecl *func = dynamic_cast<FnDecl *>(field->GetDeclRelativeToBase(baseType));
    if (base) {
        printer.printHelper.push_back(0);
        base->generate(cg);
        result = cg->genDynamicDispatch(base->result, func->GetOffset(), &l, !resultType->isEquivalentTo(Type::voidType));
        printer.printHelper.pop_back();
    } else {
        result = cg->genFunctionCall(func->GetFunctionLabel(), &l, !resultType->isEquivalentTo(Type::voidType));
    }
}



NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) {
    ASSERT(c != NULL);
    (cType = c)->SetParent(this);
}

Type *NewExpr::checkAndComputeResultType() {
    if (!cType->isClass()) {
        ReportError::IdentifierNotDeclared(cType->GetId(), lookingForClass);
        return Type::errorType;
    }
    return cType;
}
void NewExpr::generate(CodeGenerator *cg) {
    printer.print(string("New-Expression : ") + string(cType->GetId()->getName()));
    ClassDecl *cd = dynamic_cast<ClassDecl *>(cType->GetDeclForType());
    result = cg->genNew(cd->GetClassName(), cd->GetClassSize());
}

NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    ASSERT(sz != NULL && et != NULL);
    (size = sz)->SetParent(this);
    (elemType = et)->SetParent(this);
}
Type *NewArrayExpr::checkAndComputeResultType() {
    Type *st = size->checkAndComputeResultType();
    if (!st->isCompatibleWith(Type::intType))
        ReportError::NewArraySizeNotInteger(size);
    elemType->check();
    yyltype none;
    return new ArrayType(none, elemType);
}

void NewArrayExpr::generate(CodeGenerator *cg) {
    using namespace std;
    stringstream type;
    elemType->PrintToStream(type);
    printer.print(string("NewArray-Expression : ") + type.str());
    printer.printHelper.push_back(1);
    size->generate(cg);
    result = cg->genNewArray(size->GetResult());
    printer.printHelper.pop_back();
}

Type *ReadIntegerExpr::checkAndComputeResultType() {
    return Type::intType;
}
Type *ReadLineExpr::checkAndComputeResultType() {
    return Type::stringType;
}

void ReadIntegerExpr::generate(CodeGenerator *cg) {
    printer.print("ReadInteger-Expression");
    result = cg->genBuiltInCall(ReadInteger);
}

void ReadLineExpr::generate(CodeGenerator *cg) {
    printer.print("ReadLine-Expression");
    result = cg->genBuiltInCall(ReadLine);
}


