/**
 * @file mips.cpp
 * @brief implementation of mips generator
 *
 * @details
 *  detailed introduction
 *
 * @date 2018-06-13
 * @author Sixu Hu
**/

#include "mips.h"
#include <stdarg.h>
#include <cstring>


static bool LocationsAreSame(Location *var1, Location *var2) {
    // excactly the same location or same name & segment & offset
    return (var1 == var2 ||
                    (var1 && var2
                            && !strcmp(var1->getName(), var2->getName())
                            && var1->GetSegment()  == var2->GetSegment()
                            && var1->GetOffset() == var2->GetOffset()));
}

void Mips::generateDiscardValue(Location *dst) {
    // last use of value dst.
    rd = (Register) regPickRegForVar(dst, false);
    generate("%20s# Last use of %s. Discard register %s",
            "", dst->getName(), regs[rd].name);
    regs[rd].canDiscard = true;
}

void Mips::regDescriptorInsert(Location *varLoc, Register reg) {
    // if location is already in the table
    if (regDescriptorLookupRegisterForVar(varLoc) != -1) {
        regDescriptorUpdateRegister(varLoc, reg); // assume we want to update
        return;
    }
    if (regDescriptorGetRegContents(reg) != NULL) {
        printf("#Dbg: register already in use\n");
    }
    if (regs[reg].isDirty) {
        printf("#Dbg: Error, register is dirty.\n");
        return;
    }
    registerDescriptor.insert(std::pair<Register, Location *>(reg, varLoc));
    regs[reg].isDirty = true;
}

void Mips::regDescriptorRemove(Location *varLoc, Register reg) {
    // value is not in descriptor
    if (regDescriptorLookupRegisterForVar(varLoc) == -1)
        return;

    if (!regs[reg].isDirty) {
        // we might not need this, other things might be marking it as clean.
        printf("#Dbg: Error, register is not dirty.\n");
        return;
    }
    regs[reg].isDirty = false;
    std::map<Register, Location *>::iterator iter = regDescriptorlookupIterForReg(varLoc);
    registerDescriptor.erase(iter);
}

void Mips::regDescriptorUpdateRegister(Location *varLoc, Register reg) {
    std::map<Register, Location *>::iterator iter = regDescriptorlookupIterForReg(varLoc);
    iter->second = varLoc;
}

int Mips::regDescriptorLookupRegisterForVar(Location *varLoc) {
    std::map<Register, Location *>::iterator iter;
    Register selectedRegsiter = (Register)zero; // just to initialize it.
    int found_count = 0;
    for (iter = registerDescriptor.begin(); iter != registerDescriptor.end(); ++iter) {
        if (LocationsAreSame(varLoc, iter->second)) {
            found_count++;
            selectedRegsiter = iter->first;
        }
    }
    if (found_count == 1) {
        return selectedRegsiter;
    } else if (found_count == 0) {
        // not found, return -1
        return -1;
        // check the FPU ?
    } else {
        printf("Dbg: Error, we found multiple copies of varLoc.\n");
        return -10; // arbitrary, unused for now.
    }
}

Location *Mips::regDescriptorGetRegContents(Register reg) {
    std::map<Register, Location *>::iterator iter;
    iter = registerDescriptor.find(reg);
    if (iter == registerDescriptor.end()) {
        return (Location *) NULL;
    } else {
        return iter->second;
    }
}

std::map<Mips::Register, Location *>::iterator Mips::regDescriptorlookupIterForReg(Location *varLoc) {
    std::map<Register, Location *>::iterator iter;
    std::map<Register, Location *>::iterator iterToReturn;
    int found_count = 0;

    for (iter = registerDescriptor.begin(); iter != registerDescriptor.end(); ++iter) {
        if (LocationsAreSame(varLoc, iter->second)) {
            found_count++;
            iterToReturn = iter;
        }
    }
    if (found_count == 1) {
        return iterToReturn;
    } else if (found_count == 0) {
        // not found, return -1
        return registerDescriptor.end();
    } else {
        printf("Dbg: Error, we found multiple copies of varLoc.\n");
        return registerDescriptor.end();
    }
}

