#ifndef STACK_CPP

#define STACK_CPP

#include "Stack.h"


void StackInit(stack_t* stk, int capacity, STK_ERR_STATUS* err_status ON_DEBUG(, const char* file, const char* name, int line))
{
    assert(stk);

    if (capacity <= 0)
    {
        *err_status = NON_POSITIVE_CAPACITY;
        return;
    } 

    stk->left_canary = STRUCT_LEFT_CANARY;
    stk->right_canary = STRUCT_RIGHT_CANARY;

    stack_elem_t* given_memory = (stack_elem_t*)malloc((capacity + 2) * sizeof(stack_elem_t)); // +2 - takes places for two canaries

    if (given_memory == NULL)
    {
        *err_status = CALLOC_FAILURE;
        return;
    }

    given_memory[0] = DATA_LEFT_CANARY;
    
    stk->data = given_memory + 1; // skipping canary to the useful info
    stk->size = 0;
    stk->capacity = capacity;

    for (int i = 0; i < capacity; i++) 
    {
        stk->data[i] = POIZON;
    }

    stk->data[capacity] = DATA_RIGHT_CANARY;

    ON_DEBUG(stk->file = file);
    ON_DEBUG(stk->name = name);
    ON_DEBUG(stk->line = line);

    ASSERT_STACK_OK(stk);

    *err_status = OKAY;
    return;
}

void PrintStackData(stack_t* stk)  // this function is used in StackDump. Thats why we dont check the status of stk - we want to see elements anyways
{
    if (stk == NULL)
    {
        PRINT_RED("Cannot print data from NULL stack\n");
        return;
    }

    else if (stk->data == NULL)
    {
        PRINT_RED("    Stack data is NULL pointer\n");
        return;
    }

    printf("    {\n");

    for (int i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size)
        {
            printf(GREEN "       *[%d] = ", i);
            PRINT_STACK_ELEM(stk->data[i]);
            PRINT_GREEN("\n");
        }

        else
        {
            if (IS_POIZON(stk->data[i]))
            {
                printf(RED "        [%d] = ", i);
                PRINT_STACK_ELEM(stk->data[i]);
                PRINT_RED(" [POIZON]\n");
            }
            
            else
            {
                PRINT_YELLOW("        [%d] = ", i, stk->data[i]);
                PRINT_STACK_ELEM(stk->data[i]);
                PRINT_YELLOW(" [UNKNOWN]\n");
            }
        }
    }

    printf("    }\n");
}

void StackDump(stack_t* stk, STK_ERR_STATUS err_status, const char* file, int line, const char* func)
{   
    printf( "------------StackDump----------------\n");
    printf("StackDump was called in %s:%d in function %s\n", file, line, func);

    if (err_status == STACK_NULL || stk == NULL) 
    {
        PRINT_RED("Stack pointer is NULL!\n");
        printf("-------------------------------------\n");
        return;
    }
    
    #ifdef STACK_DEBUG
    printf("stack_t \"%s\" initialized in %s:%d\n", stk->name, stk->file, stk->line); // debug info we only have in debug mode
    #endif

    printf("err_status = ");
    PrintAllErrors(err_status);
    printf("\n");

    if (err_status == STACK_NULL)
        return;

    printf("Struct canaries:\n");
    printf("    Left:  %#llX (expected %#llX)\n", stk->left_canary, STRUCT_LEFT_CANARY);
    printf("    Right: %#llX (expected %#llX)\n", stk->right_canary, STRUCT_RIGHT_CANARY);

    printf("    Size:      %d\n", stk->size);
    printf("    Capacity:  %d\n", stk->capacity);

    printf("    data [" GREEN "%p" RESET "]\n", stk->data);
    printf("    data canaries:\n");

    if (stk->data != NULL)
    {
        PRINT_DATA_CANARIES(stk->data[-1], stk->data[stk->capacity]);
    }

    else printf("       data is NULL, no canaries :( \n");

    PrintStackData(stk);

    printf("-------------------------------------\n");
}

