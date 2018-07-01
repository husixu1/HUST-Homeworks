/**
 * @file scope.h
 * @brief scope management class
 *
 * @date 2018-06-12
 * @author Sixu Hu
**/

#ifndef SCOPE_H_
#define SCOPE_H_

#include <fstream>
#include <string>
#include "hash.h"

class Decl;
class Identifier;
class ClassDecl;

class Scope {
protected:
    /// @brief the hash table that maps identifiers to declares
    Hashtable<Decl *> *table;

    /// @brief the file to print to
    static std::fstream *printFile;

public:
    /// @brief scope constructor
    /// @param info info of the base(class/function/etc) of the scope,
    /// used for printing, empty string for global scope
    Scope(const std::string &info = std::string());

    Decl *find(Identifier *id);

    /// @brief add decl to this scope
    /// @param decl the declaration to add
    /// @return if declaration is added successfully
    bool Declare(Decl *decl);

    /// @brief copy other scope to class declaration's scope
    ///
    /// @param other
    /// @param cd
    void CopyFromScope(Scope *other, ClassDecl *cd);
};


#endif // SCOPE_H_