void Mips::fillRegister(Location *src, Register reg) {
    ASSERT(src);
    const char *offsetFromWhere = src->GetSegment() == fpRelative ? regs[fp].name : regs[gp].name;
    ASSERT(src->GetOffset() % 4 == 0); // all variables are 4 bytes in size

    Register previousReg = (Register) regDescriptorLookupRegisterForVar(src);
    if (reg > 31) {  // if filling to the fpu
        if ((int) previousReg == -1) { // if src is not already in a register
            // Load directly to coprocessor from ram
            // FregDescriptorInsert();
            generate("l.s %s, %d(%s)\t# fill %s to %s from %s%+d", regs[reg].name,
                    src->GetOffset(), offsetFromWhere, src->getName(), regs[reg].name,
                    offsetFromWhere, src->GetOffset());
        } else if (previousReg < 31) { // if src is in a regular register
            // Move to coprocessor:  mtcz Rsrc, CPdest
            // FregDescriptorInsert();
            generate("mtc1 %s, %s\t\t# move %s to %s", regs[previousReg].name, regs[reg].name,
                    regs[previousReg].name, regs[reg].name);
        } else { // src is already in coprocessor
            if (previousReg == reg) { // if src in this register already
                // do nothing
            } else { // src is in different coprocessor reg
                // move from coprocessor reg to coprocessor reg
                generate("mov.s %s, %s\t\t# move %s to %s", regs[rd].name, regs[f0].name,
                        regs[f0].name, regs[rd].name);
            }
        }
    } else { // filling to regular registers
        if ((int) previousReg == -1) { // src is not already in a register, load from ram
            generate("lw %s, %d(%s)\t# fill %s to %s from %s%+d", regs[reg].name,
                    src->GetOffset(), offsetFromWhere, src->getName(), regs[reg].name,
                    offsetFromWhere, src->GetOffset());
        } else if (reg != previousReg) { // if src in a different register
            if (previousReg > 31) { // src in coprocessor
                // mfcz Rdest, CPsrc
                generate("mfc1 %s, %s\t\t# move %s to %s", regs[reg].name, regs[previousReg].name,
                        regs[previousReg].name, regs[reg].name);
            } else { // src in regular register
                Location *previousLoc = regDescriptorGetRegContents(previousReg);
                //Register copyFrom = (Register) regPickRegForVar(previousLoc, false);
                generate("move %s, %s\t# move (fill copy) %s from %s to %s in %s", regs[reg].name, regs[previousReg].name,
                        previousLoc->getName(), regs[previousReg].name, src->getName(), regs[reg].name);
            }
        } else if (reg == previousReg) { // if already in correct register
            // do nothing
        }
    }
}

void Mips::spillRegister(Location *dst, Register reg) {
    ASSERT(dst);
    const char *offsetFromWhere = dst->GetSegment() == fpRelative ? regs[fp].name : regs[gp].name;
    ASSERT(dst->GetOffset() % 4 == 0); // all variables are 4 bytes in size

    generate("sw %s, %d(%s)\t# spill %s from %s to %s%+d", regs[reg].name,
            dst->GetOffset(), offsetFromWhere, dst->getName(), regs[reg].name,
            offsetFromWhere, dst->GetOffset());
    regDescriptorRemove(dst, reg);
    regs[reg].isDirty = false; // double assertion.
}

void Mips::discardValueInRegister(Location *dst, Register reg) {
    if (regs[reg].canDiscard == true) {
        ASSERT(dst);
        const char *offsetFromWhere = dst->GetSegment() == fpRelative ? regs[fp].name : regs[gp].name;
        // variables are 4 bytes
        ASSERT(dst->GetOffset() % 4 == 0);

        regDescriptorRemove(dst, reg);
        regs[reg].isDirty = false; // double assertion.
        regs[reg].canDiscard = false; // double assertion.
    }
}

int Mips::regPickRegForVar(Location *varLoc, bool copyRequired) {
    // if value is already in registers, keep it there.
    // a new register is needed, doesn't matter if already in regs.
    if (copyRequired == false) {
        if (regDescriptorLookupRegisterForVar(varLoc) != -1) {
            return regDescriptorLookupRegisterForVar(varLoc);
        } else {
            return regIndexOfNextClean();
        }
    } else {
        return regIndexOfNextClean();
    }
}

int Mips::regIndexOfNextClean() {
    int index = -1;

    // find a clean register
    for (int i = t0; i <= t9; i++) {
        if (regs[i].isDirty == false) {
            index = i;
            break;
        }
    }

    // if none, clean one randomlly
    if (index == -1) {
        // select a random (unlocked) register and clean it.
        index = regSelectRandom();
        regCleanRegister((Register)index);
    }

    return index;
}

