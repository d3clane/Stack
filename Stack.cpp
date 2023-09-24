#include <math.h>
#include <execinfo.h>
#include <string.h>

#include "ArrayFuncs.h"
#include "Stack.h"
#include "Log.h"

typedef unsigned long long CanaryType;
const CanaryType Canary = 0xDEADBABE;
#define CanaryTypeFormat "%#0llx"

static const size_t STANDARD_CAPACITY = 64;

static Errors StackRealloc(StackType* stk, bool increase);

static inline void CanaryCtor(void* const storage);

static inline ElemType* MovedPtr(ElemType* const stack, const size_t moveSz, const int times);

static inline void MovePtrOnCanarySz(const ElemType** const stack, const size_t times);

static inline size_t StackGetSzForCalloc(StackType* const stk);

static Errors FillStack(StackType* const stk);

static inline bool StackIsFull(StackType* stk);

static inline bool StackIsTooBig(StackType* stk);

//---------------

#undef  STACK_CHECK
#ifndef NDEBUG

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

#else

#define STACK_CHECK(stk) ;

#endif

//---------------

#undef  IF_ERR_RETURN
#define IF_ERR_RETURN(ERR)              \
do                                      \
{                                       \
    if ((ERR) != Errors::NO_ERR)        \
        return ERR;                     \
} while (0)

//---------------

Errors StackCtor(StackType* const stk, const size_t capacity)
{
    assert(stk);

    stk->size = 0;

    if (capacity > 0) stk->capacity = capacity;
    else              stk->capacity = STANDARD_CAPACITY;

    //----Callocing Memory-------

    stk->stack = (ElemType*) calloc(StackGetSzForCalloc(stk), sizeof(*stk->stack));

    if (stk->stack == nullptr)
    {
        UPDATE_ERR(Errors::MEMORY_ALLOCATION_ERR);  
        return     Errors::MEMORY_ALLOCATION_ERR;
    }

    //-----------------------

    FillStack(stk);

    STACK_CHECK(stk);

    return Errors::NO_ERR;
}

Errors StackDtor(StackType* const stk)
{
    assert(stk);

    STACK_CHECK(stk);

    stk->stack = MovedPtr(stk->stack, sizeof(CanaryType), -1);
    free(stk->stack);
    stk->stack = nullptr;

    stk->size     = 0;
    stk->capacity = 0;

    return Errors::NO_ERR;
}

Errors StackPush(StackType* stk, ElemType val)
{
    assert(stk);
    assert(isfinite(val));

    STACK_CHECK(stk);

    Errors stackReallocErr = Errors::NO_ERR;
    if (StackIsFull(stk)) stackReallocErr = StackRealloc(stk, true);

    IF_ERR_RETURN(stackReallocErr);

    stk->stack[stk->size++] = val;

    STACK_CHECK(stk);

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

        STACK_CHECK(stk);

        IF_ERR_RETURN(stackReallocErr);
    }

    STACK_CHECK(stk);

    return Errors::NO_ERR;
}

Errors StackVerify(StackType* stk)
{
    assert(stk);

    if (stk->stack == nullptr)
    {
       UPDATE_ERR(Errors::STACK_IS_NULLPTR);
       return     Errors::STACK_IS_NULLPTR;
    }

    if (stk->capacity <= 0)
    {  
        UPDATE_ERR(Errors::STACK_CAPACITY_OUT_OF_RANGE);
        return     Errors::STACK_CAPACITY_OUT_OF_RANGE;
    }

    if (stk->size > stk->capacity)
    {
        UPDATE_ERR(Errors::STACK_SIZE_OUT_OF_RANGE);
        return     Errors::STACK_SIZE_OUT_OF_RANGE;
    }

    // -----------Canary checking----------

    if (*(CanaryType*)MovedPtr(stk->stack, sizeof(CanaryType), -1) != Canary)
    {
        UPDATE_ERR(Errors::STACK_INVALID_CANARY);
        return     Errors::STACK_INVALID_CANARY;
    }

    if (*(CanaryType*)(stk->stack + stk->capacity) != Canary)
    {
        UPDATE_ERR(Errors::STACK_INVALID_CANARY);
        return     Errors::STACK_INVALID_CANARY;
    }

    return Errors::NO_ERR;
}

