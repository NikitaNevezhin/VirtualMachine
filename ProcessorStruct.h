#ifndef PROCESSOR_STRUCT_H

#define PROCESSOR_STRUCT_H

#include "STACK/Stack.cpp"

struct Processor
{
    stack_t stk;
    STK_ERR_STATUS stk_err_status;

    int* code_segment;
    int code_length;
    int ip;
};

#endif
