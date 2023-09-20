#include "Log.h"

FILE* LOG_FILE;

void LogOpen(const char* argv0)
{
    char* newString = strdup(argv0);

    LOG_FILE = fopen(strcat(newString, ".log"), "a+");

    fprintf(LOG_FILE, "New log was opened by program compiled %s at %s\n", __DATE__, __TIME__);
    //TODO: when open current time

    atexit(LogClose);
    free(newString);
}

void LogClose()
{
    fprintf(LOG_FILE, "Log file was closed by program compiled %s at %s\n", __DATE__, __TIME__);
    //TODO: when closed current time

    PrintSeparator();

    fclose(LOG_FILE);
    LOG_FILE = nullptr;
}