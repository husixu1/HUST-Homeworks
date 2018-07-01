/**
 * @file cfg.cpp
 * @brief control-flow graph implementation
 *
 * @date 2018-06-14
 * @author Sixu Hu
**/

#include "flow.h"
#include "tac.h"

ControlFlowGraph::ControlFlowGraph(iterator first, iterator last)
    : first( first ), last( last ) {
    mapLabels();
    mapEdges();
}

void ControlFlowGraph::mapLabels() {
    for (iterator p = first; p != last; ++p) {
        Label *label = dynamic_cast<Label *>(*p);
        if (label) {
            labelInstructionMap[label->text()] = *p;
        }
    }
}

void ControlFlowGraph::mapEdges() {
    std::string jumpType;
    for (iterator cur = first; cur != last; ++cur) {
        iterator next = cur;
        ++next;

        if (dynamic_cast<Return *>(*cur)) {
            next = first;
            addEdge(*cur, *next);
        } else if (dynamic_cast<LCall *>(*cur)) {
            jumpType = "LCall";
            mapEdgesForJump(cur, jumpType);
        } else if (dynamic_cast<ACall *>(*cur)) {
            jumpType = "ACall";
            mapEdgesForJump(cur, jumpType);
        } else if (dynamic_cast<IfZ *>(*cur)) {
            jumpType = "IfZ";
            mapEdgesForJump(cur, jumpType);
        } else if (dynamic_cast<Goto *>(*cur)) {
            jumpType = "Goto";
            mapEdgesForJump(cur, jumpType);
        }
    }
}

void ControlFlowGraph::mapEdgesForJump(iterator cur, std::string jumpType) {
    int jumpInstrLength = jumpType.length() + 1;
    std::string labelString = (*cur)->getPrinted();
    size_t positionToRemove = labelString.find(jumpType);
    labelString.erase(0, positionToRemove + jumpInstrLength);
    Instruction *instruction;

    if (labelInstructionMap.find(labelString) != labelInstructionMap.end()) {
        // if label is in this block
        instruction = labelInstructionMap.at(labelString);
        addEdge(*cur, instruction);
    } else {
        //label is in another block, come back to it.
        if (strcmp(labelString.c_str(), "_PrintString") == 0 ||
                strcmp(labelString.c_str(), "_PrintInt") == 0) {
            return;
        }
        addEdge(*cur, *cur);
    }
}

void ControlFlowGraph::addEdge(Instruction *from, Instruction *to) {
    inEdges[to].push_back(from);
    outEdges[from].push_back(to);
}
