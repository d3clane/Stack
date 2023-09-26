#include <math.h>
#include <execinfo.h>
#include <string.h>

#include "ArrayFuncs.h"
#include "Stack.h"
#include "Log.h"
#include "HashFuncs.h"

//--------CANARY PROTECTION----------

#undef CANARY_CTOR
#undef GET_AFTER_FIRST_CANARY_ADR
#undef GET_FIRST_CANARY_ADR
#undef GET_SECOND_CANARY_ADR
#ifdef STACK_CANARY_PROTECTION

    typedef unsigned long long CanaryType;
    #define CanaryTypeFormat "%#0llx"
    const CanaryType Canary = 0xDEADBABE;

    #define CANARY_CTOR(STORAGE)                        \
    do                                                  \
    {                                                   \
        assert(STORAGE);                                \
        CanaryType* strg = (CanaryType*) (STORAGE);     \
                                                        \
        *strg = Canary;                                 \
    } while(0)

    const size_t Aligning = 8;
    #define GET_AFTER_FIRST_CANARY_ADR(STK) MovePtr((STK)->data, sizeof(CanaryType), 1);
    #define GET_FIRST_CANARY_ADR(STK) MovePtr((STK)->data, sizeof(CanaryType), -1)
    #define GET_SECOND_CANARY_ADR(STK) (char*)((STK)->data + (STK)->capacity) +  \
                                                Aligning - (STK->capacity * sizeof(ElemType)) % Aligning

#else
    
    #define CANARY_CTOR

    #define GET_AFTER_FIRST_CANARY_ADR(STK)  (STK)->data
    #define GET_FIRST_CANARY_ADR(STK)  (STK)->data
    #define GET_SECOND_CANARY_ADR(STK) (STK)->data

#endif

//-----------HASH PROTECTION---------------

#undef UPDATE_DATA_HASH
#ifdef STACK_HASH_PROTECTION

    #define CALC_DATA_HASH(STK) MurmurHash((STK)->data, (STK)->capacity * sizeof(ElemType))
    #define UPDATE_DATA_HASH(STK) (STK)->dataHash = CALC_DATA_HASH(STK)
    
    #define UPDATE_STRUCT_HASH(STK)                             \
    do                                                          \
    {                                                           \
        (STK)->structHash = 0;                                  \
        (STK)->structHash = MurmurHash((STK), sizeof(*STK));    \
    } while (0);
    
#else

    #define GET_HASH(STK)           
    #define UPDATE_DATA_HASH(STK)   

    #define UPDATE_STRUCT_HASH(STK) 
#endif

//----------static functions------------

static Errors StackRealloc(StackType* stk, bool increase);

static inline ElemType* MovePtr(ElemType* const data, const size_t moveSz, const int times);

static inline size_t StackGetSzForCalloc(StackType* const stk);

static Errors StackFill(StackType* const stk);

static inline bool StackIsFull(StackType* stk);

static inline bool StackIsTooBig(StackType* stk);

//--------------Consts-----------------

static const size_t STANDARD_CAPACITY = 64;

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
            HANDLE_ERR(stackErr);            \
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

    stk->data = (ElemType*) calloc(StackGetSzForCalloc(stk), sizeof(*stk->data));

    if (stk->data == nullptr)
    {
        HANDLE_ERR(Errors::MEMORY_ALLOCATION_ERR);  
        return     Errors::MEMORY_ALLOCATION_ERR;
    }

    //-----------------------

    StackFill(stk);

    stk->data = GET_AFTER_FIRST_CANARY_ADR(stk);

    UPDATE_DATA_HASH(stk);
    UPDATE_STRUCT_HASH(stk);

    STACK_CHECK(stk);

    return Errors::NO_ERR;
}

Errors StackDtor(StackType* const stk)
{
    assert(stk);

    STACK_CHECK(stk);

    stk->data = GET_FIRST_CANARY_ADR(stk);

    free(stk->data);
    stk->data = nullptr;

    stk->size     = 0;
    stk->capacity = 0;

#ifdef STACK_HASH_PROTECTION
    stk->dataHash = 0;
#endif

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

    stk->data[stk->size++] = val;

    UPDATE_DATA_HASH(stk);
    UPDATE_STRUCT_HASH(stk);

    STACK_CHECK(stk);

    return Errors::NO_ERR;
}

Errors StackPop(StackType* stk, ElemType* retVal)
{
    assert(stk);

    STACK_CHECK(stk);
    
    if (StackIsEmpty(stk))
    { 
        HANDLE_ERR(Errors::STACK_EMPTY_ERR);
        return     Errors::STACK_EMPTY_ERR;
    }

    --stk->size;
    if (retVal) *retVal = stk->data[--stk->size];
    stk->data[stk->size] = POISON;

    UPDATE_DATA_HASH(stk);
    UPDATE_STRUCT_HASH(stk);

    if (StackIsTooBig(stk))
    {
        Errors stackReallocErr = StackRealloc(stk, false);

        STACK_CHECK(stk);

        IF_ERR_RETURN(stackReallocErr);

        UPDATE_DATA_HASH(stk);
        UPDATE_STRUCT_HASH(stk);
    }

    STACK_CHECK(stk);

    return Errors::NO_ERR;
}

