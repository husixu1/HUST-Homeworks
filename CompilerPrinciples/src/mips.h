/**
 * @file mips.h
 * @brief mips code generator
 *
 * @details
 * controls the generation of mips code, including managing allocation and use
 * of registers.
 *
 * @date 2018-06-13
 * @author Sixu Hu
**/

#ifndef MIPS_H
#define MIPS_H


#include "tac.h"
#include "list.h"
#include "flow.h"
#include <map>
class Location;


/// @brief mips instruction generte class
class Mips {
private:
    /// @brief all available registers
    typedef enum {
        zero, at, v0, v1, a0, a1, a2, a3,
        s0, s1, s2, s3, s4, s5, s6, s7,
        t0, t1, t2, t3, t4, t5, t6, t7,
        t8, t9, k0, k1, gp, sp, fp, ra,
        f0, f1, f2, f3, f4, f5, f6, f7,
        f8, f9, f10, f11, f12, f13, f14,
        f15, f16, f17, f18, f19, f20, f21,
        f22, f23, f24, f25, f26, f27, f28,
        f29, f30, f31
    } Register;

    /// @brief contents of a register
    struct RegContents {
        bool isDirty;
        Location *var;
        const char *name;
        bool isGeneralPurpose;
        bool mutexLocked;
        bool canDiscard;
    } regs[64];

    Register rs, rt, rd;

    /// @brief maps register to location
    std::map<Register, Location *> registerDescriptor;
    //std::map<Location*, Register> copyOf_registerDescriptor;
    int oldestTmpReg;

    typedef enum { ForRead, ForWrite } Reason;

    /// @brief load a word from src into register reg
    /// @param src the source location
    /// @param reg the destination variable
    void fillRegister(Location *src, Register reg);
    /// @brief store from register to memory location
    /// @param dst the destination to store
    /// @param reg the register whose value is to be stored
    void spillRegister(Location *dst, Register reg);
    /// @brief remove a discardable value from registerDescriptor
    void discardValueInRegister(Location *dst, Register reg);

    // register maintenence functions
    /// @brief get a clean register, clean one if necessary
    /// @return the cleaned register
    int regIndexOfNextClean();
    /// @brief randomlly select a unlocked or dirty register
    /// @return the selected register
    int regSelectRandom(); //uses a mutex.
    /// @brief pick a register for variable
    /// @param varLoc location of the variable
    /// @param copyRequired is copying required, if yes, a new register will be forced
    /// @return picked register
    int regPickRegForVar(Location *varLoc, bool copyRequired);
    /// @brief clean a register completely (spilling register with unknown location)
    /// @param reg the register to clean
    void regCleanRegister(Register reg);
    /// @brief wrapper for clean register
    void regCleanForBranch();

    /* register descriptor manipulation and access */
    /// @brief insert register into descriptor map
    /// @param varLoc variable location
    /// @param reg corresponding register
    void regDescriptorInsert(Location *varLoc, Register reg);
    /// @brief remove register from descriptor map
    /// @param varLoc variable location
    /// @param reg corresponding register
    void regDescriptorRemove(Location *varLoc, Register reg);
    /// @brief update variable location for register
    /// @param varLoc the updated location
    /// @param reg corresponding register
    void regDescriptorUpdateRegister(Location *varLoc, Register reg);
    /// @brief find register according to variable location
    /// @param varLoc variable location (key)
    /// @return the found register
    int regDescriptorLookupRegisterForVar(Location *varLoc);
    /// @brief find variable according to register
    /// @param reg the register (key)
    /// @return found variable
    Location *regDescriptorGetRegContents(Register reg);

    /// @brief wrapper for the upper function
    std::map<Register, Location *>::iterator regDescriptorlookupIterForReg(Location *varLoc);

    /// @brief generate code for a function call
    /// @detail used by generate LCall and generateACall, use jal for label and
    /// jalr for register (dynamic)
    /// @param dst the destination variable location
    /// @param function the function to call
    /// @param isL is static call
    void generateCallInstr(Location *dst, const char *function, bool isL);

