#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern FILE* LOG_FILE;

void LogOpen(const char* argv0);

void LogClose();

//TODO: __DATE__ and __TIME__ is showing compiling time - how to get current time 

#define LOG_START(STRING) fprintf(LOG_FILE, "New log called %s at %s from"                        \
                                            "file: %s, from function: %s, from line: %d\n\n\n,",  \
                                            __DATE__, __TIME__, __FILE__, __func__, __LINE__ );


#define LOG(STRING) fprintf(LOG_FILE, STRING "\n");

#define LOG_END(STRING) fprintf(LOG_FILE, "Logging ended %s at %s in"                \
                                          "file: %s, function: %s, line: %d\n\n\n,"  \
                                          __DATE__, __TIME__, __FILE__, __func__, __LINE__ );                                          )

static inline void PrintSeparator()
{
    fprintf(LOG_FILE, "\n\n---------------------------------------------------------------------------\n\n");
}

#endif