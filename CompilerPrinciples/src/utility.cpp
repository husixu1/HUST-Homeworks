/**
 * @file utility.cc
 * @brief implementation of utility.h
 *
 * @date 2018-06-11
 * @author Sixu Hu
**/

#include "utility.h"
#include <stdarg.h>
#include "list.h"
#include <string.h>

static List<const char *> debugKeys;
static const int BufferSize = 2048;

void fatal(const char *format, ...) {
    va_list args;
    char errbuf[BufferSize];

    va_start(args, format);
    vsprintf(errbuf, format, args);
    va_end(args);
    fflush(stdout);
    fprintf(stderr, "\n*** fatal: %s\n\n", errbuf);
    abort();
}



int IndexOf(const char *key) {
    for (int i = 0; i < debugKeys.NumElements(); i++)
        if (!strcmp(debugKeys.Nth(i), key))
            return i;
    return -1;
}

bool isDebugOn(const char *generator) {
    return (IndexOf(generator) != -1);
}


void toggleDebug(const char *generator, bool value) {
    int k = IndexOf(generator);
    if (!value && k != -1)
        debugKeys.RemoveAt(k);
    else if (value && k == -1)
        debugKeys.Append(generator);
}



void debug(const char *generator, const char *format, ...) {
    va_list args;
    char buf[BufferSize];

    if (!isDebugOn(generator))
        return;

    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);
    printf("+++ (%s): %s%s", generator, buf, buf[strlen(buf) - 1] != '\n' ? "\n" : "");
}


void parseArguments(int argc, char *argv[]) {
    if (argc == 1)
        return;

    if (strcmp(argv[1], "-d") != 0) {
        printf("Usage:   -d <debug-key-1> <debug-key-2> ... \n");
        exit(2);
    }

    for (int i = 2; i < argc; i++)
        toggleDebug(argv[i], true);
}
