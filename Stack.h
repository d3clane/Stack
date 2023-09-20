#ifndef STACK_H
#define STACK_H

#include <assert.h>
#include <stdio.h>

#include "Errors.h"

#define STACK_DUMP(STK) StackDump((STK), __FILE__, __LINE__, __func__)

struct StackType
{
    int* stack;
    size_t size;

    size_t capacity;
};

Errors StackCtor(StackType* const stk, const size_t capacity);

Errors StackDtor(StackType* const stk);

Errors StackPush(StackType* stk, int val);

Errors StackPop(StackType* stk, int* retVal = nullptr);

Errors StackVerify(StackType* stk);

Errors StackDump(StackType* stk, const char* const fileName, 
                             const int       lineNumber, 
                             const char* const funcName);

Errors StackRealloc(StackType* stk, bool increase);

static inline bool StackIsFull(StackType* stk)
{
    assert(stk);
    assert(stk->stack);

    return stk->size >= stk->capacity;
}

static inline bool StackIsTooBig(StackType* stk)
{
    assert(stk);
    assert(stk->stack);

    return stk->size * 4 <= stk->capacity;
}

static inline bool StackIsEmpty(StackType* stk)
{
    assert(stk);
    assert(stk->stack);
    
    return stk->size == 0;
}

#endif // STACK_H
