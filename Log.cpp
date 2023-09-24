#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Log.h"

FILE* LOG_FILE;

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

void LogClose()
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
