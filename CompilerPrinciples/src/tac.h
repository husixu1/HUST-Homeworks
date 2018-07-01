/**
 * @file tac.h
 * @brief instruction classs for generating tac code.
 *
 * @details
 * print() method print the tac code, and generate method is used to convert
 * tac code to mips instruction. The operands of instructions are location class
 *
 * @date 2018-06-13
 * @author Sixu Hu
**/

#ifndef TAC_H_
#define TAC_H_

#include "list.h"
class Mips;


// that is, this class defines the global/stak variables with positions
typedef enum {fpRelative, gpRelative} Segment;

class Location {
protected:
    /// @brief name of the original variable
    const char *variableName;
    /// @brief is the location fp-relative or gp-relative (local or global)
    Segment segment;
    /// @brief offset to the segment
    int offset;

    /// @brief base location of the reference
    Location *base;
    /// @brief reference offset
    int refOffset;
    /// @brief is the variable a reference
    bool _isReference;

public:
    /// @brief constructor
    Location(Segment seg, int offset, const char *name);
    /// @brief constructor for reference
    Location(Location *base_ptr, int refOff) :
        variableName(base_ptr->variableName), segment(base_ptr->segment),
        offset(base_ptr->offset), base(base_ptr), refOffset(refOff), _isReference(true) {}

    // getters
    const char *getName() const {
        return variableName;
    }
    Segment GetSegment() const {
        return segment;
    }
    int GetOffset() const {
        return offset;
    }
    Location *GetBase() const {
        return base;
    }
    bool isReference() const {
        return _isReference;
    }
    int GetRefOffset() const {
        return refOffset;
    }
};



/// @brief base class of all instruction
class Instruction {
protected:
    /// @brief the formatted tac code
    char printed[128];

public:
    /// @brief print tac code
    virtual void Print();

    /// @brief generate mips assembly instruction
    /// @param mips the mips code generator
    virtual void generateSpecific(Mips *mips) = 0;

    /// @brief generate both comment and instruction, calls generateSpecific()
    /// @param mips the mips code generator
    void generate(Mips *mips);

    /// @brief getter for printed
    char *getPrinted() {
        return printed;
    }

    /// @brief operands A
    Location *varA;
    /// @brief operands B
    Location *varB;
    /// @brief operands C
    Location *varC;

    /// @brief number of operands
    int numVars;
};


class LoadConstant;
class LoadStringConstant;
class LoadLabel;
class Assign;
class Load;
class Store;
class BinaryOp;
class Label;
class Goto;
class IfZ;
class BeginFunc;
class EndFunc;
class Return;
class PushParam;
class PopParams;
class LCall;
class ACall;
class VTable;

class DiscardValue;

class DiscardValue: public Instruction {
    Location *dst;
public:
    DiscardValue(Location *dst);
    void generateSpecific(Mips *mips);
};


class LoadConstant: public Instruction {
    Location *dst;
    int val;
public:
    LoadConstant(Location *dst, int val);
    void generateSpecific(Mips *mips);
};

class LoadStringConstant: public Instruction {
    Location *dst;
    char *str;
public:
    LoadStringConstant(Location *dst, const char *s);
    void generateSpecific(Mips *mips);
};

class LoadLabel: public Instruction {
    Location *dst;
    const char *label;
public:
    LoadLabel(Location *dst, const char *label);
    void generateSpecific(Mips *mips);
};

class Assign: public Instruction {
    Location *dst, *src;
public:
    Assign(Location *dst, Location *src);
    void generateSpecific(Mips *mips);
};

class Load: public Instruction {
    Location *dst, *src;
    int offset;
public:
    Load(Location *dst, Location *src, int offset = 0);
    void generateSpecific(Mips *mips);
};

class Store: public Instruction {
    Location *dst, *src;
    int offset;
public:
    Store(Location *d, Location *s, int offset = 0);
    void generateSpecific(Mips *mips);
};

class BinaryOp: public Instruction {

public:
    typedef enum {Add, Sub, Mul, Div, Mod, Eq, Less, And, Or, NumOps} OpCode;
    static const char *const opName[NumOps];
    static OpCode OpCodeForName(const char *name);

protected:
    OpCode code;
    Location *dst, *op1, *op2;
public:
    BinaryOp(OpCode c, Location *dst, Location *op1, Location *op2);
    void generateSpecific(Mips *mips);
};

class Label: public Instruction {
    const char *label;
public:
    Label(const char *label);
    void Print();
    void generateSpecific(Mips *mips);
    const char *text() const {
        return label;
    }
};

class Goto: public Instruction {
    const char *label;
public:
    Goto(const char *label);
    void generateSpecific(Mips *mips);
    const char *branch_label() const {
        return label;
    }
};

class IfZ: public Instruction {
    Location *test;
    const char *label;
public:
    IfZ(Location *test, const char *label);
    void generateSpecific(Mips *mips);
    const char *branch_label() const {
        return label;
    }
};

class BeginFunc: public Instruction {
    int frameSize;
public:
    BeginFunc();
    // used to backpatch the instruction with frame size once known
    void SetFrameSize(int numBytesForAllLocalsAndTemps);
    void generateSpecific(Mips *mips);
};

class EndFunc: public Instruction {
public:
    EndFunc();
    void generateSpecific(Mips *mips);
};

class Return: public Instruction {
    Location *val;
public:
    Return(Location *val);
    void generateSpecific(Mips *mips);
};

class PushParam: public Instruction {
    Location *param;
public:
    PushParam(Location *param);
    void generateSpecific(Mips *mips);
};

class PopParams: public Instruction {
    int numBytes;
public:
    PopParams(int numBytesOfParamsToRemove);
    void generateSpecific(Mips *mips);
};

class LCall: public Instruction {
    const char *label;
    Location *dst;
public:
    LCall(const char *labe, Location *result);
    void generateSpecific(Mips *mips);
};

class ACall: public Instruction {
    Location *dst, *methodAddr;
public:
    ACall(Location *meth, Location *result);
    void generateSpecific(Mips *mips);
};

class VTable: public Instruction {
    List<const char *> *methodLabels;
    const char *label;
public:
    VTable(const char *labelForTable, List<const char *> *methodLabels);
    void Print();
    void generateSpecific(Mips *mips);
};

#endif // TAC_H_
