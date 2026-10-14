#include "ProcessorStruct.h"

int CreateCodeSegment(Processor* processor, FileInfo *code_info)
{
    assert(processor);
    assert(code_info);
    // we have max 2 numbers in a single line. total_elems of code segment is less than lines * 2
    int* code_segment = (int*)calloc(code_info->lines * 2, sizeof(int));

    int read_args = 0;  // the amount of arguments found by sscanf in a line: 1 for ADD, SUB, MULT etc and 2 for PUSH, e.g., PUSH 20
    stack_elem_t command_arg = 0; // the argument for the command (for PUSH)
    int command = 0;

    int ip = 0;  // instruction pointer

    for (int i = 0; i < code_info->lines; i++)
    {
        read_args = sscanf(code_info->indexes[i], "%d %d", &command, &command_arg);

        if (read_args == 0) 
        {
            PRINT_RED("Failed at reading %s:%d in %s\n", code_info->filename, i + 1, __func__);
            abort();
        }

        code_segment[ip++] = command;
        if (read_args == 2) code_segment[ip++] = command_arg;
    }

    processor->code_length = ip;
    processor->code_segment = code_segment;

    return 0;
}

int ProcessorInit(Processor* processor, const char* byte_code)
{
    assert(processor);
    assert(byte_code);
    
    FileInfo CodeInfo = {};
    CreateFileInfo(&CodeInfo, byte_code);

    CreateCodeSegment(processor, &CodeInfo);  // initializing code_length, code_segment

    STACK_INIT(&processor->stk, processor->code_length, &processor->stk_err_status);  // initializing stk
    ProcessError(processor->stk_err_status);

    processor->ip = 0;  // ready to read first instruction of code_segment

    return 0;
}

int PrintRegisters(Processor* processor)  // ProcessorDump helper function
{
    printf("    ax = %d\n", processor->ax);
    printf("    bx = %d\n", processor->bx);
    printf("    cx = %d\n", processor->cx);
    printf("    dx = %d\n", processor->dx);

    return 0;
}

int PrintCodeSegment(Processor* processor)
{
    for (int i = 0; i < processor->code_length; i++)
    {
        if (i == processor->ip)
            PRINT_YELLOW("%3d", i);
            
        else
            printf("%3d", i);
    }

    printf("\n");

    for (int i = 0; i < processor->code_length; i++)
    {   
        if (i == processor->ip)
            PRINT_YELLOW("%3d", processor->code_segment[i]);

        else if (i < processor->ip)

            PRINT_GREEN("%3d", processor->code_segment[i]);

        else 
            PRINT_WHITE("%3d", processor->code_segment[i]);
    }

    printf("\n");

    return 0;
}

void ProcessorDump(Processor* processor, const char* file, int line, const char* func)
{   
    printf( "------------ProcessorDump----------------\n");
    printf("ProcessorDump was called in %s:%d in function %s\n", file, line, func);

    if (processor == NULL) 
    {
        PRINT_RED("Processor pointer is NULL!\n");
        printf("-------------------------------------\n");
        return;
    }

    #ifdef DEBUG_PROCESSOR_STACK
    PRINT_GREEN("Processor stack info:\n");
    STACK_DUMP(&processor->stk, processor->stk_err_status);
    #endif

    PRINT_GREEN("REGISTERS:\n");
    PrintRegisters(processor);

    PRINT_GREEN("CODE SEGMENT:\n");
    PrintCodeSegment(processor);

    printf("-------------------------------------\n");
}