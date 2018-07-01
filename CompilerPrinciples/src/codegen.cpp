/**
 * @file codegen.cpp
 * @brief the implementation of code generator
 *
 * @date 2018-06-13
 * @author Sixu Hu
**/

#include "codegen.h"
#include <string.h>
#include "tac.h"
#include "mips.h"
#include "ast_decl.h"
#include "errors.h"
#include "printer.h"

Location *CodeGenerator::ThisPtr = new Location(fpRelative, 4, "this");

CodeGenerator::CodeGenerator() {
    curGlobalOffset = 0;
}

char *CodeGenerator::NewLabel() {
    static int nextLabelNum = 0;
    char temp[10];
    sprintf(temp, "_L%d", nextLabelNum++);
    return strdup(temp);
}


Location *CodeGenerator::genTempVar() {
    static int nextTempNum;
    char temp[10];
    Location *result = NULL;
    sprintf(temp, "_tmp%d", nextTempNum++);
    return genLocalVariable(temp);
}


Location *CodeGenerator::genLocalVariable(const char *varName) {
    curStackOffset -= VarSize;
    return new Location(fpRelative, curStackOffset + 4,  varName);
}

Location *CodeGenerator::genGlobalVariable(const char *varName) {
    curGlobalOffset += VarSize;
    return new Location(gpRelative, curGlobalOffset - 4, varName);
}


Location *CodeGenerator::genLoadConstant(int value) {
    Location *result = genTempVar();
    code.push_back(new LoadConstant(result, value));
    return result;
}

Location *CodeGenerator::genLoadConstant(const char *s) {
    Location *result = genTempVar();
    code.push_back(new LoadStringConstant(result, s));
    return result;
}

Location *CodeGenerator::genLoadLabel(const char *label) {
    Location *result = genTempVar();
    code.push_back(new LoadLabel(result, label));
    return result;
}


void CodeGenerator::genAssign(Location *dst, Location *src) {
    code.push_back(new Assign(dst, src));
}


Location *CodeGenerator::genLoad(Location *ref, int offset) {
    Location *result = genTempVar();
    code.push_back(new Load(result, ref, offset));
    return result;
}

void CodeGenerator::genStore(Location *dst, Location *src, int offset) {
    code.push_back(new Store(dst, src, offset));
}


Location *CodeGenerator::genBinaryOp(const char *opName, Location *op1,
        Location *op2) {
    Location *result = genTempVar();
    code.push_back(new BinaryOp(BinaryOp::OpCodeForName(opName), result, op1, op2));
    return result;
}


void CodeGenerator::genLabel(const char *label) {
    code.push_back(new Label(label));
}

void CodeGenerator::genIfZ(Location *test, const char *label) {
    code.push_back(new IfZ(test, label));
}

void CodeGenerator::genGoto(const char *label) {
    code.push_back(new Goto(label));
}

void CodeGenerator::genReturn(Location *val) {
    code.push_back(new Return(val));
}


BeginFunc *CodeGenerator::genBeginFunc(FnDecl *fn) {
    BeginFunc *result = new BeginFunc;
    code.push_back(insideFn = result);
    List<VarDecl *> *formals = fn->GetFormals();
    int start = OffsetToFirstParam;
    if (fn->isMethodDecl())
        start += VarSize;
    for (int i = 0; i < formals->NumElements(); i++)
        formals->Nth(i)->rtLoc = new Location(fpRelative, i * VarSize + start, formals->Nth(i)->getName());
    curStackOffset = OffsetToFirstLocal;
    return result;
}

void CodeGenerator::genEndFunc() {
    code.push_back(new EndFunc());
    insideFn->SetFrameSize(OffsetToFirstLocal - curStackOffset);
    insideFn = NULL;
}

void CodeGenerator::genPushParam(Location *param) {
    code.push_back(new PushParam(param));
}

void CodeGenerator::genPopParams(int numBytesOfParams) {
    ASSERT(numBytesOfParams >= 0 && numBytesOfParams % VarSize == 0); // sanity check
    if (numBytesOfParams > 0)
        code.push_back(new PopParams(numBytesOfParams));
}

Location *CodeGenerator::genLCall(const char *label, bool fnHasReturnValue) {
    Location *result = fnHasReturnValue ? genTempVar() : NULL;
    code.push_back(new LCall(label, result));
    return result;
}

Location *CodeGenerator::genFunctionCall(const char *fnLabel, List<Location *> *args, bool hasReturnValue) {
    for (int i = args->NumElements() - 1; i >= 0; i--) // push params right to left
        genPushParam(args->Nth(i));
    Location *result = genLCall(fnLabel, hasReturnValue);
    genPopParams(args->NumElements()*VarSize);
    return result;
}

Location *CodeGenerator::genACall(Location *fnAddr, bool fnHasReturnValue) {
    Location *result = fnHasReturnValue ? genTempVar() : NULL;
    code.push_back(new ACall(fnAddr, result));
    return result;
}

