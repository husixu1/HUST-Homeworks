/**
 * @file ast.h
 * @brief Defines the base class of all ast nodes
 *
 * @details
 * Each ast node contains a location and parent, location is used for
 * debugging and reporting errors, parent pointer pointes to the parent node,
 * each node has a parent node except for the Program node
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#ifndef AST_H_
#define AST_H_


#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "list.h"
#include "utility.h"

class Scope;
class Decl;
class Identifier;
class Type;
class CodeGenerator;

/// @brief base class of all ast nodes
class AstNode {
protected:
    /// @brief location of the node
    yyltype *location;
    /// @brief parent of the node
    AstNode *parent;
    /// @brief scope of the node
    Scope *nodeScope;

public:
    AstNode(yyltype loc);
    AstNode();

    yyltype *GetLocation()   {
        return location;
    }
    void SetParent(AstNode *p)  {
        parent = p;
    }
    AstNode *GetParent()        {
        return parent;
    }
    virtual void check() {}

    /// @brief lookup strategy when finding declarations (shallow or recursive)
    typedef enum { kShallow, kDeep } lookup;

    /// @brief find declaration for identifier id
    /// @param id the id to find declaration with
    /// @param l lookup strategy
    /// @return the found declare (null for not found)
    virtual Decl *FindDecl(Identifier *id, lookup l = kDeep);

    virtual Scope *PrepareScope() {
        return NULL;
    }
    template <class Specific> Specific *FindSpecificParent() {
        AstNode *p = parent;
        while (p) {
            if (Specific *s = dynamic_cast<Specific *>(p))
                return s;
            p = p->parent;
        }
        return NULL;
    }

    /// @brief generate code for ast, converts ast to codegen object recursively
    /// that is, to make the parsed tree a linear code
    /// @param cg
    virtual void generate(CodeGenerator *cg) {}
};


/// @brief class for identifier (variable/function/class instances and types, etc)
class Identifier : public AstNode {
protected:
    /// @brief name of the identifier
    char *name;

    /// @brief cached declaration of the identifier
    Decl *cached;

public:
    /// @brief constructor
    /// @param loc location of the identifier
    /// @param name name of the identifier
    Identifier(yyltype loc, const char *name);

    Decl *GetDeclRelativeToBase(Type *base = NULL);

    const char *getName() {
        return name;
    }
    friend std::ostream &operator<<(std::ostream &out, Identifier *id) {
        return out << id->name;
    }
};


// This node class is designed to represent a portion of the tree that
// encountered syntax errors during parsing. The partial completed tree
// is discarded along with the states being popped, and an instance of
// the Error class can stand in as the placeholder in the parse tree
// when your parser can continue after an error.
class Error : public AstNode {
public:
    Error() : AstNode() {}
};


#endif // AST_H_
