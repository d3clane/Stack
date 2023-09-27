#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Errors.h"
#include "Log.h"

//---------------

static void PrintError(const Errors error);

static bool IsFatalError(const Errors error);

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

ErrorInfoType ErrorInfo = {.error = 0, .fileWithError = "NO_ERRORS.txt", .lineWithError = -1};

void PrintError()
{
    for (size_t errorIndex = 0; errorIndex < sizeof(ErrorType) * 8; ++errorIndex)
    {
        if (GetError((Errors) errorIndex))
            PrintError((Errors) errorIndex);
    }
}

static void PrintError(const Errors error)
{
    assert((size_t) error <= sizeof(ErrorType) * 8);
    assert((size_t) error >= 0);

    switch(error)
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

bool HasFatalError()
{
    for (size_t errorIndex = 0; errorIndex < sizeof(ErrorType) * 8; ++errorIndex)
    {
        if (GetError((Errors) errorIndex) && IsFatalError((Errors) errorIndex)) return true;
    }

    return false;    
}

static bool IsFatalError(const Errors error)
{
    switch(error)
    {   
        case Errors::NO_ERR:
        case Errors::STACK_EMPTY_ERR:
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
