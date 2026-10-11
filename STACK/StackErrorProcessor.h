#ifndef STACK_ERROR_PROCESSOR_H

#define STACK_ERROR_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <iostream>

#include "../UTILS/PrintColor.h"

enum STK_ERR_STATUS
{
    OKAY = 0,
    CALLOC_FAILURE = 1,
    REALLOC_FAILURE = 1 << 1,
    STACK_OVERFLOW = 1 << 2,
    STACK_UNDERFLOW = 1 << 3,
    DATA_NULL = 1 << 4,
    NON_POSITIVE_CAPACITY = 1 << 5,
    STACK_NULL = 1 << 6,
    STRUCT_CANARY_CHANGED = 1 << 7,
    DATA_CANARY_CHANGED = 1 << 8
};

const char*     GetErrorString      (STK_ERR_STATUS err_status);

void            PrintError          (STK_ERR_STATUS err_status, STK_ERR_STATUS error, bool* first);

void            PrintAllErrors      (STK_ERR_STATUS err_status);

void            ProcessError        (STK_ERR_STATUS ErrorStatus);

#endif