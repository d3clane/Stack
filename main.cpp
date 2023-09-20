#include <stdio.h>

#include "Stack.h"

int main() 
{
    StackType stk = 
    {
        .stack    = (int*) calloc(2, sizeof(int)),
        .size     =  0,
        .capacity = 2,
    };

    StackPush(&stk, 17);
    StackPush(&stk, 21);
    StackPush(&stk, 25);

    StackPop(&stk);
    StackPop(&stk);

    STACK_DUMP(&stk);
}