    /// @brief map binary operator to mips instruction
    static const char *mipsName[BinaryOp::NumOps];
    /// @brief getter for mipsName
    /// @param code the binary operator code
    /// @return mips instruction
    static const char *tacToMips(BinaryOp::OpCode code);

    Instruction *currentInstruction;
public:
    /// @brief constructor, initializes global constants
    Mips();

    /// @brief helper to generate mips instruction
    /// @param fmt the printf-style format string
    /// @param ... printf arguments
    static void generate(const char *fmt, ...);
    /// @brief discard register for unused value
    /// @param dst variable location
    void generateDiscardValue(Location *dst);

    /// @brief assign constant to variable
    /// @param dst variable location
    /// @param val the constant to assign
    void generateLoadConstant(Location *dst, int val);
    /// @brief assign string constant to vriable
    /// @param dst variable location
    /// @param str the string constant to assign
    void generateLoadStringConstant(Location *dst, const char *str);
    /// @brief assign label constant to variable
    /// @param dst variable location
    /// @param label the label location to assign
    void generateLoadLabel(Location *dst, const char *label);

    /// @brief load memory content into variable
    /// @param dst desitnation variable location
    /// @param reference variable storing address
    /// @param offset offset to address
    void generateLoad(Location *dst, Location *reference, int offset);
    /// @brief store variable content into memory
    /// @param reference memory address
    /// @param value variable to store
    /// @param offset offset to address
    void generateStore(Location *reference, Location *value, int offset);

    /// @brief copy one variable to an other
    /// @param dst destination variable location
    /// @param src source variable location
    void generateCopy(Location *dst, Location *src);

	/// @brief generate mips code for binary operation
	/// @param code the binary operator
	/// @param dst destination variable location
	/// @param op1 operator one
	/// @param op2 operator two
    void generateBinaryOp(BinaryOp::OpCode code, Location *dst,
            Location *op1, Location *op2);
    /// @brief fp version of generateBinaryOp, called by generateBinaryOp
    /// @see generateBinaryOp()
    void FP_generateBinaryOp(const char *operation, Location *dst,
            Location *op1, Location *op2);

    /// @brief generate a label mark
    /// @param label name of the label
    void generateLabel(const char *label);
    /// @brief generate a goto-command
    /// @param label name of the label to go to
    void generateGoto(const char *label);
    /// @brief generate conditional branch using beqz
    /// @param test condition variable
    /// @param label label to jump to
    void generateIfZ(Location *test, const char *label);
    /// @brief generate return instruction
    /// @detail save retrun variable to corresponding register and revert
    /// the stack frame.
    /// @param returnVal return value
    void generateReturn(Location *returnVal);

    /// @brief calles's code when entering the function
    /// @details callee decrease $sp to make space and save current vaules
    /// of $fp and $ra , and setup new $fp then decrease $sp again to make
    /// space for all local variables
    /// @param frameSize size of the stack frame
    void generateBeginFunction(int frameSize);
    /// @brief generate ending code of the function, calls generateReturn()
    void generateEndFunction();

    /// @brief push parameter into stack frame
    /// @param arg the parameter variable location
    void generateParam(Location *arg);
    /// @brief generate call for static function call
    /// @param result result variable location
    /// @param label function label name
    void generateLCall(Location *result, const char *label);
    /// @brief generate call for dynamic function call
    /// @param result result variable location
    /// @param fnAddr variable storing the function address
    void generateACall(Location *result, Location *fnAddr);
    /// @brief remove all parameters from stack
    /// @param bytes bytes to remove
    void generatePopParams(int bytes);

    /// @brief generate class virtual table
    /// @param label class name
    /// @param methodLabels methods name
    void generateVTable(const char *label, List<const char *> *methodLabels);

    /// @brief generate header for the program
    void generateHeader();

    /// @brief current instruction
    class CurrentInstruction;
};


/// @brief current mips instruction
class Mips::CurrentInstruction {
public:
    /// @brief constructor
    CurrentInstruction(Mips &mips, Instruction *instr)
        : mips( mips ) {
        mips.currentInstruction = instr;
    }

    /// @brief destructor
    ~CurrentInstruction() {
        mips.currentInstruction = NULL;
    }

private:
    Mips &mips;
};


#endif // MIPS_H
