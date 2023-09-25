#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Colors.h"
#include "Errors.h"
#include "Log.h"

//---------------

#undef PRINT_ERR

#ifndef NDEBUG
    #define PRINT_ERR(X) LOG(HTML_RED_HEAD_BEGIN "\n"                                \
                             X "Error occured in file %s in func %s in line %d\n"    \
                             HTML_HEAD_END "\n",                                     \
                             ErrorInfo.fileWithError, ErrorInfo.funcWithError, ErrorInfo.lineWithError)
#else
    #define PRINT_ERR(X) LOG(RED_TEXT(X));
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
            PRINT_ERR("Stack canary is invalid.\n");
            break;
        case Errors::STACK_INVALID_DATA_HASH:
            PRINT_ERR("Stack data hash is invalid.\n");
            break;
        case Errors::STACK_INVALID_STRUCT_HASH:
            PRINT_ERR("Stack struct hash is invalid.\n");

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
        
        case Errors::STACK_INVALID_STRUCT_HASH:
        case Errors::STACK_INVALID_DATA_HASH:
        case Errors::STACK_INVALID_CANARY:
        case Errors::MEMORY_ALLOCATION_ERR:
        case Errors::STACK_CAPACITY_OUT_OF_RANGE:
        case Errors::STACK_IS_NULLPTR:
        case Errors::STACK_SIZE_OUT_OF_RANGE:
        default:
            return true;
    }
}
