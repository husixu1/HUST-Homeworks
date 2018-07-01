/* File: tac.cc
 * ------------
 * Implementation of Location class and Instruction class/subclasses.
 */

#include "tac.h"
#include "mips.h"
#include <cstring>

Location::Location(Segment s, int o, const char *name) :
  variableName(strdup(name)), segment(s), offset(o), base(NULL) , refOffset(0),_isReference(false)
{

}


void Instruction::Print() {
  printf("\t%s ;\n", printed);
}

void Instruction::generate(Mips *mips) {
  Mips::CurrentInstruction ci(*mips, this);

  // generate tac code comments
  if (*printed)
    mips->generate("#### %s", printed);

  // generate real instruction
  generateSpecific(mips);
}

DiscardValue::DiscardValue(Location *d)
  : dst(d) {
  ASSERT(dst != NULL);
  //sprintf(printed, "Last use of %s.", dst->getName());
}

void DiscardValue::generateSpecific(Mips *mips) {
  mips->generateDiscardValue(dst);
}

LoadConstant::LoadConstant(Location *d, int v)
  : dst(d), val(v) {
  ASSERT(dst != NULL);
  sprintf(printed, "%s = %d", dst->getName(), val);
  numVars = 1;
  varA = d;
}
void LoadConstant::generateSpecific(Mips *mips) {
  mips->generateLoadConstant(dst, val);
}


LoadStringConstant::LoadStringConstant(Location *d, const char *s)
  : dst(d) {
  ASSERT(dst != NULL && s != NULL);
  const char *quote = (*s == '"') ? "" : "\"";
  str = new char[strlen(s) + 2*strlen(quote) + 1];
  sprintf(str, "%s%s%s", quote, s, quote);
  quote = (strlen(str) > 50) ? "...\"" : "";
  sprintf(printed, "%s = %.50s%s", dst->getName(), str, quote);
  numVars = 1;
  varA = d;
}
void LoadStringConstant::generateSpecific(Mips *mips) {
  mips->generateLoadStringConstant(dst, str);
}


LoadLabel::LoadLabel(Location *d, const char *l)
  : dst(d), label(strdup(l)) {
  ASSERT(dst != NULL && label != NULL);
  sprintf(printed, "%s = %s", dst->getName(), label);
  numVars = 1;
  varA = d;
}
void LoadLabel::generateSpecific(Mips *mips) {
  mips->generateLoadLabel(dst, label);
}


Assign::Assign(Location *d, Location *s)
  : dst(d), src(s) {
  ASSERT(dst != NULL && src != NULL);
  sprintf(printed, "%s = %s", dst->getName(), src->getName());
  numVars = 2;
  varA = d;
  varB = s;

}
void Assign::generateSpecific(Mips *mips) {
  mips->generateCopy(dst, src);
}


Load::Load(Location *d, Location *s, int off)
  : dst(d), src(s), offset(off) {
  ASSERT(dst != NULL && src != NULL);
  if (offset)
    sprintf(printed, "%s = *(%s + %d)", dst->getName(), src->getName(), offset);
  else
    sprintf(printed, "%s = *(%s)", dst->getName(), src->getName());
  numVars = 2;
  varA = d;
  varB = s;
}
void Load::generateSpecific(Mips *mips) {
  mips->generateLoad(dst, src, offset);
}


Store::Store(Location *d, Location *s, int off)
  : dst(d), src(s), offset(off) {
  ASSERT(dst != NULL && src != NULL);
  if (offset)
    sprintf(printed, "*(%s + %d) = %s", dst->getName(), offset, src->getName());
  else
    sprintf(printed, "*(%s) = %s", dst->getName(), src->getName());
  numVars = 2;
  varA = d;
  varB = s;
}
void Store::generateSpecific(Mips *mips) {
  mips->generateStore(dst, src, offset);
}


const char * const BinaryOp::opName[BinaryOp::NumOps]  = {"+", "-", "*", "/", "%", "==", "<", "&&", "||"};;

BinaryOp::OpCode BinaryOp::OpCodeForName(const char *name) {
  for (int i = 0; i < NumOps; i++)
    if (opName[i] && !strcmp(opName[i], name))
	return (OpCode)i;
  fatal("Unrecognized Tac operator: '%s'\n", name);
  return Add; // can't get here, but compiler doesn't know that
}

