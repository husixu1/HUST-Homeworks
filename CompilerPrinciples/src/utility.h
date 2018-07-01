/**
 * @file utility.h
 * @brief utility definitions and functions
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdlib.h>
#include <stdio.h>
#include <functional>

#ifndef YYLTYPE

/// @brief store location information
typedef struct yyltype {
    int timestamp;
    int first_line, first_column;
    int last_line, last_column;
    char *text;
} yyltype;

#define YYLTYPE yyltype

/// @brief the global location variable for lexer
extern struct yyltype yylloc;

/// @brief concatenate two locations together
/// @param first the fisrt location
/// @param last the second location
/// @return the joined location
inline yyltype concatLocation(yyltype first, yyltype second) {
    yyltype result;
    result.first_column = first.first_column;
    result.first_line = first.first_line;
    result.last_column = second.last_column;
    result.last_line = second.last_line;
    return result;
}

/// @brief concatenate two locations together
/// @param first the fisrt location pointer
/// @param second the second location poninter
/// @return the joined location
inline yyltype concatLocation(yyltype *first, yyltype *second) {
    return concatLocation(*first, *second);
}

#endif

/// @brief a simple scope guard
class ScopeGuard {
private:
    /// @brief this function is called when program leaves scope
    std::function<void()> guardFunc;
public:
    /// @brief constructor
    /// @param func the function to call before leaving scope
    ScopeGuard(std::function<void()> func) {
        guardFunc = func;
    }

    /// @brief destructor, calls guardFunc
    ~ScopeGuard() {
        guardFunc();
    }

    /// @brief prevent copy
    ScopeGuard(const ScopeGuard &) = delete;
    /// @brief prevent copy
    ScopeGuard &operator=(const ScopeGuard &) = delete;
};

/// @brief wrapper for assert assert.h, but print more infos
/// @param expr the expression to assert
#define ASSERT(expr)  \
    ((expr) ? (void)0 : fatal("Assertion failed: %s, line %d:\n    %s", __FILE__, __LINE__, #expr))

/// @brief reoprt fatal error message and exit program
/// @param format the printf-style format string
/// @param ... parameters
void fatal(const char *format, ...);

/// @brief print debug
/// @param generator the debug generator
/// @param format the printf-style format string
/// @param ... parameters
void debug(const char *generator, const char *format, ...);

/// @brief turn debug on/off
/// @param generator the debug generator
/// @param val true/false -> on/off
void toggleDebug(const char *key, bool val);

/// @brief query if the debug is on
/// @param generator the generator of debug
/// @return is debug on for generator
bool isDebugOn(const char *generator);

/// @brief parse command-line arguments
/// @param argc the argc parameter in main function
/// @param argv the argv parameter in main function
void parseArguments(int argc, char *argv[]);

#endif // UTILITY_H_
