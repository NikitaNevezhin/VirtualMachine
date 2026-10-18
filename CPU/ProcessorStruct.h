#ifndef PROCESSOR_STRUCT_H

#define PROCESSOR_STRUCT_H

#include "../STACK/Stack.h"
#include "../UTILS/FileTools.h"

#define DEBUG_PROCESSOR_STACK

// #define DUMP_EACH_STEP
#define PROCESSOR_DUMP(processor)  ProcessorDump(processor, __FILE__, __LINE__, __func__)

#define CALL_STK_MIN_SIZE    5
#define MAX_CODE_SEGMENT_LEN 1024 * 1024 // 1 MB
#define REGISTERS_AMOUNT     4

#define RAM_SIZE             20

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

    stack_elem_t  REGS[REGISTERS_AMOUNT];
    stack_elem_t* RAM;

    stack_t call_stk;
    STK_ERR_STATUS call_stk_err_status;
};

int     LoadCodeSegment     (Processor* processor, const char* bin_file_name);  // creates code_segment from ByteCode.bin (IS CURRENTLY USED)

int     ProcessorInit       (Processor* processor, const char* byte_code);

int     PrintRegisters      (Processor* processor);

int     PrintCodeSegment    (Processor* processor);

int     PrintRAM            (Processor* processor);

void    ProcessorDump       (Processor* processor, const char* file, int line, const char* func);

#endif
