#include "printer.h"
#include <iomanip>

void Printer::print(std::string info) {
    outFile << info;
}

void Printer::print(const char *info) {
    print(std::string(info));
}

void Printer::openFile(std::string fileName) {
    if (outFile.is_open())
        outFile.close();
    outFile.open(fileName);
}

void Printer::closeFile() {
    outFile.close();
}

LexPrinter &LexPrinter::getDefaultPrinter() {
    static LexPrinter printer;
    return printer;
}

void LexPrinter::printToken(const char *token, const char *type) {
    using namespace std;
    outFile << right << setw(20) << token << " : " << left << setw(20)  << type << endl;
}

void AstPrinter::printPrefix() {
    int i = 0;
    for (; i < static_cast<int>(printHelper.size()) - 1; ++i)
        outFile << (printHelper[i] ? "│  " : "   ");
    if (i < printHelper.size())
        outFile << (printHelper[i] ? "├─ " : "└─ ");
}

void AstPrinter::print(std::string info) {
    using namespace std;
    printPrefix();
    outFile << info << endl;
}

AstPrinter &AstPrinter::getDefaultPrinter() {
    static AstPrinter printer;
    return printer;
}

ScopePrinter &ScopePrinter::getDefaultPrinter() {
    static ScopePrinter printer;
    return printer;
}

void ScopePrinter::printTail() {
    using namespace std;
    outFile << "[92m  ";
    for(int i=0; i < maxLevel - 1; ++i)
        outFile << "└── ";
    outFile << endl << "  ";
    for(int i=0; i < maxLevel - 1; ++i)
        outFile << " " << left << setw(3)  << i;
    outFile << " <-- Scope level[0m" << endl;
}

void ScopePrinter::print(std::string info) {
    for (int i = 0; i < level; ++i)
        outFile << "  │ ";
    outFile << info << std::endl;
}

void ScopePrinter::printScopeStart(const std::string &info) {
    using namespace std;
    for (int i = 0; i < level - 1; ++i)
        outFile << "  │ ";
    outFile << "  ├─ [93m"
        << left << (info.empty() ? "[Global]" : info) << "[0m" << endl;
}

void ScopePrinter::setLevel(int level) {
    this->level = level;
    maxLevel = (level > maxLevel ? level : maxLevel);
}

int ScopePrinter::getLevel() {
    return level;
}

void ScopePrinter::levelIncrease() {
    ++level;
    maxLevel = (level > maxLevel ? level : maxLevel);
}

void ScopePrinter::levelDecrease() {
    --level;
}

void TacPrinter::print(const std::string instruction){
    using namespace std;
    outFile << instruction << endl;
}

TacPrinter &TacPrinter::getDefaultPrinter(){
    static TacPrinter printer;
    return printer;
}
