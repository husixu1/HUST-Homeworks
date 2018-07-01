/**
 * @file ast_printer.h
 * @brief print the ast tree
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/
#ifndef AST_PRINTER_H_
#define AST_PRINTER_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

/// @brief the base class of all printers
class Printer {
protected:
    /// @brief the file to print to
    std::ofstream outFile;

public:
    /// @brief print information without endline
    /// @param info the information to print
    virtual void print(std::string info);

    /// @brief alias for print(std::string info)
    /// @note subclass only needs to override print(std::stirng info) if needed.
    virtual void print(const char *info);

    /// @brief set the file to print, close opened file
    /// @param file the file to print
    void openFile(std::string file);

    /// @brief close the opened file
    void closeFile();
};

class LexPrinter : public Printer {
public:
    /// @brief get the global default printer
    /// @return the default printer
    static LexPrinter &getDefaultPrinter();

    /// @brief print token info
    /// @param token the token value
    /// @param type the token type
    virtual void printToken(const char *token, const char *type);
};

class AstPrinter : public Printer {
private:
    /// @brief print current node prefix to printFile, assume printFile opened
    void printPrefix();
public:
    /// @brief printHelper stack to help print the tree hierarchy
    std::vector<int> printHelper;

    /// @brief print prefix + info
    /// @param info the info to print
    void print(std::string info) override;

    /// @brief get the default ast printer
    /// @return the default printer
    static AstPrinter &getDefaultPrinter();
};

class ScopePrinter : public Printer {
private:
    /// @brief current print level
    int level;
    /// @brief the maximum level reached
    int maxLevel;

public:
    ScopePrinter() : level(1), maxLevel(1) {};

    /// @brief print Scope Tail information
    void printTail();

    /// @brief print prefix + info
    /// @param info the identifier to print
    void print(std::string info) override;

    /// @brief print the start of the scope
    /// @param info the info to add
    void printScopeStart(const std::string &info);

    /// @brief get the default scope printer
    /// @return the default printer
    static ScopePrinter &getDefaultPrinter();

    /// @brief setter for level
    void setLevel(int level);
    /// @brief getter for level
    int getLevel();
    /// @brief increase level by 1
    void levelIncrease();
    /// @brief decrease level by 1
    void levelDecrease();
};

class TacPrinter : public Printer {
public:
    /// @brief print tac instruction
    /// @param instruction the instruction to print
    void print(std::string instruction) override;

    /// @brief get the default tac printer
    /// @return the default printer
    static TacPrinter &getDefaultPrinter();
};

#endif // AST_PRINTER_H_
