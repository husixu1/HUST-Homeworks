/**
 * @file flow.h
 * @brief control-flow graph management classes
 *
 * @date 2018-06-14
 * @author Sixu Hu
**/
#ifndef CFG_H_
#define CFG_H_


#include <list>
#include <map>
#include <string>
#include <iostream>
#include <vector>

class Instruction;
typedef std::vector<Instruction *> EdgeList;
typedef std::list<Instruction *> Block;

/// @brief a flow graph for a function, creates a map of edges between instructions
class ControlFlowGraph {
public:
    /// @brief list of instruction
    typedef std::list<Instruction *>::iterator iterator;

    /// @brief constrcuctor
    /// @param first
    /// @param last
    ControlFlowGraph(iterator first, iterator last);

    /// @brief foward flow and reverse flow class
    class ForwardFlow;
    class ReverseFlow;

private:

    /// @brief map labels to instructions
    void mapLabels();
    /// @brief map instructions to input edge and output edge
    void mapEdges();
    /// @brief add an edge from instruction to instruction
    /// @param from the source instruction
    /// @param to the destination instruction
    void addEdge(Instruction *from, Instruction *to);
    /// @brief map jump instruction to a jump destination
    /// @param cur current instruction iterator
    /// @param jumpType jump type
    void mapEdgesForJump(iterator cur, std::string jumpType);

    /// @brief map labels to instructions
    std::map<std::string, Instruction *> labelInstructionMap;
    /// @brief in edges map
    std::map<Instruction *, EdgeList> inEdges;
    /// @brief out edges map
    std::map<Instruction *, EdgeList> outEdges;

    iterator first;
    iterator last;
};

/// @brief traverse cfg in forward direction
class ControlFlowGraph::ForwardFlow {
public:
    /// @brief iterator
    typedef std::list<Instruction *>::const_iterator iterator;

    /// @brief constructor
    /// @param cfg the cfg to traverse
    ForwardFlow( ControlFlowGraph &cfg ) : cfg( cfg ) {}

    /// @brief return the first instruction iterator
    iterator first() {
        return iterator( cfg.first );
    }

    /// @brief return the last instruction iterator
    iterator last()  {
        return iterator( cfg.last );
    }

    /// @brief return edges in forward direction
    std::map<Instruction *, EdgeList> &in() {
        return cfg.inEdges;
    }

    /// @brief return edges in backward direction
    std::map<Instruction *, EdgeList> &out() {
        return cfg.outEdges;
    }

private:
    ControlFlowGraph &cfg;
};

/// @brief traverse cfg in backward direction
class ControlFlowGraph::ReverseFlow {
public:
    /// @brief iterator
    typedef std::list<Instruction *>::const_reverse_iterator iterator;

    /// @brief constructor
    /// @param cfg the cfg to traverse
    ReverseFlow( ControlFlowGraph &cfg ) : cfg( cfg ) {}

    /// @brief return the first instruction iterator
    iterator first() {
        return iterator( cfg.last );
    }
    /// @brief return the last instruction iterator
    iterator last()  {
        return iterator( cfg.first );
    }

    /// @brief return edges in backward direction
    std::map<Instruction *, EdgeList> &in() {
        return cfg.outEdges;
    }
    /// @brief return edges in foward direction
    std::map<Instruction *, EdgeList> &out() {
        return cfg.inEdges;
    }

private:
    ControlFlowGraph &cfg;
};

#endif // CFG_H_
