#include <stdio.h>
#include <stdlib.h>

#include "FileTools.cpp"
#include "STACK/Stack.cpp"

#include "ProcessorStruct.h"
#include "ProcessorEngine.cpp"

int ProcessorInit(Processor* processor, const char* byte_code);

int main(int argc, char* argv[])
{   
    if (argc != 2)  
    {   
        PRINT_RED("ERROR: one argument required!\n"
                   "Fuck you! I need just one file name: the one I read from\n");
        return EXIT_FAILURE;
    }

    const char* byte_code = argv[1];

    Processor processor = {};
    ProcessorInit(&processor, byte_code);

    RunProcessor(&processor);

    return EXIT_SUCCESS;
}

int ProcessorInit(Processor* processor, const char* byte_code)
{
    FileInfo CodeInfo = {};
    CreateFileInfo(&CodeInfo, byte_code);

    CreateCodeSegment(processor, &CodeInfo);  // initializing code_length, code_segment

    STACK_INIT(&processor->stk, processor->code_length, &processor->stk_err_status);  // initializing stk
    ProcessError(processor->stk_err_status);

    processor->ip = 0;  // ready to read first instruction of code_segment

    return 0;
}
