#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Log.h"

static FILE* LOG_FILE;

static inline void PrintSeparator();

static void LogClose();

void LogOpen(const char* argv0)
{
    char* newString = strdup(argv0);

    LOG_FILE = fopen(strcat(newString, ".log.html"), "a+");

    time_t timeInSeconds = time(nullptr);

    fprintf(LOG_FILE, "<pre>\n\n");
    fprintf(LOG_FILE, HTML_RED_HEAD_BEGIN "\n" 
                      "Log file was opened by program %s, compiled %s at %s. "
                      "Opening time: %s"
                      HTML_HEAD_END "\n",
                      argv0, __DATE__, __TIME__, ctime(&timeInSeconds));

    atexit(LogClose);
    free(newString);
}

static void LogClose()
{
    time_t timeInSeconds = time(nullptr);

    fprintf(LOG_FILE, "\n" HTML_RED_HEAD_BEGIN "\n"
                      "Log file was closed by program compiled %s at %s. "
                      "Closing time: %s"
                      HTML_HEAD_END "\n",
                      __DATE__, __TIME__, ctime(&timeInSeconds));

    PrintSeparator();

    fprintf(LOG_FILE, "</pre>\n");

    fclose(LOG_FILE);
    LOG_FILE = nullptr;
}

void LogBegin(const char* fileName, const char* funcName, const int line)
{
    time_t timeInSeconds = time(nullptr);                                        
    fprintf(LOG_FILE, "\n-----------------------\n\n"                            
                      HTML_GREEN_HEAD_BEGIN "\n"                                 
                      "New log called %s"                                        
                      "Called from file: %s, from function: %s, from line: %d\n" 
                      HTML_HEAD_END "\n\n\n",                                    
                      ctime(&timeInSeconds), fileName, funcName, line);      
}

void Log(const char* format, ...)
{
    va_list args = {};

    va_start(args, format);
    vfprintf(LOG_FILE, format, args);
    va_end(args);
}

void LogEnd(const char* fileName, const char* funcName, const int line)
{
    time_t timeInSeconds = time(nullptr);                                   
    fprintf(LOG_FILE, "\n" HTML_GREEN_HEAD_BEGIN "\n"                       
                      "Logging ended %s"                                    
                      "Ended in file: %s, function: %s, line: %d\n"         
                      HTML_HEAD_END "\n\n"                                  
                      "-----------------------\n\n\n",                      
                      ctime(&timeInSeconds), fileName, funcName, line); 
}

static inline void PrintSeparator()
{
    fprintf(LOG_FILE,
            "\n\n---------------------------------------------------------------------------\n\n");
}