Errors StackVerify(StackType* stk)
{
    assert(stk);

    if (stk->data == nullptr)
    {
       HANDLE_ERR(Errors::STACK_IS_NULLPTR);
       return     Errors::STACK_IS_NULLPTR;
    }

    if (stk->capacity <= 0)
    {  
        HANDLE_ERR(Errors::STACK_CAPACITY_OUT_OF_RANGE);
        return     Errors::STACK_CAPACITY_OUT_OF_RANGE;
    }

    if (stk->size > stk->capacity)
    {
        HANDLE_ERR(Errors::STACK_SIZE_OUT_OF_RANGE);
        return     Errors::STACK_SIZE_OUT_OF_RANGE;
    }

    //-----------Canary checking----------

#ifdef STACK_CANARY_PROTECTION
    if (*(CanaryType*)(GET_FIRST_CANARY_ADR(stk)) != Canary)
    {
        HANDLE_ERR(Errors::STACK_INVALID_CANARY);
        return     Errors::STACK_INVALID_CANARY;
    }

    if (*(CanaryType*)(GET_SECOND_CANARY_ADR(stk)) != Canary)
    {
        HANDLE_ERR(Errors::STACK_INVALID_CANARY);
        return     Errors::STACK_INVALID_CANARY;
    }
#endif

    //------------Hash checking----------

#ifdef STACK_HASH_PROTECTION
    if (CALC_DATA_HASH(stk) != stk->dataHash)
    {
        UPDATE_ERR(Errors::STACK_INVALID_DATA_HASH);
    }

    uint64_t prevStructHash = stk->structHash;
    UPDATE_STRUCT_HASH(stk);

    if (prevStructHash != stk->structHash)
    {
        UPDATE_ERR(Errors::STACK_INVALID_STRUCT_HASH);

        stk->structHash = prevStructHash;
    }
#endif

    return Errors::NO_ERR;
}

// don't like this, should think about it
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

    LOG("StackDump was called in file %s, function %s, line %d\n", fileName, funcName, lineNumber);
    LOG("Stk[%p]\n{\n", stk);
    LOG("\tStk capacity: %zu, \n"
        "\tStk size    : %zu,\n",
        stk->capacity, stk->size);

#ifdef STACK_CANARY_PROTECTION
    LOG("\tLeft canary : " CanaryTypeFormat ",\n", 
        *(CanaryType*)(GET_FIRST_CANARY_ADR(stk)));
    LOG("\tRight canary: " CanaryTypeFormat ",\n", 
        *(CanaryType*)(GET_SECOND_CANARY_ADR(stk)));
#endif

#ifdef STACK_HASH_PROTECTION
    LOG("\tData hash  : %llu\n", stk->dataHash);
    LOG("\tStruct hash: %llu\n", stk->structHash);
#endif

    LOG("\tdata data[%p]\n\t{\n", stk->data);

    if (stk->data != nullptr)
    {
        //чет MIN здесь прям не оч надо чет с этим сделать
        for (size_t i = 0; i < MIN(stk->size, stk->capacity); ++i)
        {
            LOG("\t\t*[%zu] = " ElemTypeFormat, i, stk->data[i]);

            if (Equal(&stk->data[i], &POISON)) LOG(" (POISON)");

            LOG("\n");
        }

        LOG("\t\tNot used values:\n");

        for(size_t i = stk->size; i < stk->capacity; ++i)
        {
            LOG("\t\t*[%zu] = " ElemTypeFormat, i, stk->data[i]);
            
            if (Equal(&stk->data[i], &POISON)) LOG(" (POISON)");

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
    assert(stk->data);
    assert(stk->capacity > 0);
    
    if (increase) stk->capacity <<= 1;
    else          stk->capacity >>= 1;

    if (!increase) 
        FillArray(stk->data + stk->capacity, stk->data + stk->size, POISON);

    ElemType* tmpStack = (ElemType*) realloc(GET_FIRST_CANARY_ADR(stk), 
                                             StackGetSzForCalloc(stk) * sizeof(*stk->data));

    if (tmpStack == nullptr)
    {
        HANDLE_ERR(Errors::MEMORY_ALLOCATION_ERR);

        assert(stk);
        if (increase) stk->capacity >>= 1;
        else          stk->capacity <<= 1;

        STACK_CHECK(stk);

        return Errors::MEMORY_ALLOCATION_ERR;
    }

    stk->data = tmpStack;

    // -------Moving forward after reallocing-------
    stk->data = GET_AFTER_FIRST_CANARY_ADR(stk);

    if (increase) 
        FillArray(stk->data + stk->size, stk->data + stk->capacity, POISON);

    // -------Putting canary at the end-----------
    CANARY_CTOR(GET_SECOND_CANARY_ADR(stk));

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

static inline ElemType* MovePtr(ElemType* const data, const size_t moveSz, const int times)
{
    assert(data);
    assert(moveSz > 0);

    return (ElemType*)((char*)data + times * (long long)moveSz);
}

static Errors StackFill(StackType* const stk)
{
    assert(stk);
    assert(stk->data);
    assert(stk->capacity > 0);

    CANARY_CTOR(stk->data);
    stk->data = GET_AFTER_FIRST_CANARY_ADR(stk);

    FillArray(stk->data, stk->data + stk->capacity, POISON);
    GET_AFTER_FIRST_CANARY_ADR(stk);
    CANARY_CTOR(GET_SECOND_CANARY_ADR(stk));

    stk->data = GET_FIRST_CANARY_ADR(stk);

    return Errors::NO_ERR;
}

static inline size_t StackGetSzForCalloc(StackType* const stk)
{
    assert(stk);

#ifdef STACK_CANARY_PROTECTION
    return stk->capacity + 3 * sizeof(CanaryType) / sizeof(*stk->data);
#else
    return stk->capacity;
#endif
}

#undef STACK_CHECK
#undef IF_ERR_RETURN
#undef GET_AFTER_FIRST_CANARY_ADR
#undef GET_FIRST_CANARY_ADR
#undef GET_SECOND_CANARY_ADR