Location *CodeGenerator::genMethodCall(Location *rcvr,
        Location *meth, List<Location *> *args, bool fnHasReturnValue) {
    for (int i = args->NumElements() - 1; i >= 0; i--)
        genPushParam(args->Nth(i));
    genPushParam(rcvr);	// hidden "this" parameter
    Location *result = genACall(meth, fnHasReturnValue);
    genPopParams((args->NumElements() + 1)*VarSize);
    return result;
}


static struct _builtin {
    const char *label;
    int numArgs;
    bool hasReturn;
} builtins[] = {
    {"_Alloc", 1, true},
    {"_ReadLine", 0, true},
    {"_ReadInteger", 0, true},
    {"_StringEqual", 2, true},
    {"_PrintInt", 1, false},
    {"_PrintString", 1, false},
    {"_PrintBool", 1, false},
    {"_Halt", 0, false}
};

Location *CodeGenerator::genBuiltInCall(BuiltIn bn, Location *arg1, Location *arg2) {
    ASSERT(bn >= 0 && bn < NumBuiltIns);
    struct _builtin *b = &builtins[bn];
    Location *result = NULL;

    if (b->hasReturn)
        result = genTempVar();
    // verify appropriate number of non-NULL arguments given
    ASSERT((b->numArgs == 0 && !arg1 && !arg2)
            || (b->numArgs == 1 && arg1 && !arg2)
            || (b->numArgs == 2 && arg1 && arg2));
    if (arg2)
        code.push_back(new PushParam(arg2));
    if (arg1)
        code.push_back(new PushParam(arg1));
    code.push_back(new LCall(b->label, result));
    genPopParams(VarSize * b->numArgs);
    return result;
}


void CodeGenerator::genVTable(const char *className, List<const char *> *methodLabels) {
    code.push_back(new VTable(className, methodLabels));
}


void CodeGenerator::generateFinalCode() {
    // print tac to file
    TacPrinter &tacPrinter = TacPrinter::getDefaultPrinter();
    for (auto instruction : code)
        tacPrinter.print(instruction->getPrinted());

    Mips mips;
    mips.generateHeader();

    std::list<Instruction *>::iterator p = code.begin(),
                                       begin_block = code.end(),
                                       end_block = code.end();

    int block_count = 0;
    Block current_block;
    std::list<ControlFlowGraph> cfg_list;
    std::list<ControlFlowGraph>::iterator cfg_list_iter;
    while (p != code.end()) {
        // watch for beginning of function
        if (dynamic_cast<BeginFunc *>(*p)) {
            begin_block = p;
            current_block.push_back((*begin_block));

            while (p != code.end()) {
                if (dynamic_cast<EndFunc *>(*p)) {
                    break;
                }
                ++p;
            }
            if (p == code.end())
                p = begin_block;
            else {
                end_block = p;
                current_block.push_back((*end_block));
                Block::iterator b_iter = current_block.begin();

                ControlFlowGraph cfg(begin_block, end_block);
                cfg_list.push_back(cfg);
                current_block.clear();
                block_count++;

                for (p = begin_block; p != end_block; ++p);
            }
        }
        current_block.push_back((*p));
        ++p;
    }

    // a list of variable_list
    std::list<std::map<Location *, Instruction *> > liveness_list;
    std::list<std::map<Location *, Instruction *> >::iterator liveness_list_iter;

    // a map from key:<the first var location in instruction> to value:<the instruction>
    // variable_list is such a list that:
    // its keys are all the variable used, and the corresponding values are the *last* instruction
    // that uses this value. This list is used for discarding values.
    std::map<Location *, Instruction *> variable_list;
    std::map<Location *, Instruction *>::iterator variable_list_iter;

    // iterate over all control flow graphs for each function
    for (cfg_list_iter = cfg_list.begin(); cfg_list_iter != cfg_list.end(); cfg_list_iter++) {
        variable_list.clear();

        // construct a forward flow alias for the cfg
        ControlFlowGraph::ForwardFlow flow((*cfg_list_iter));
        ControlFlowGraph::ForwardFlow::iterator cfg_iter = flow.first();

        // iterate through the current control flow graph
        // cfg_iter is the iterator of instructions in the cfg
        for (cfg_iter; cfg_iter != flow.last(); cfg_iter++) {
            // calls Instruction->numVars,
            // returning number of the variable (instructionr register count)
            int numVars = (*cfg_iter)->numVars;

            if (numVars == 1 || numVars == 2 || numVars == 3) {
                // try find and erase the instruction with key "varA" in the variable_list
                variable_list_iter = variable_list.find((*cfg_iter)->varA);
                if (variable_list_iter != variable_list.end())
                    variable_list.erase(variable_list_iter);

                // insert {varA, instruction} pair into variable_list, if name of varA contains "_tmp"
                if (strstr((*cfg_iter)->varA->getName(), "_tmp") != NULL) {
                    variable_list.insert(std::pair<Location *, Instruction *>((*cfg_iter)->varA, (*cfg_iter)));
                }
            }

            // do similar operations, but to instructions with 2/3 operands
            if (numVars == 2 || numVars == 3) {
                variable_list_iter = variable_list.find((*cfg_iter)->varB);
                if (variable_list_iter != variable_list.end())
                    variable_list.erase(variable_list_iter);

                if (strstr((*cfg_iter)->varB->getName(), "_tmp") != NULL) {
                    variable_list.insert(std::pair<Location *, Instruction *>((*cfg_iter)->varB, (*cfg_iter)));
                }
            }
            if (numVars == 3) {
                variable_list_iter = variable_list.find((*cfg_iter)->varC);
                if (variable_list_iter != variable_list.end())
                    variable_list.erase(variable_list_iter);

                if (strstr((*cfg_iter)->varC->getName(), "_tmp") != NULL) {
                    variable_list.insert(std::pair<Location *, Instruction *>((*cfg_iter)->varC, (*cfg_iter)));
                }
            }
        }
        // push the variableLocation-instruction map to liveness_list,
        // representing the location-instruction map for a single function
        liveness_list.push_back(variable_list);
    }


    block_count = 0;
    p = code.begin();
    // iterate through all instructions
    while (p != code.end()) {
        // watch for beginnings of functions
        if (dynamic_cast<BeginFunc *>(*p)) {
            begin_block = p;

            // iter until the end of the function
            while (p != code.end()) {
                if (dynamic_cast<EndFunc *>(*p))
                    break;
                ++p;
            }

            if (p == code.end())
                p = begin_block;
            else {
                // found the end of the function
                end_block = p;
                block_count++;
                if (block_count == 1) {
                    liveness_list_iter = liveness_list.begin();
                } else {
                    liveness_list_iter++;
                }

                // here, [begin_block, end_block) is the instruction in a function
                for (p = begin_block; p != end_block; ++p) {
                    // generate mips code
                    (*p)->generate(&mips);

                    // iter through function address-insturction maps
                    for (variable_list_iter = (*liveness_list_iter).begin();
                            variable_list_iter != (*liveness_list_iter).end();
                            variable_list_iter++) {

                        // if current instruction equals instruction in the live list
                        // then discard the destination variable.
                        // That is, if this instruction is the last one to use the value "variable_list_iter->first"
                        // then no instructions will use it further, and it can and should be safely discarded.
                        if ((*p) == (*variable_list_iter).second) {
                            Instruction *discard_instr = new DiscardValue(variable_list_iter->first);
                            discard_instr->generateSpecific(&mips);
                        }
                    }
                }
            }
        }
        // generate normal instructions
        (*p)->generate(&mips);
        ++p;
    }
}