int Mips::regSelectRandom() {
    bool done = false;
    int selection;
    int safety_counter = 0;
    while (!done) {
        // gen random int from 0-10 because t0-t9
        selection = rand() % 10 + 16;
        if (regs[selection].mutexLocked == false) {
            if (regs[selection].isDirty == true) {
                done = true;
            }
        }
        // just in case everything is locked...
        safety_counter++;
        if (safety_counter >= 100) {
            selection = -100;
            break;
        }
    }
    return selection;
}

void Mips::regCleanRegister(Register reg) {
    Location *varLoc = regDescriptorGetRegContents(reg);
    if (varLoc == NULL)
        return;
    spillRegister(varLoc, reg);
}

void Mips::regCleanForBranch() {
    for (int i = t0; i <= t9; i++) {
        if (regs[i].isDirty == true) {
            regCleanRegister((Register)i);
        }
    }
    registerDescriptor.clear();
}

void Mips::generate(const char *fmt, ...) {
    va_list args;
    char buf[1024];

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    // do not indent label
    if (buf[strlen(buf) - 1] != ':') {
        if (buf[0] != '#')
            printf("    ");
        else
            printf("%24s", "");
    }
    printf("%s", buf);
    if (buf[strlen(buf) - 1] != '\n')
        printf("\n"); // end with a newline
}

void Mips::generateLoadConstant(Location *dst, int val) {
    rd = (Register) regPickRegForVar(dst, false);
    regs[rd].mutexLocked = true;

    //fillRegister(dst,rd);
    generate("li %s, %d\t\t# load constant value %d into %s", regs[rd].name,
            val, val, regs[rd].name);
    regDescriptorInsert(dst, rd);
    regs[rd].mutexLocked = false;
}

void Mips::generateLoadStringConstant(Location *dst, const char *str) {
    static int strNum = 1;
    char label[16];
    sprintf(label, "_string%d", strNum++);
    generate(".data\t\t# create string constant marked with label");
    generate("%s: .asciiz %s", label, str);
    generate(".text");
    generateLoadLabel(dst, label);
}

void Mips::generateLoadLabel(Location *dst, const char *label) {
    rd = (Register) regPickRegForVar(dst, false);
    regs[rd].mutexLocked = true;
    //fillRegister(dst,rd);
    generate("la %s, %s\t# load label", regs[rd].name, label);
    regDescriptorInsert(dst, rd);
    regs[rd].mutexLocked = false;
}

void Mips::generateCopy(Location *dst, Location *src) {

    rs = (Register) regPickRegForVar(src, false);
    regs[rs].mutexLocked = true;
    fillRegister(src, rs);
    regDescriptorInsert(src, rs);

    // If rs will never be used again, send it to a black hold.
    if (regs[rs].canDiscard == true) {
        discardValueInRegister(src, rs);
    }

    rd = (Register) regPickRegForVar(dst, false);
    regs[rd].mutexLocked = true;

    generate("move %s, %s\t# move (copy) %s from %s to %s in %s", regs[rd].name, regs[rs].name,
            src->getName(), regs[rs].name, dst->getName(), regs[rd].name);

    regDescriptorInsert(dst, rd);
    regs[rs].mutexLocked = false;
    regs[rd].mutexLocked = false;
}

void Mips::generateLoad(Location *dst, Location *reference, int offset) {
    rs = (Register) regPickRegForVar(reference, false);
    regs[rs].mutexLocked = true;
    fillRegister(reference, rs);
    regDescriptorInsert(reference, rs);

    rd = (Register) regPickRegForVar(dst, false);
    regs[rd].mutexLocked = true;


    generate("lw %s, %d(%s) \t# load with offset", regs[rd].name,
            offset, regs[rs].name);

    regDescriptorInsert(dst, rd);

    regCleanRegister(rs);
    regs[rs].mutexLocked = false;
    regs[rd].mutexLocked = false;
}