STK_ERR_STATUS StackVerify(stack_t* stk)
{
    if (stk == NULL)
        return STACK_NULL;  // immediately return STACK_NULL as we cannot check anything below (bro, stack is NULL)

    int status = OKAY;  // bit operations give warning, so working with int first

    if (stk->left_canary != STRUCT_LEFT_CANARY || stk->right_canary != STRUCT_RIGHT_CANARY)
        status |= STRUCT_CANARY_CHANGED;

    if (stk->data == NULL) 
    {
        status |= DATA_NULL;
    } 

    else 
    {
        if (!CHECK_LEFT_CANARY(stk->data[-1]) || !CHECK_RIGHT_CANARY(stk->data[stk->capacity]))
            status |= DATA_CANARY_CHANGED;
    }
    
    if (stk->capacity <= 0)
        status |= NON_POSITIVE_CAPACITY;

    if (stk->size < 0)
        status |= STACK_UNDERFLOW; // 

    if (stk->capacity > 0 && stk->size > stk->capacity)
        status |= STACK_OVERFLOW;

    return (STK_ERR_STATUS)status;  // converting to STK_ERR_STATUS after working with int
}

void StackResize(stack_t* stk,STK_ERR_STATUS* err_status, int direction)
{
    ASSERT_STACK_OK(stk);

    int new_capacity = stk->capacity;

    if (direction == UP)
        new_capacity *= 2;

    else if (direction == DOWN)
        new_capacity /= 2;

    if (new_capacity < 4) // in case we work with small stacks, we wouldn't like the capacity to fall less than 4
        new_capacity = 4;

    stack_elem_t* temp = (stack_elem_t*)realloc(stk->data - 1, (new_capacity + 2) * sizeof(stack_elem_t)); // -1 to get the pointer on the left canary

    if (temp == NULL)
    {
        *err_status = REALLOC_FAILURE;
        return;
    }

    stk->data = temp + 1; // skipping left canary to the useful info

    if (direction == UP)
    {
        for (int i = stk->capacity; i < new_capacity; i++)
            stk->data[i] = POIZON;
    }

    stk->capacity = new_capacity;
    stk->data[stk->capacity] = DATA_RIGHT_CANARY;

    ASSERT_STACK_OK(stk)

    *err_status = OKAY;
    return;
}

void StackPush(stack_t* stk, stack_elem_t value, STK_ERR_STATUS* err_status)
{
    ASSERT_STACK_OK(stk);

    if (stk->size == stk->capacity)
    {
        StackResize(stk, err_status, UP);
        ProcessError(*err_status);
    }

    stk->data[stk->size++] = value;

    ASSERT_STACK_OK(stk);

    *err_status = OKAY;
    return;
}

stack_elem_t StackPop(stack_t* stk, STK_ERR_STATUS* err_status)
{
    ASSERT_STACK_OK(stk);
    
    if (stk->size == 0)
    {   
        stk->size--;
        *err_status = STACK_UNDERFLOW;
        return POIZON;
    }

    stk->size--;
    stack_elem_t last_value = stk->data[stk->size];
    stk->data[stk->size] = POIZON;

    // if (stk->size < stk->capacity / 4)
    //     StackResize(stk, err_status, DOWN);
    // ProcessError(*err_status);
    // resize down is now user-only choice

    ASSERT_STACK_OK(stk);

    *err_status = OKAY;
    return last_value;
}

void StackDestroy(stack_t* stk, STK_ERR_STATUS* err_status)
{
    if (stk == NULL)
    {
        *err_status = STACK_NULL;
        return;
    }

    ASSERT_STACK_OK(stk);

    if (stk->data != NULL)
    {
        free(stk->data - 1); // stk->data points on useful info but we have canary before. that's why -1
        stk->data = NULL;
    }

    stk->size = 0;
    stk->capacity = 0;

    *err_status = OKAY;
}

#endif