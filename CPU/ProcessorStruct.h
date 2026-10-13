#ifndef PROCESSOR_STRUCT_H

#define PROCESSOR_STRUCT_H

#include "../STACK/Stack.h"
#include "../UTILS/FileTools.h"

enum REGISTER_CODE
{   
    UNKNOWN_REG = -1,
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};

struct Processor
{
    stack_t stk;
    STK_ERR_STATUS stk_err_status;

    int* code_segment;
    int code_length;
    int ip;

    stack_elem_t ax;
    stack_elem_t bx;
    stack_elem_t cx;
    stack_elem_t dx;
};

int     CreateCodeSegment   (Processor* processor, FileInfo *code_info);

int     ProcessorInit       (Processor* processor, const char* byte_code);

#endif
