#ifndef STACK_ERROR_PROCESSOR_CPP

#define STACK_ERROR_PROCESSOR_CPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <iostream>

#include "../UTILS/PrintColor.h"

enum ERROR_STATUS
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

const char*     GetErrorString      (ERROR_STATUS err_status);

void            PrintError          (ERROR_STATUS err_status, ERROR_STATUS error, bool* first);

void            PrintAllErrors      (ERROR_STATUS err_status);

void            ProcessError        (ERROR_STATUS ErrorStatus);


const char* GetErrorString(ERROR_STATUS err_status) 
{
    switch (err_status) 
    {
        case STACK_NULL:            return  "STACK_NULL";

        case OKAY:                  return  "OKAY";

        case CALLOC_FAILURE:        return  "CALLOC_FAILURE";

        case REALLOC_FAILURE:       return  "REALLOC_FAILURE";

        case STACK_OVERFLOW:        return  "STACK_OVERFLOW";

        case STACK_UNDERFLOW:       return  "STACK_UNDERFLOW";

        case DATA_NULL:             return  "DATA_NULL";

        case NON_POSITIVE_CAPACITY: return  "NON_POSITIVE_CAPACITY";

        case STRUCT_CANARY_CHANGED: return  "STRUCT_CANARY_CHANGED";

        case DATA_CANARY_CHANGED:   return  "DATA_CANARY_CHANGED";

        default:                    return  "You are seriously fucked up, man";
    }
}

void PrintError(ERROR_STATUS err_status, ERROR_STATUS error, bool* first)  // PrintAllErrors helper function
{
    if (err_status & error)
    {   
        if (*first)
            *first = false;
        
        else
            PRINT_RED(" | ");
        
        PRINT_RED("%s", GetErrorString(error)); // %s was added as I had warning saying direct using of function in string is unsafe
    }

    return;
}

void PrintAllErrors(ERROR_STATUS err_status)
{
    if (err_status == OKAY)
    {
        printf(GREEN "Stack is OKAY\n" RESET);
        return;
    }

    bool first = true;

    PrintError(err_status, STACK_NULL, &first);
    PrintError(err_status, CALLOC_FAILURE, &first);
    PrintError(err_status, REALLOC_FAILURE, &first);
    PrintError(err_status, STACK_OVERFLOW, &first);
    PrintError(err_status, STACK_UNDERFLOW, &first);
    PrintError(err_status, DATA_NULL, &first);
    PrintError(err_status, NON_POSITIVE_CAPACITY, &first);
    PrintError(err_status, STRUCT_CANARY_CHANGED, &first);
    PrintError(err_status, DATA_CANARY_CHANGED, &first);

}

void ProcessError(ERROR_STATUS err_status)
{
    if (err_status == OKAY) return;
    if (err_status & STACK_NULL)            PRINT_RED("Error: Stack pointer is NULL\n");
    if (err_status & CALLOC_FAILURE)        PRINT_RED("Error: Could not allocate memory with calloc\n");
    if (err_status & REALLOC_FAILURE)       PRINT_RED("Error: Could not reallocate memory with realloc\n");
    if (err_status & STACK_OVERFLOW)        PRINT_RED("Error: StackOverFlowError\n");
    if (err_status & STACK_UNDERFLOW)       PRINT_RED("Error: StackUnderFlowError\n");
    if (err_status & DATA_NULL)             PRINT_RED("Error: Stack data is NULL pointer!\n");
    if (err_status & NON_POSITIVE_CAPACITY) PRINT_RED("Error: Stack capacity is not positive\n");
    if (err_status & STRUCT_CANARY_CHANGED) PRINT_RED("Error: Struct canary value is changed.\n");
    if (err_status & DATA_CANARY_CHANGED)   PRINT_RED("Error: Stack data canary value is changed.\n");

    return;
}

#endif