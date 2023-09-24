#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Colors.h"
#include "Errors.h"

//---------------

#undef PRINT_ERR

#ifndef NDEBUG
    #define PRINT_ERR(X) fprintf(stderr, RED_TEXT(X "\nError occured in file %s in func %s in line %d\n"), \
                                 ErrorInfo.fileWithError, ErrorInfo.funcWithError, ErrorInfo.lineWithError)
#else
    #define PRINT_ERR(X) fputs(RED_TEXT(X), stderr);
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
        
        case Errors::STACK_CAPACITY_OUT_OF_RANGE:
            PRINT_ERR("Stack capacity is out of range.\n");
            break;
        case Errors::STACK_IS_NULLPTR:
            PRINT_ERR("Stack is nullptr.\n");
            break;
        case Errors::STACK_EMPTY_ERR:
            PRINT_ERR("Trying to pop from empty stack.\n");
            break;
        case Errors::STACK_SIZE_OUT_OF_RANGE:
            PRINT_ERR("Stack size is out of range.\n");
            break;
        case Errors::STACK_INVALID_CANARY:
            PRINT_ERR("Stack canary is invalid\n");
        case Errors::NO_ERR:
        default:
            break;
    }
}

bool IsFatalError()
{
    switch(ErrorInfo.error)
    {
        case Errors::STACK_EMPTY_ERR:
        case Errors::NO_ERR:
            return false;
        
        case Errors::STACK_INVALID_CANARY:
        case Errors::MEMORY_ALLOCATION_ERR:
        case Errors::STACK_CAPACITY_OUT_OF_RANGE:
        case Errors::STACK_IS_NULLPTR:
        case Errors::STACK_SIZE_OUT_OF_RANGE:
        default:
            return true;
    }
}
