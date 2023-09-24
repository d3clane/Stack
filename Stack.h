#ifndef STACK_H
#define STACK_H

#include <assert.h>

#include "Errors.h"
#include "Types.h"

#define STACK_CANARY_PROTECTION
#define STACK_HASH_PROTECTION

#undef  STACK_DUMP
#define STACK_DUMP(STK) StackDump((STK), __FILE__, __func__, __LINE__)

/// @brief Contains all info about stack to use it 
struct StackType
{
    ElemType* stack;  ///< stack with values. Have to be a dynamic array.
    size_t size;      ///< pos to push/pop values (actually size of the stack at this moment)

    size_t capacity;  ///< REAL size of the stack at this moment (calloced more than need at this moment)
};

/// @brief Constructor
/// @param stk 
/// @param capacity 
/// @return 
Errors StackCtor(StackType* const stk, const size_t capacity = 0);

/// @brief 
/// @param stk 
/// @return 
Errors StackDtor(StackType* const stk);

/// @brief 
/// @param stk 
/// @param val 
/// @return 
Errors StackPush(StackType* stk, ElemType val);

/// @brief 
/// @param stk 
/// @param retVal 
/// @return 
Errors StackPop(StackType* stk, ElemType* retVal = nullptr);

/// @brief 
/// @param stk 
/// @return 
Errors StackVerify(StackType* stk);

/// @brief 
/// @param stk 
/// @param fileName 
/// @param funcName 
/// @param lineNumber 
/// @return 
Errors StackDump(StackType* stk, const char* const fileName, 
                                 const char* const funcName,
                                 const int lineNumber);

/// @brief 
/// @param stk 
/// @return 
static inline bool StackIsEmpty(StackType* stk)
{
    assert(stk);
    assert(stk->stack);
    
    return stk->size == 0;
}

#endif // STACK_H