void Mips::generateStore(Location *reference, Location *value, int offset) {
    rs = (Register) regPickRegForVar(value, false);
    regs[rs].mutexLocked = true;
    fillRegister(value, rs);
    regDescriptorInsert(value, rs);


    rd = (Register) regPickRegForVar(reference, false);
    regs[rd].mutexLocked = true;
    fillRegister(reference, rd);
    regDescriptorInsert(reference, rd);

    generate("sw %s, %d(%s) \t# store with offset",
            regs[rs].name, offset, regs[rd].name);

    regs[rd].mutexLocked = false;
    regs[rs].mutexLocked = false;
}

void Mips::generateBinaryOp(BinaryOp::OpCode code, Location *dst,
        Location *op1, Location *op2) {
    const char *operation = tacToMips(code);
    if (!strcmp(operation, "disable fpu")) {
        FP_generateBinaryOp("add.s", dst, op1, op2);
    } else {
        rs = (Register) regPickRegForVar(op1, false);
        regs[rs].mutexLocked = true;
        fillRegister(op1, rs);
        regDescriptorInsert(op1, rs);

        rt = (Register) regPickRegForVar(op2, false);
        regs[rt].mutexLocked = true;
        fillRegister(op2, rt);
        regDescriptorInsert(op2, rt);

        rd = (Register) regPickRegForVar(dst, false);
        regs[rd].mutexLocked = true;

        generate("%s %s, %s, %s\t", tacToMips(code), regs[rd].name,
                regs[rs].name, regs[rt].name);
        regDescriptorInsert(dst, rd);
        regs[rd].canDiscard = true;
        regs[rs].mutexLocked = false;
        regs[rt].mutexLocked = false;
        regs[rd].mutexLocked = false;
    }
}

void Mips::FP_generateBinaryOp(const char *operation, Location *dst,
        Location *op1, Location *op2) {
    //fpu rs~f2, rt~f4, rd~f0 , fpu uses even regs.
    rd = (Register) regPickRegForVar(dst, false);
    regs[rd].mutexLocked = true;
    regs[f0].mutexLocked = true;
    regs[f2].mutexLocked = true;
    regs[f4].mutexLocked = true;
    fillRegister(op1, f2);
    fillRegister(op2, f4);

    generate("%s %s, %s, %s\t", operation, regs[f0].name,
            regs[f2].name, regs[f4].name);

    generate("mfc1 %s, %s\t\t# move %s to %s", regs[rd].name, regs[f0].name,
            regs[f0].name, regs[rd].name);
    regDescriptorInsert(dst, rd);
    regs[rd].mutexLocked = false;
    regs[f0].mutexLocked = false;
    regs[f2].mutexLocked = false;
    regs[f4].mutexLocked = false;
}

void Mips::generateLabel(const char *label) {

    regCleanForBranch();
    generate("%s:", label);
}

void Mips::generateGoto(const char *label) {

    regCleanForBranch();
    generate("b %s\t\t# unconditional branch", label);
}

void Mips::generateIfZ(Location *test, const char *label) {
    fillRegister(test, v0);
    regCleanForBranch();
    generate("beqz %s, %s\t# branch if %s is zero ", regs[v0].name, label,
            test->getName());
}

void Mips::generateParam(Location *arg) {

    rs = (Register) regPickRegForVar(arg, false);
    regs[rs].mutexLocked = true;

    fillRegister(arg, rs);
    generate("subu $sp, $sp, 4\t# decrement sp to make space for param");
    generate("sw %s, 4($sp)\t# copy param value to stack", regs[rs].name);
    //regDescriptorRemove(arg, rs);
    regs[rs].mutexLocked = false;
}

void Mips::generateCallInstr(Location *result, const char *fn, bool isLabel) {
    regCleanForBranch();
    if (result != NULL) {
        rd = (Register) regPickRegForVar(result, false);
        regs[rd].mutexLocked = true;
        generate("%s %-15s\t# jump to function", isLabel ? "jal" : "jalr", fn);
        generate("move %s, %s\t# copy function return value from $v0",
                regs[rd].name, regs[v0].name);
        regDescriptorInsert(result, rd);
        regs[rd].mutexLocked = false;
    } else {
        generate("%s %-15s\t# jump to function", isLabel ? "jal" : "jalr", fn);
    }
}


void Mips::generateLCall(Location *dst, const char *label) {
    generateCallInstr(dst, label, true);
}

void Mips::generateACall(Location *dst, Location *fn) {
    fillRegister(fn, v0);
    generateCallInstr(dst, regs[v0].name, false);
}