#undef  MIN
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

    LOG("\tLeft canary : " CanaryTypeFormat ",\n", 
        *(CanaryType*)(MovedPtr(stk->stack, sizeof(CanaryType), -1)));
    LOG("\tRight canary: " CanaryTypeFormat ",\n", 
        *(CanaryType*)(stk->stack + stk->capacity)); //TODO change all on function getRightCanary, getLeftcanary
    LOG("\tdata stack[%p]\n\t{\n", stk->stack);

    if (stk->stack != nullptr)
    {
        //чет MIN здесь прям не оч надо чет с этим сделать
        for (size_t i = 0; i < MIN(stk->size, stk->capacity); ++i)
        {
            LOG("\t\t*[%zu] = " ElemTypeFormat, i, stk->stack[i]);

            if (Equal(&stk->stack[i], &POISON)) LOG(" (POISON)");

            LOG("\n");
        }

        LOG("\t\tNot used values:\n");

        for(size_t i = stk->size; i < stk->capacity; ++i)
        {
            LOG("\t\t*[%zu] = " ElemTypeFormat, i, stk->stack[i]);
            
            if (Equal(&stk->stack[i], &POISON)) LOG(" (POISON)");

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
    /*char *buffer[70];
    int sz = backtrace((void**) buffer, 70);
    
    backtrace_symbols_fd((void**) buffer, sz, fileno(stdout));*/

    assert(stk);
    assert(stk->stack);
    assert(stk->capacity > 0);
    
    if (increase) stk->capacity <<= 1;
    else          stk->capacity >>= 1;

    if (!increase) 
        FillArray(stk->stack + stk->capacity, stk->stack + stk->size, POISON);
    
    ElemType* tmpStack = (ElemType*) realloc(MovedPtr(stk->stack, sizeof(CanaryType), -1), 
                                             StackGetSzForCalloc(stk) * sizeof(*stk->stack));

    if (tmpStack == nullptr)
    {
        UPDATE_ERR(Errors::MEMORY_ALLOCATION_ERR);

        assert(stk);
        if (increase) stk->capacity >>= 1;
        else          stk->capacity <<= 1;

        STACK_CHECK(stk);

        return Errors::MEMORY_ALLOCATION_ERR;
    }

    stk->stack = tmpStack;

    // -------Moving forward after reallocing-------
    stk->stack = MovedPtr(stk->stack, sizeof(CanaryType), 1);

    if (increase) 
        FillArray(stk->stack + stk->size, stk->stack + stk->capacity, POISON);

    // -------Putting canary at the end-----------
    CanaryCtor(stk->stack + stk->capacity);

    STACK_CHECK(stk);

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

    return (stk->size * 4 <= stk->capacity) & (stk->capacity > STANDARD_CAPACITY);
}

static inline void CanaryCtor(void* const storage)
{
    assert(storage);

    CanaryType* strg = (CanaryType*) storage;

    *strg = Canary;
}

static inline ElemType* MovedPtr(ElemType* const stack, const size_t moveSz, const int times)
{
    assert(stack);
    assert(moveSz > 0);

    return (ElemType*)((char*)stack + times * (long long)moveSz);
}

static inline void MovePtrOnCanarySz(const ElemType** const stack, const size_t times)
{
    assert(stack);
    
    *stack = (const ElemType*)((const CanaryType*)(*stack) + times);
}

static Errors FillStack(StackType* const stk)
{
    assert(stk);
    assert(stk->stack);
    assert(stk->capacity > 0);

#ifdef STACK_CANARY_PROTECTION
    CanaryCtor(stk->stack);
    stk->stack = MovedPtr(stk->stack, sizeof(CanaryType), 1);
#endif

    FillArray(stk->stack, stk->stack + stk->capacity, POISON);

#ifdef STACK_CANARY_PROTECTION
    CanaryCtor(stk->stack + stk->capacity);
#endif

    STACK_CHECK(stk);

    return Errors::NO_ERR;
}

static inline size_t StackGetSzForCalloc(StackType* const stk)
{
    assert(stk);

#ifdef STACK_CANARY_PROTECTION
    return stk->capacity + 2 * sizeof(CanaryType) / sizeof(*stk->stack);
#else
    return stk->capacity;
#endif
}

#undef STACK_CHECK
#undef IF_ERR_RETURN