BinaryOp::BinaryOp(OpCode c, Location *d, Location *o1, Location *o2)
  : code(c), dst(d), op1(o1), op2(o2) {
  ASSERT(dst != NULL && op1 != NULL && op2 != NULL);
  ASSERT(code >= 0 && code < NumOps);
  sprintf(printed, "%s = %s %s %s", dst->getName(), op1->getName(), opName[code], op2->getName());
  numVars = 3;
  varA = d;
  varB = o1;
  varC = o2;
}
void BinaryOp::generateSpecific(Mips *mips) {
  mips->generateBinaryOp(code, dst, op1, op2);
}

Label::Label(const char *l) : label(strdup(l)) {
  ASSERT(label != NULL);
  *printed = '\0';
}
void Label::Print() {
  printf("%s:\n", label);
}
void Label::generateSpecific(Mips *mips) {
  mips->generateLabel(label);
}

Goto::Goto(const char *l) : label(strdup(l)) {
  ASSERT(label != NULL);
  sprintf(printed, "Goto %s", label);
}
void Goto::generateSpecific(Mips *mips) {
  mips->generateGoto(label);
}

IfZ::IfZ(Location *te, const char *l)
   : test(te), label(strdup(l)) {
  ASSERT(test != NULL && label != NULL);
  sprintf(printed, "IfZ %s Goto %s", test->getName(), label);
  numVars = 1;
  varA = te;
}
void IfZ::generateSpecific(Mips *mips) {
  mips->generateIfZ(test, label);
}

BeginFunc::BeginFunc() {
  sprintf(printed,"BeginFunc (unassigned)");
  frameSize = -555; // used as sentinel to recognized unassigned value
}
void BeginFunc::SetFrameSize(int numBytesForAllLocalsAndTemps) {
  frameSize = numBytesForAllLocalsAndTemps;
  sprintf(printed,"BeginFunc %d", frameSize);
}
void BeginFunc::generateSpecific(Mips *mips) {
  mips->generateBeginFunction(frameSize);
}

EndFunc::EndFunc() : Instruction() {
  sprintf(printed, "EndFunc");
}
void EndFunc::generateSpecific(Mips *mips) {
  mips->generateEndFunction();
}

Return::Return(Location *v) : val(v) {
  sprintf(printed, "Return %s", val? val->getName() : "");
  numVars = 1;
  varA = v;
}
void Return::generateSpecific(Mips *mips) {
  mips->generateReturn(val);
}

PushParam::PushParam(Location *p)
  :  param(p) {
  ASSERT(param != NULL);
  sprintf(printed, "PushParam %s", param->getName());
  numVars = 1;
  varA = p;
}
void PushParam::generateSpecific(Mips *mips) {
  mips->generateParam(param);
}

PopParams::PopParams(int nb)
  :  numBytes(nb) {
  sprintf(printed, "PopParams %d", numBytes);
}
void PopParams::generateSpecific(Mips *mips) {
  mips->generatePopParams(numBytes);
}


LCall::LCall(const char *l, Location *d)
  :  label(strdup(l)), dst(d) {
  sprintf(printed, "%s%sLCall %s", dst? dst->getName(): "", dst?" = ":"", label);
  if (dst) {
      numVars = 1;
      varA = d;
  }
}
void LCall::generateSpecific(Mips *mips) {
  mips->generateLCall(dst, label);
}

ACall::ACall(Location *ma, Location *d)
  : dst(d), methodAddr(ma) {
  ASSERT(methodAddr != NULL);
  sprintf(printed, "%s%sACall %s", dst? dst->getName(): "", dst?" = ":"",
	    methodAddr->getName());
  if (dst) {
      numVars = 1;
      varA = d;
  }
}
void ACall::generateSpecific(Mips *mips) {
  mips->generateACall(dst, methodAddr);
}

VTable::VTable(const char *l, List<const char *> *m)
  : methodLabels(m), label(strdup(l)) {
  ASSERT(methodLabels != NULL && label != NULL);
  sprintf(printed, "VTable for class %s", l);
}

void VTable::Print() {
  printf("VTable %s =\n", label);
  for (int i = 0; i < methodLabels->NumElements(); i++)
    printf("\t%s,\n", methodLabels->Nth(i));
  printf("; \n");
}
void VTable::generateSpecific(Mips *mips) {
  mips->generateVTable(label, methodLabels);
}
