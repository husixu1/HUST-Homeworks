/**
 * @file codegen.h
 * @brief generate code using tac instructions
 *
 * @details
 * tac instructions are defined in tac.h this file parse the ast tree,
 * generate tac instructions and put all tac instructions in one linear list.
 *
 * @date 2018-06-13
 * @author Sixu Hu
**/

#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <cstdlib>
#include <list>
#include "tac.h"
class FnDecl;


/// @brief built-in functions
typedef enum { Alloc, ReadLine, ReadInteger, StringEqual,
        PrintInt, PrintString, PrintBool, Halt, NumBuiltIns
} BuiltIn;

/// @brief tac code generator class
class CodeGenerator {
private:
    //[husixu] stores tac instructions (represented by Instruction class)
    /// @brief the generated tac code
    std::list<Instruction *> code;
    /// @brief current stack offset and global offset
    int curStackOffset, curGlobalOffset;
    /// @brief current function
    BeginFunc *insideFn;

public:
    /// @brief som convenient offset constant
    static const int OffsetToFirstLocal = -8,
                     OffsetToFirstParam = 4,
                     OffsetToFirstGlobal = 0;
    static const int VarSize = 4;

    /// @brief location of "this" pointer
    static Location *ThisPtr;

    /// @brief constructor
    CodeGenerator();

    /// @brief generate a unique global label, does not generate tac instructions
    /// @return label name
    char *NewLabel();


    // these three functions does not generate tac instructions
    /// @brief generate for a new uniquely named temp variable.
    /// @return the location of the variable
    Location *genTempVar();
    /// @brief generate for a new uniquely named local variable.
    /// @param varName name of the variable
    /// @return the location of the variable
    Location *genLocalVariable(const char *varName);
    /// @brief generate for a new uniquely named global variable.
    /// @param varName name of the variable
    /// @return the location of the variable
    Location *genGlobalVariable(const char *varName);

    // these three function create a temp var to load a constant value
    /// @brief generate a tac instruction and a temp var to load a constant int.
    /// @param value the constant int to load
    /// @return the location of the generated variable
    Location *genLoadConstant(int value);
    /// @brief generate a tac instruction and a temp var to load a constant string.
    /// @param str the constant string to load
    /// @return the location of the generated variable
    Location *genLoadConstant(const char *str);
    /// @brief generate a tac instruction and a temp var to load a constant location.
    /// @param str the constant location to load
    /// @return the location of the generated variable
    Location *genLoadLabel(const char *label);


    /// @brief generate assign instruction
    /// @param dst destination variable location
    /// @param src source variable location
    void genAssign(Location *dst, Location *src);

    /// @brief generte store instrution to store value to address
    /// @param addr the address to store into
    /// @param var the variable location
    /// @param offset offset in array (0 for no array)
    void genStore(Location *addr, Location *var, int offset = 0);
    /// @brief generate load instrution to load value from address
    /// @param addr the address to load
    /// @param offset offset to the address
    /// @return generated variable location
    Location *genLoad(Location *addr, int offset = 0);

    /// @brief generate biary operator instructions
    /// @param opName operator name
    /// @param op1 first operand
    /// @param op2 second operand
    /// @return temporary result location
    Location *genBinaryOp(const char *opName, Location *op1, Location *op2);

    /// @brief push parameterinto stack
    /// @param param the parameter's location
    void genPushParam(Location *param);
    /// @brief pop parameter from stak
    /// @param numBytesOfParams bytes to pop
    void genPopParams(int numBytesOfParams);

    /// @brief generate instruction for LCall (call a compile-time address)
    /// @param label the label to jump to
    /// @param fnHasReturnValue is function has a return value
    /// @return the retun value's temp varable's location
    Location *genLCall(const char *label, bool fnHasReturnValue);
    /// @brief generate intruction for ACall (call a runtime address)
    /// @param fnAddr address of the variable pointing to the function address
    /// @param fnHasReturnValue is function has a return value
    /// @return the retun value's temp varable's location
    Location *genACall(Location *fnAddr, bool fnHasReturnValue);
    /// @brief generate instructions for built-in functions
    /// @param b the builtin function identifier
    /// @param arg1 argument 1
    /// @param arg2 argument 2
    /// @return the retun value's temp varable's location
    Location *genBuiltInCall(BuiltIn b, Location *arg1 = NULL, Location *arg2 = NULL);

    /// @brief generate if instruction control flow
    /// @param test the condition variable location
    /// @param label the label to jump to
    void genIfZ(Location *test, const char *label);
    /// @brief generate goto instruction control flow
    /// @param label the label to jump to
    void genGoto(const char *label);
    /// @brief generate return instruction
    /// @param val the variable's location whose value is to be returned
    void genReturn(Location *val = NULL);
    /// @brief generate instruction for label
    /// @param label the label name
    void genLabel(const char *label);


    /// @brief generate instructions which marks the satart of the function
    /// @param fn target function declaration
    /// @return begining of the function
    BeginFunc *genBeginFunc(FnDecl *fn);
    /// @brief generate instructions which marks the end of the function
    void genEndFunc();

    /// @brief generate virtual table for method
    /// @param className target class name
    /// @param methodLabels list of method labels to generate
    void genVTable(const char *className, List<const char *> *methodLabels);

    Location *genNewArray(Location *numElements);
    Location *genArrayLen(Location *array);
    Location *genNew(const char *vTableLabel, int instanceSize);
    Location *genDynamicDispatch(Location *obj, int vtableOffset, List<Location *> *args, bool hasReturnValue);
    Location *genSubscript(Location *array, Location *index);
    Location *genFunctionCall(const char *fnLabel, List<Location *> *args, bool hasReturnValue);

    // private helper, not for public user
    Location *genMethodCall(Location *rcvr, Location *meth, List<Location *> *args, bool hasReturnValue);
    void genHaltWithMessage(const char *msg);

    /// @brief translate all generated tac code in to mips instructions
    void generateFinalCode();
};


#endif // CODEGEN_H_
