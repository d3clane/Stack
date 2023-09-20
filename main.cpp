#include <stdio.h>

#include "Stack.h"
#include "Log.h"

int main(const int argc, const char* const argv[]) 
{
    if (argc > 0) LogOpen(argv[0]);
    
    StackType stk = 
    {
        .stack    = (ElemType*) calloc(2, sizeof(ElemType)),
        .size     = 0,
        .capacity = 2,
    };

    StackPush(&stk, 17.233232);
    StackPush(&stk, 21.21);
    StackPush(&stk, 25.22221123);

    StackPop(&stk);
    StackPop(&stk);


    STACK_DUMP(&stk);
}
