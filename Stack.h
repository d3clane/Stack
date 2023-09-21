#ifndef STACK_H
#define STACK_H

#include <assert.h>

#include "Errors.h"
#include "Types.h"

#define STACK_DUMP(STK) StackDump((STK), __FILE__, __func__, __LINE__)

struct StackType
{
    ElemType* stack;
    size_t size;

    size_t capacity;
};

Errors StackCtor(StackType* const stk, const size_t capacity);

Errors StackDtor(StackType* const stk);

Errors StackPush(StackType* stk, ElemType val);

Errors StackPop(StackType* stk, ElemType* retVal = nullptr);

Errors StackVerify(StackType* stk);

Errors StackDump(StackType* stk, const char* const fileName, 
                                 const char* const funcName,
                                 const int lineNumber);

static inline bool StackIsEmpty(StackType* stk)
{
    assert(stk);
    assert(stk->stack);
    
    return stk->size == 0;
}

#endif // STACK_H
