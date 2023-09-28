#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Colors.h"
#include "Errors.h"
#include "Log.h"

//---------------

#undef PRINT_ERR

#ifndef NDEBUG
    #define PRINT_ERR(X) Log(HTML_RED_HEAD_BEGIN "\n"                                \
                             X "Error occured in file %s in func %s in line %d\n"    \
                             HTML_HEAD_END "\n",                                     \
                             ErrorInfo.fileWithError, ErrorInfo.funcWithError, ErrorInfo.lineWithError)
#else
    #define PRINT_ERR(X) Log(HTML_RED_HEAD_BEGIN "\n" (X) "\n" HTML_HEAD_END "\n")
#endif

//---------------

ErrorInfoType ErrorInfo = {.error = Errors::NO_ERR, .fileWithError = "NO_ERRORS.txt", .lineWithError = -1};

void PrintError()
{
    switch(ErrorInfo.error)
    {
        case Errors::MEMORY_ALLOCATION_ERR:
            PRINT_ERR("Memory allocation error.\n");
            break;

        case Errors::NO_ERR:
        default:
            break;
    }
}

bool IsFatalError()
{
    switch(ErrorInfo.error)
    {
        case Errors::NO_ERR:
            return false;
        
        case Errors::MEMORY_ALLOCATION_ERR:
        default:
            return true;
    }
}