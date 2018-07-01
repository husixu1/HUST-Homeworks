#ifndef LIST_H_
#define LIST_H_

#include <deque>
#include "utility.h"
#include "scope.h"
#include "printer.h"

class AstNode;
class CodeGenerator;

template<typename Element> class List {

private:
    std::deque<Element> elems;

public:
    /// @brief constructor
    List() {}

    int NumElements() const {
        return elems.size();
    }

    // Returns element at index in list. Indexing is 0-based.
    // Raises an assert if index is out of range.
    Element Nth(int index) const {
        ASSERT(index >= 0 && index < NumElements());
        return elems[index];
    }

    // Inserts element at index, shuffling over others
    // Raises assert if index out of range
    void InsertAt(const Element &elem, int index) {
        ASSERT(index >= 0 && index <= NumElements());
        elems.insert(elems.begin() + index, elem);
    }

    // Adds element to list end
    void Append(const Element &elem) {
        elems.push_back(elem);
    }

    // Removes element at index, shuffling down others
    // Raises assert if index out of range
    void RemoveAt(int index) {
        ASSERT(index >= 0 && index < NumElements());
        elems.erase(elems.begin() + index);
    }

    // These are some specific methods useful for lists of ast nodes
    // They will only work on lists of elements that respond to the
    // messages, but since C++ only instantiates the template if you use
    // you can still have Lists of ints, chars*, as long as you
    // don't try to SetParentAll on that list.
    void SetParentAll(AstNode *p) {
        for (int i = 0; i < NumElements(); i++)
            Nth(i)->SetParent(p);
    }
    void DeclareAll(Scope *s) {
        for (int i = 0; i < NumElements(); i++)
            s->Declare(Nth(i));
    }

    void checkAll() {
        ScopePrinter &printer = ScopePrinter::getDefaultPrinter();

        printer.levelIncrease();
        for (int i = 0; i < NumElements(); i++)
            Nth(i)->check();
        printer.levelDecrease();
    }

    /// @brief generate code use cg
    /// @param cg the code generator
    /// @param terminate whether to terminate the block when printing
    void generateAll(CodeGenerator *cg, bool terminate = true) {
        AstPrinter &printer = AstPrinter::getDefaultPrinter();

        printer.printHelper.push_back(1);
        for (int i = 0; i < NumElements(); ++i){
            if(i == NumElements() - 1)
                printer.printHelper.back() = terminate ? 0 : 1;
            Nth(i)->generate(cg);
        }
        printer.printHelper.pop_back();
    }

};


#endif // LIST_H_