void Mips::generatePopParams(int bytes) {
    if (bytes != 0)
        generate("add $sp, $sp, %d\t# pop params off stack", bytes);
}

void Mips::generateReturn(Location *returnVal) {
    if (returnVal != NULL)
        fillRegister(returnVal, v0);
    generate("move $sp, $fp\t# pop callee frame off stack");
    generate("lw $ra, -4($fp)\t# restore saved ra");
    generate("lw $fp, 0($fp)\t# restore saved fp");
    generate("jr $ra\t\t# return from function");
}

void Mips::generateBeginFunction(int stackFrameSize) {
    ASSERT(stackFrameSize >= 0);
    generate("subu $sp, $sp, 8\t# decrement sp to make space to save ra, fp");
    generate("sw $fp, 8($sp)\t# save fp");
    generate("sw $ra, 4($sp)\t# save ra");
    generate("addiu $fp, $sp, 8\t# set up new fp");

    if (stackFrameSize != 0)
        generate("subu $sp, $sp, %d\t# decrement sp to make space for locals/temps",
                stackFrameSize);
}

void Mips::generateEndFunction() {
    generate("# (below handles reaching end of fn body with no explicit return)");
    generateReturn(NULL);
}

void Mips::generateVTable(const char *label, List<const char *> *methodLabels) {
    generate(".data");
    generate(".align 2");
    generate("%s:\t\t# label for class %s vtable", label, label);
    for (int i = 0; i < methodLabels->NumElements(); i++)
        generate(".word %s\n", methodLabels->Nth(i));
    generate(".text");
}

void Mips::generateHeader() {
    generate("# standard Decaf preamble ");
    generate(".text");
    //generate(".align 2");
    generate(".globl main");
    generate("jal main");
    generate("li $v0, 10");
    generate("syscall");
}

const char *Mips::tacToMips(BinaryOp::OpCode code) {
    ASSERT(code >= 0 && code < BinaryOp::NumOps);
    const char *name = mipsName[code];
    ASSERT(name != NULL);
    return name;
}

