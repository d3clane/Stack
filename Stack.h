#ifndef STACK_H
#define STACK_H

#include <assert.h>

#include "Errors.h"
#include "Types.h"

#define STACK_CANARY_PROTECTION

#define STACK_HASH_PROTECTION

/// @brief Contains all info about data to use it 
struct StackType
{
    ElemType* data;       ///< data with values. Have to be a dynamic array.
    size_t size;          ///< pos to push/pop values (actual size of the data at this moment).

#ifdef STACK_HASH_PROTECTION
    uint64_t dataHash;    ///< hash of all elements in data.
    
    uint64_t structHash;  ///< hash of all elements in struct.
#endif

    size_t capacity;     ///< REAL size of the data at this moment (calloced more than need at this moment).
};

/// @brief Constructor
/// @param stk 
/// @param capacity 
/// @return 
uint64_t StackCtor(StackType* const stk, const size_t capacity = 0);

/// @brief 
/// @param stk 
/// @return 
uint64_t StackDtor(StackType* const stk);

/// @brief 
/// @param stk 
/// @param val 
/// @return 
uint64_t StackPush(StackType* stk, ElemType val);

/// @brief 
/// @param stk 
/// @param retVal 
/// @return 
uint64_t StackPop(StackType* stk, ElemType* retVal = nullptr);

/// @brief 
/// @param stk 
/// @return 
uint64_t StackVerify(StackType* stk);

/// @brief 
/// @param stk 
/// @param fileName 
/// @param funcName 
/// @param lineNumber 
/// @return 
void StackDump(StackType* stk, const char* const fileName, 
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