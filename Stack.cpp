#include "Stack.h"


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
        return Errors::MEMORY_ALLOCATION_ERR;
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

    STACK_CHECK(stk);

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

    if (retVal) *retVal = stk->stack[stk->size];
                                   --stk->size;

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
    LOG("Stk capacity: %zu, \n"
        "Stk size    : %zu\n",
        stk->capacity, stk->size)

    LOG("data stack[%p]\n{\n", stk->stack);
    for (size_t i = 0; i < MIN(stk->size, stk->capacity); ++i)
    {
        LOG("*[%zu] = " ElemTypePrint "\n", i, stk->stack[i]);
    }
    
    LOG("Not used values:\n");

    for(size_t i = stk->size; i < stk->capacity; ++i)
    {
        LOG("*[%zu] = " ElemTypePrint "\n", i, stk->stack[i]);
    }

    LOG("}\n}\n");

    return Errors::NO_ERR;
}

#undef MIN

Errors StackRealloc(StackType* stk, bool increase)
{
    assert(stk);

    if (increase) stk->capacity <<= 1;
    else          stk->capacity >>= 1;

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

    return Errors::NO_ERR;
}

#undef STACK_CHECK
#undef IF_ERR_RETURN