Location *CodeGenerator::genArrayLen(Location *array) {
    return genLoad(array, -4);
}

Location *CodeGenerator::genNew(const char *vTableLabel, int instanceSize) {
    Location *size = genLoadConstant(instanceSize);
    Location *result = genBuiltInCall(Alloc, size);
    Location *vt = genLoadLabel(vTableLabel);
    genStore(result, vt);
    return result;
}


Location *CodeGenerator::genDynamicDispatch(Location *rcvr, int vtableOffset, List<Location *> *args, bool hasReturnValue) {
    Location *vptr = genLoad(rcvr); // load vptr
    ASSERT(vtableOffset >= 0);
    Location *m = genLoad(vptr, vtableOffset * 4);
    return genMethodCall(rcvr, m, args, hasReturnValue);
}

// all variables (ints, bools, ptrs, arrays) are 4 bytes in for code generation
// so this simplifies the math for offsets
Location *CodeGenerator::genSubscript(Location *array, Location *index) {
    Location *zero = genLoadConstant(0);
    Location *isNegative = genBinaryOp("<", index, zero);
    Location *count = genLoad(array, -4);
    Location *isWithinRange = genBinaryOp("<", index, count);
    Location *pastEnd = genBinaryOp("==", isWithinRange, zero);
    Location *outOfRange = genBinaryOp("||", isNegative, pastEnd);
    const char *pastError = NewLabel();
    genIfZ(outOfRange, pastError);
    genHaltWithMessage(errorArrayOutOfBound);
    genLabel(pastError);
    Location *four = genLoadConstant(VarSize);
    Location *offset = genBinaryOp("*", four, index);
    Location *elem = genBinaryOp("+", array, offset);
    return new Location(elem, 0);
}



Location *CodeGenerator::genNewArray(Location *numElems) {
    Location *zero = genLoadConstant(0);
    Location *isNegative = genBinaryOp("<", numElems, zero);
    const char *pastError = NewLabel();
    genIfZ(isNegative, pastError);
    genHaltWithMessage(errorArrayBadSize);
    genLabel(pastError);

    // need (numElems+1)*VarSize total bytes (extra 1 is for length)
    Location *arraySize = genLoadConstant(1);
    Location *num = genBinaryOp("+", arraySize, numElems);
    Location *four = genLoadConstant(VarSize);
    Location *bytes = genBinaryOp("*", num, four);
    Location *result = genBuiltInCall(Alloc, bytes);
    genStore(result, numElems);
    return genBinaryOp("+", result, four);
}


void CodeGenerator::genHaltWithMessage(const char *message) {
    Location *msg = genLoadConstant(message);
    genBuiltInCall(PrintString, msg);
    genBuiltInCall(Halt, NULL);
}


