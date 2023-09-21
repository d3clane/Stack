#include <math.h>

#include "ArrayFuncs.h"
#include "Stack.h"
#include "Log.h"

static Errors StackRealloc(StackType* stk, bool increase);

static inline bool StackIsFull(StackType* stk);

static inline bool StackIsTooBig(StackType* stk);

#define STACK_CHECK(stk)                 \
do                                       \
{                                        \
    Errors stackErr = StackVerify(stk);  \
                                         \
    if (stackErr != Errors::NO_ERR)      \
    {                                    \
        UPDATE_ERR(stackErr);            \
        STACK_DUMP(stk);                 \
        return stackErr;                 \
    }                                    \
} while (0)

#define IF_ERR_RETURN(ERR)              \
do                                      \
{                                       \
    if ((ERR) != Errors::NO_ERR)        \
        return ERR;                     \
} while (0)


Errors StackCtor(StackType* const stk, const size_t capacity = 0)
{
    assert(stk);

    STACK_CHECK(stk);

    static const size_t STANDARD_CAPACITY = 64;

    stk->size = 0;

    if (capacity > 0) stk->capacity = capacity;
    else              stk->capacity = STANDARD_CAPACITY;

    stk->stack = (ElemType*) calloc(stk->capacity, sizeof(*stk->stack));

    if (stk->stack == nullptr)
    {
        UPDATE_ERR(Errors::MEMORY_ALLOCATION_ERR);
        return     Errors::MEMORY_ALLOCATION_ERR;
    }

    return Errors::NO_ERR;
}

Errors StackDtor(StackType* const stk)
{
    assert(stk);

    STACK_CHECK(stk);

    free(stk->stack);
    stk->stack = nullptr;

    stk->size     = 0;
    stk->capacity = 0;

    return Errors::NO_ERR;
}

Errors StackPush(StackType* stk, ElemType val)
{
    assert(stk);
    assert(stk->stack || stk->capacity == 0); //check if stk->stack == nullptr -> stk->capacity = 0
    assert(isfinite(val));

    Errors stackErr = StackVerify(stk);

    if (stackErr != Errors::NO_ERR && stackErr != Errors::STACK_IS_NULLPTR)      
    {                                    
        UPDATE_ERR(stackErr);
        STACK_DUMP(stk);
        return stackErr;
    }

    Errors stackReallocErr = Errors::NO_ERR;
    if (StackIsFull(stk)) stackReallocErr = StackRealloc(stk, true);

    IF_ERR_RETURN(stackReallocErr);

    stk->stack[stk->size++] = val;

    return Errors::NO_ERR;
}

Errors StackPop(StackType* stk, ElemType* retVal)
{
    assert(stk);

    STACK_CHECK(stk);
    
    if (StackIsEmpty(stk))
    { 
        UPDATE_ERR(Errors::STACK_EMPTY_ERR);
        return     Errors::STACK_EMPTY_ERR;
    }

    --stk->size;
    if (retVal) *retVal = stk->stack[--stk->size];
    stk->stack[stk->size] = POISON;

    if (StackIsTooBig(stk))
    {
        Errors stackReallocErr = StackRealloc(stk, false);
        
        IF_ERR_RETURN(stackReallocErr);
    }

    return Errors::NO_ERR;
}

Errors StackVerify(StackType* stk)
{
    assert(stk);
    
    if (stk->stack == nullptr)
        return Errors::STACK_IS_NULLPTR;
    
    if (stk->capacity <= 0)
        return Errors::STACK_CAPACITY_OUT_OF_RANGE;
    
    if (stk->size > stk->capacity)
        return Errors::STACK_SIZE_OUT_OF_RANGE;

    return Errors::NO_ERR;
}

#define MIN(X, Y) ((X) < (Y) ? X : Y)

Errors StackDump(StackType* stk, const char* const fileName, 
                                 const char* const funcName,
                                 const int lineNumber)
{
    assert(stk);
    assert(fileName);
    assert(funcName);
    assert(lineNumber > 0);
    
    LOG_BEGIN();

    LOG("Stk[%p]\n{\n", stk);
    LOG("\tStk capacity: %zu, \n"
        "\tStk size    : %zu,\n",
        stk->capacity, stk->size);

    LOG("\tdata stack[%p]\n\t{\n", stk->stack);

    if (stk->stack != nullptr)
    {
        //чет MIN здесь прям не оч надо чет с этим сделать
        for (size_t i = 0; i < MIN(stk->size, stk->capacity); ++i)
        {
            LOG("\t\t*[%zu] = " ElemTypeFormat, i, stk->stack[i]);

            if (stk->stack[i] == POISON) LOG(" (POISON)"); //TODO: CHANGE ON MY CMP FUNCTION BASED ON _GENERIC
            LOG("\n");
        }

        LOG("\t\tNot used values:\n");

        for(size_t i = stk->size; i < stk->capacity; ++i)
        {
            LOG("\t\t*[%zu] = " ElemTypeFormat "\n", i, stk->stack[i]);
            
            if (stk->stack[i] == POISON) LOG(" (POISON)"); //TODO: CHANGE ON MY CMP FUNCTION BASED ON _GENERIC
            LOG("\n");
        }
    }

    LOG("\t}\n}\n");

    LOG_END();

    return Errors::NO_ERR;
}

#undef MIN

Errors StackRealloc(StackType* stk, bool increase)
{
    assert(stk);
    assert(increase || stk->capacity > 0);
    assert(increase || stk->stack);

    if (increase) stk->capacity <<= 1;
    else          stk->capacity >>= 1;

    if (!increase) FillArray(stk->stack + stk->capacity, stk->stack + stk->size, POISON);
    
    if (stk->capacity == 0) 
        stk->capacity = 1;
    
    ElemType* tmpStack = (ElemType*) realloc(stk->stack, stk->capacity * sizeof(*stk->stack));

    if (tmpStack == nullptr)
    {
        UPDATE_ERR(Errors::MEMORY_ALLOCATION_ERR);

        assert(stk);
        if (increase) stk->capacity >>= 1;
        else          stk->capacity <<= 1;

        return Errors::MEMORY_ALLOCATION_ERR;
    }

    stk->stack = tmpStack;

    if (increase) FillArray(stk->stack + stk->size, stk->stack + stk->capacity, POISON);

    return Errors::NO_ERR;
}

static inline bool StackIsFull(StackType* stk)
{
    assert(stk);

    return stk->size >= stk->capacity;
}

static inline bool StackIsTooBig(StackType* stk)
{
    assert(stk);

    return stk->size * 4 <= stk->capacity;
}

#undef STACK_CHECK
#undef IF_ERR_RETURN