Mips::Mips() {
    mipsName[BinaryOp::Add] = "add";
    mipsName[BinaryOp::Sub] = "sub";
    mipsName[BinaryOp::Mul] = "mul";
    mipsName[BinaryOp::Div] = "div";
    mipsName[BinaryOp::Mod] = "rem";
    mipsName[BinaryOp::Eq]  = "seq";
    mipsName[BinaryOp::Less] = "slt";
    mipsName[BinaryOp::And] = "and";
    mipsName[BinaryOp::Or]  = "or";

    regs[zero] = (RegContents) {
        false, NULL, "$zero", false, false, false
    };
    regs[at] = (RegContents) {
        false, NULL, "$at", false, false, false
    };
    regs[v0] = (RegContents) {
        false, NULL, "$v0", false, false, false
    };
    regs[v1] = (RegContents) {
        false, NULL, "$v1", false, false, false
    };
    regs[a0] = (RegContents) {
        false, NULL, "$a0", false, false, false
    };
    regs[a1] = (RegContents) {
        false, NULL, "$a1", false, false, false
    };
    regs[a2] = (RegContents) {
        false, NULL, "$a2", false, false, false
    };
    regs[a3] = (RegContents) {
        false, NULL, "$a3", false, false, false
    };
    regs[k0] = (RegContents) {
        false, NULL, "$k0", false, false, false
    };
    regs[k1] = (RegContents) {
        false, NULL, "$k1", false, false, false
    };
    regs[gp] = (RegContents) {
        false, NULL, "$gp", false, false, false
    };
    regs[sp] = (RegContents) {
        false, NULL, "$sp", false, false, false
    };
    regs[fp] = (RegContents) {
        false, NULL, "$fp", false, false, false
    };
    regs[ra] = (RegContents) {
        false, NULL, "$ra", false, false, false
    };

    regs[t0] = (RegContents) {
        false, NULL, "$t0", true, false, false
    };
    regs[t1] = (RegContents) {
        false, NULL, "$t1", true, false, false
    };
    regs[t2] = (RegContents) {
        false, NULL, "$t2", true, false, false
    };
    regs[t3] = (RegContents) {
        false, NULL, "$t3", true, false, false
    };
    regs[t4] = (RegContents) {
        false, NULL, "$t4", true, false, false
    };
    regs[t5] = (RegContents) {
        false, NULL, "$t5", true, false, false
    };
    regs[t6] = (RegContents) {
        false, NULL, "$t6", true, false, false
    };
    regs[t7] = (RegContents) {
        false, NULL, "$t7", true, false, false
    };
    regs[t8] = (RegContents) {
        false, NULL, "$t8", true, false, false
    };
    regs[t9] = (RegContents) {
        false, NULL, "$t9", true, false, false
    };
    regs[s0] = (RegContents) {
        false, NULL, "$s0", true, false, false
    };
    regs[s1] = (RegContents) {
        false, NULL, "$s1", true, false, false
    };
    regs[s2] = (RegContents) {
        false, NULL, "$s2", true, false, false
    };
    regs[s3] = (RegContents) {
        false, NULL, "$s3", true, false, false
    };
    regs[s4] = (RegContents) {
        false, NULL, "$s4", true, false, false
    };
    regs[s5] = (RegContents) {
        false, NULL, "$s5", true, false, false
    };
    regs[s6] = (RegContents) {
        false, NULL, "$s6", true, false, false
    };
    regs[s7] = (RegContents) {
        false, NULL, "$s7", true, false, false
    };
    //  rs = t, false0; rt = t1; rd = t2;

    // floating point registers (for fun)
    regs[f0]  = (RegContents) {
        false, NULL, "$f0", true, false, false
    };
    regs[f1]  = (RegContents) {
        false, NULL, "$f1", true, false, false
    };
    regs[f2]  = (RegContents) {
        false, NULL, "$f2", true, false, false
    };
    regs[f3]  = (RegContents) {
        false, NULL, "$f3", true, false, false
    };
    regs[f4]  = (RegContents) {
        false, NULL, "$f4", true, false, false
    };
    regs[f5]  = (RegContents) {
        false, NULL, "$f5", true, false, false
    };
    regs[f6]  = (RegContents) {
        false, NULL, "$f6", true, false, false
    };
    regs[f7]  = (RegContents) {
        false, NULL, "$f7", true, false, false
    };
    regs[f8]  = (RegContents) {
        false, NULL, "$f8", true, false, false
    };
    regs[f9]  = (RegContents) {
        false, NULL, "$f9", true, false, false
    };
    regs[f10] = (RegContents) {
        false, NULL, "$f10", true, false, false
    };
    regs[f11] = (RegContents) {
        false, NULL, "$f11", true, false, false
    };
    regs[f12] = (RegContents) {
        false, NULL, "$f12", true, false, false
    };
    regs[f13] = (RegContents) {
        false, NULL, "$f13", true, false, false
    };
    regs[f14] = (RegContents) {
        false, NULL, "$f14", true, false, false
    };
    regs[f15] = (RegContents) {
        false, NULL, "$f15", true, false, false
    };
    regs[f16] = (RegContents) {
        false, NULL, "$f16", true, false, false
    };
    regs[f17] = (RegContents) {
        false, NULL, "$f17", true, false, false
    };
    regs[f18] = (RegContents) {
        false, NULL, "$f18", true, false, false
    };
    regs[f19] = (RegContents) {
        false, NULL, "$f19", true, false, false
    };
    regs[f20] = (RegContents) {
        false, NULL, "$f20", true, false, false
    };
    regs[f21] = (RegContents) {
        false, NULL, "$f21", true, false, false
    };
    regs[f22] = (RegContents) {
        false, NULL, "$f22", true, false, false
    };
    regs[f23] = (RegContents) {
        false, NULL, "$f23", true, false, false
    };
    regs[f24] = (RegContents) {
        false, NULL, "$f24", true, false, false
    };
    regs[f25] = (RegContents) {
        false, NULL, "$f25", true, false, false
    };
    regs[f26] = (RegContents) {
        false, NULL, "$f26", true, false, false
    };
    regs[f27] = (RegContents) {
        false, NULL, "$f27", true, false, false
    };
    regs[f28] = (RegContents) {
        false, NULL, "$f28", true, false, false
    };
    regs[f29] = (RegContents) {
        false, NULL, "$f29", true, false, false
    };
    regs[f30] = (RegContents) {
        false, NULL, "$f30", true, false, false
    };
    regs[f31] = (RegContents) {
        false, NULL, "$f31", true, false, false
    };
}
const char *Mips::mipsName[BinaryOp::NumOps];
