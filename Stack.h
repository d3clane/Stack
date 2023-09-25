#ifndef STACK_H
#define STACK_H

#include <assert.h>

#include "Errors.h"
#include "Types.h"

#undef  STACK_CANARY_PROTECTION
#define STACK_CANARY_PROTECTION

#undef  STACK_HASH_PROTECTION
#define STACK_HASH_PROTECTION

#undef  STACK_DUMP
#define STACK_DUMP(STK) StackDump((STK), __FILE__, __func__, __LINE__)

/// @brief Contains all info about data to use it 
struct StackType
{
    ElemType* data;       ///< data with values. Have to be a dynamic array.
    size_t size;          ///< pos to push/pop values (actually size of the data at this moment).
#ifdef STACK_HASH_PROTECTION
    uint64_t dataHash;    ///< hash of all elements in data.
    
    uint64_t structHash;  ///< hash of all elements in struct.
#endif

    size_t capacity;   ///< REAL size of the data at this moment (calloced more than need at this moment).
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
    assert(stk->data);
    
    return stk->size == 0;
}

#endif // STACK_H
