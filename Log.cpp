#include "Log.h"

FILE* LOG_FILE;

void LogOpen(const char* argv0)
{
    char* newString = strdup(argv0);
    
    LOG_FILE = fopen(strcat(newString, ".log.html"), "a+");

    fprintf(LOG_FILE, "<pre>\n\n");
    fprintf(LOG_FILE, HTML_RED_HEAD_BEGIN  
                      "Log file was opened by program %s, compiled %s at %s"
                      HTML_HEAD_END "\n",
                      argv0, __DATE__, __TIME__);
    //TODO: when open -> current time

    atexit(LogClose);
    free(newString);
}

void LogClose()
{
    fprintf(LOG_FILE, "\n" HTML_RED_HEAD_BEGIN
                      "Log file was closed by program compiled %s at %s"
                      HTML_HEAD_END "\n",
                      __DATE__, __TIME__);

    //TODO: when closed current time

    PrintSeparator();

    fprintf(LOG_FILE, "</pre>\n");

    fclose(LOG_FILE);
    LOG_FILE = nullptr;
}