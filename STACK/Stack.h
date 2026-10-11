#ifndef STACK_H

#define STACK_H

#define STACK_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../UTILS/PrintColor.h"

#include "StackErrorProcessor.h"
#include "ASSERT_STACK_OK.h"
#include "ElemTypeConfig.h"

#ifdef STACK_DEBUG

#define ON_DEBUG(...) __VA_ARGS__

#define STACK_INIT(stk, capacity, err_status) StackInit(stk, capacity, err_status, __FILE__, #stk, __LINE__)

#else

#define ON_DEBUG(...)

#define STACK_INIT(stk, capacity, err_status) StackInit(stk, capacity, err_status)

#endif

#define STRUCT_LEFT_CANARY  0xDEDDEDDED
#define STRUCT_RIGHT_CANARY 0xEDAEDAEDA

#define STACK_DUMP(stk, err_status) StackDump(stk, err_status, __FILE__, __LINE__, __func__)  

struct stack_t 
{
    unsigned long long left_canary;

    stack_elem_t* data;
    int size;
    int capacity;

    ON_DEBUG
    (
        const char* file; 
        const char* name; 
        int line;
    )

    unsigned long long right_canary;
}; 

enum RESIZE_DIRECTIONS
{
    UP,
    DOWN
};


void             StackInit        (stack_t* stk, int capacity, STK_ERR_STATUS* err_status ON_DEBUG(, const char* file, const char* name, int line));

void             PrintStackData   (stack_t* stk);

void             StackDump        (stack_t* stk, STK_ERR_STATUS err_status, const char* file, int line, const char* func);

STK_ERR_STATUS   StackVerify      (stack_t* stk);

void             StackResize      (stack_t* stk, STK_ERR_STATUS* err_status, int direction);

void             StackPush        (stack_t* stk, stack_elem_t value, STK_ERR_STATUS* err_status);

stack_elem_t     StackPop         (stack_t* stk, STK_ERR_STATUS* err_status);

void             StackDestroy     (stack_t* stk, STK_ERR_STATUS* err_status);

#endif