#include <stdio.h>

#include "Stack.h"
#include "Log.h"

int main(const int argc, const char* const argv[]) 
{
    if (argc > 0) LogOpen(argv[0]);

    StackType stk = {};

    StackCtor(&stk, 4);

    StackPush(&stk, 1237);
    StackPush(&stk, 111);
    StackPush(&stk, 12312);
    StackPop(&stk);

    StackPop(&stk);
    StackPop(&stk);
    StackPop(&stk);
    StackPop(&stk);
    
    StackPop(&stk);
    StackPush(&stk, 12312);
    StackPush(&stk, 111);

    //stk.data[stk.capacity - 1] = 17;

    StackPush(&stk, 111);

    StackDtor(&stk);
}
