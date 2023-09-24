#ifndef LOG_H
#define LOG_H

#include <time.h>
#include <stdio.h>

#include "Colors.h"

extern FILE* LOG_FILE;

void LogOpen(const char* argv0);

void LogClose();

//TODO: __DATE__ and __TIME__ is showing compiling time - how to get current time 

#undef  LOG_BEGIN
#define LOG_BEGIN()                                                              \
do                                                                               \
{                                                                                \
    time_t timeInSeconds = time(nullptr);                                        \
    fprintf(LOG_FILE, "\n-----------------------\n\n"                            \
                      HTML_GREEN_HEAD_BEGIN "\n"                                 \
                      "New log called %s"                                        \
                      "Called from file: %s, from function: %s, from line: %d\n" \
                      HTML_HEAD_END "\n\n\n",                                    \
                      ctime(&timeInSeconds), __FILE__, __func__, __LINE__);      \
} while (0)

#undef  LOG
#define LOG(...) fprintf(LOG_FILE, __VA_ARGS__)

#undef  LOG_END
#define LOG_END()                                                           \
do                                                                          \
{                                                                           \
    time_t timeInSeconds = time(nullptr);                                   \
    fprintf(LOG_FILE, "\n" HTML_GREEN_HEAD_BEGIN "\n"                       \
                      "Logging ended %s"                                    \
                      "Ended in file: %s, function: %s, line: %d\n"         \
                      HTML_HEAD_END "\n\n"                                  \
                      "-----------------------\n\n\n",                      \
                      ctime(&timeInSeconds), __FILE__, __func__, __LINE__); \
} while (0)

static inline void PrintSeparator()
{
    fprintf(LOG_FILE,
            "\n\n---------------------------------------------------------------------------\n\n");
}

#endif
