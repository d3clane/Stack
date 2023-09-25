#ifndef ARRAY_FUNCS_H
#define ARRAY_FUNCS_H
#include "Types.h"

void FillArray(ElemType* firstBorder, 
               ElemType* secondBorder, 
               const ElemType value);

void Swap(void* const element1, void* const element2, const size_t elemSize);

#endif // ARRAY_FUNCS_H
