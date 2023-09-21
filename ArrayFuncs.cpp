#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "ArrayFuncs.h"

void FillArray(ElemType* firstBorder, ElemType* secondBorder, const ElemType value)
{
    assert(firstBorder);
    assert(secondBorder);
    
    if (firstBorder > secondBorder) //TODO: change on Swap function
    {
        ElemType* tmp = firstBorder;
                        firstBorder = secondBorder;
                                      secondBorder = tmp;
    }

    for (ElemType* arrIterator = firstBorder; arrIterator < secondBorder; ++arrIterator)
        *arrIterator = value;
}