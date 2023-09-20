#include "Errors.h"

#ifndef NDEBUG
    #define PRINT_ERR(X) fprintf(stderr, RED_TEXT(X "\nError occured in file %s in func %s in line %d\n"), \
                                 ErrorInfo.fileWithError, ErrorInfo.funcWithError, ErrorInfo.lineWithError)
#else
    #define PRINT_ERR(X) fprintf(stderr, RED_TEXT(X));
#endif

ErrorInfoType ErrorInfo = {.error = Errors::NO_ERR, .fileWithError = "NO_ERRORS.txt", .lineWithError = -1};

void PrintError()
{
    switch(ErrorInfo.error)
    {
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
        
        default:
            return true;
    }
}