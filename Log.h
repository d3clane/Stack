#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Colors.h"

extern FILE* LOG_FILE;

void LogOpen(const char* argv0);

void LogClose();

//TODO: __DATE__ and __TIME__ is showing compiling time - how to get current time 

#define LOG_BEGIN() fprintf(LOG_FILE, "\n-----------------------\n\n"                           \
                                      HTML_GREEN_HEAD_BEGIN                                     \
                                      "New log called %s at %s "                                \
                                      "from file: %s, from function: %s, from line: %d"         \
                                      HTML_HEAD_END "\n\n\n",                                   \
                                      __DATE__, __TIME__, __FILE__, __func__, __LINE__)

#define LOG(...) fprintf(LOG_FILE, __VA_ARGS__)

#define LOG_END() fprintf(LOG_FILE, "\n" HTML_GREEN_HEAD_BEGIN                  \
                                    "Logging ended %s at %s "                   \
                                    "in file: %s, function: %s, line: %d"       \
                                    HTML_HEAD_END "\n\n"                        \
                                    "-----------------------\n\n\n",            \
                                    __DATE__, __TIME__, __FILE__, __func__, __LINE__)

static inline void PrintSeparator()
{
    fprintf(LOG_FILE,
            "\n\n---------------------------------------------------------------------------\n\n");
}

#endif