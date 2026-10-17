#include "ProcessorStruct.h"

int LoadCodeSegment(Processor* processor, const char* bin_file_name)
{
    assert(processor);
    assert(bin_file_name);

    FILE* BinFile = fopen(bin_file_name, "rb");
    if (BinFile == NULL)
    {
        perror("ERROR: LoadFromByteCode -> fopen()");
        return 1;
    }

    processor->code_segment = (int*)calloc(MAX_CODE_SEGMENT_LEN, sizeof(int));

    int read_elems = fread(processor->code_segment, sizeof(int), MAX_CODE_SEGMENT_LEN, BinFile);

    if (read_elems <= 0)
    {
        printf("ERROR: Binary file could not be read (it might be empty)\n");
        fclose(BinFile);
        return 1;
    }

    fclose(BinFile);

    processor->code_length = read_elems;
    processor->ip = 0;

    return 0;
}

int ProcessorInit(Processor* processor, const char* bin_byte_code)
{
    assert(processor);
    assert(bin_byte_code);

    LoadCodeSegment(processor, bin_byte_code);  // initializing code_length, code_segment

    STACK_INIT(&processor->stk, processor->code_length, &processor->stk_err_status);  // initializing stk
    STACK_INIT(&processor->call_stk, CALL_STK_MIN_SIZE, &processor->call_stk_err_status);
    ProcessError(processor->stk_err_status);

    for (int i = 0; i < REGISTERS_AMOUNT; i++)
        processor->REGS[i] = POIZON;

    processor->ip = 0;  // ready to read first instruction of code_segment

    return 0;
}

int PrintRegisters(Processor* processor)  // ProcessorDump helper function
{
    for (int i = 0; i < REGISTERS_AMOUNT; i++)
        printf("%cx = %d\n", 'a' + i, processor->REGS[i]);

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

    PRINT_BLUE("MOVE TO NEXT STEP after current PROCESSOR_DUMP: ");
    getchar();
}