#ifndef PROCESSOR_COMMANDS_CPP

#define PROCESSOR_COMMANDS_CPP

#include "ProcessorCommands.h"

CMD_STATUS ProcessorPUSH(Processor* processor)
{
    assert(processor);
    PROCESSOR_STACK_PUSH(processor, processor->code_segment[processor->ip + 1] * 1000);
    processor->ip += 2;  // the next command after PUSH is situated after PUSH argument
    // STACK_DUMP(&processor->stk, processor->stk_err_status);
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorADD(Processor* processor)
{   
    assert(processor);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    
    stack_elem_t res = prev_elem + curr_elem;
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorSUB(Processor* processor)
{
    assert(processor);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    
    stack_elem_t res = prev_elem - curr_elem;
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;   
}

CMD_STATUS ProcessorMUL(Processor* processor)
{
    assert(processor);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    
    stack_elem_t res = (prev_elem * curr_elem) / 1000;
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;   
}

CMD_STATUS ProcessorDIV(Processor* processor)
{
    assert(processor);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    if (curr_elem == 0)
        return ZERO_DIVISION;
    
    stack_elem_t res = (1000 * prev_elem) / curr_elem;
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;     
}

CMD_STATUS ProcessorOUT(Processor* processor)
{
    assert(processor);
    printf("Last stack elem is <%lg>\n", (double)PROCESSOR_STACK_POP(processor) / 1000);
    ProcessError(processor->stk_err_status);
    processor->ip++;
    return EXECUTION_SUCCESS; 
}

CMD_STATUS ProcessorHLT(Processor* processor)
{
    assert(processor);    // just to escape warning of unused parameter
    return MACHINE_STOP;
}

CMD_STATUS ProcessorPUSHr(Processor* processor)
{   
    assert(processor);
    stack_elem_t required_value = 0;

    int register_code = processor->code_segment[processor->ip + 1];

    if (register_code <= 0 || register_code > REGISTERS_AMOUNT)  return UNKNOWN_REGISTER;

    required_value = processor->REGS[register_code - 1];

    PROCESSOR_STACK_PUSH(processor, required_value);
    ProcessError(processor->stk_err_status);

    processor->ip += 2;
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorPOPr(Processor* processor)
{   
    assert(processor);
    // STACK_DUMP(&processor->stk, processor->stk_err_status);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    int register_code = processor->code_segment[processor->ip + 1];

    if (register_code <= 0 || register_code > REGISTERS_AMOUNT)  return UNKNOWN_REGISTER;

    processor->REGS[register_code - 1] = curr_elem;
    
    processor->ip += 2;
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorJMP(Processor* processor)
{
    assert(processor);
    processor->ip = processor->code_segment[processor->ip + 1];
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorJA(Processor* processor)
{
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    if (curr_elem > prev_elem)
        return ProcessorJMP(processor);

    processor->ip += 2;
    
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorJAE(Processor* processor)
{
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    if (curr_elem >= prev_elem)
        return ProcessorJMP(processor);

    processor->ip += 2;
    
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorJB(Processor* processor)
{
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    if (curr_elem < prev_elem)
        return ProcessorJMP(processor);

    processor->ip += 2;
    
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorJBE(Processor* processor)
{
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    if (curr_elem <= prev_elem)
        return ProcessorJMP(processor);

    processor->ip += 2;
    
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorJE(Processor* processor)
{
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    if (curr_elem == prev_elem)
        return ProcessorJMP(processor);

    processor->ip += 2;
    
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorJNE(Processor* processor)
{
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    if (curr_elem != prev_elem)
        return ProcessorJMP(processor);

    processor->ip += 2;
    
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorIN(Processor* processor)
{   
    stack_elem_t input_value = POIZON;
    PRINT_BLUE("Processor asks for numeric value: ");
    int read_res = SCANF_STACK_ELEM(&input_value);

    // cleaning buffer from spaces and '\n'
    while (getchar() != '\n')
        ;

    if (read_res == 0)
        return INVALID_INPUT;

    PROCESSOR_STACK_PUSH(processor, input_value * 1000);
    ProcessError(processor->stk_err_status);

    processor->ip++;

    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorCALL(Processor* processor)
{
    processor->ip++;  // go to the argument
    int function_label = processor->code_segment[processor->ip];

    stack_elem_t ret_position = (stack_elem_t)(processor->ip + 1);
    PROCESSOR_CALL_STK_PUSH(processor, ret_position);
    ProcessError(processor->call_stk_err_status);

    processor->ip = function_label;

    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorRET(Processor* processor)
{
    int ret_position = (int)PROCESSOR_CALL_STK_POP(processor);
    ProcessError(processor->call_stk_err_status);
    processor->ip = ret_position;

    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorPUSHM(Processor* processor)
{   
    assert(processor);
    stack_elem_t RAM_value = 0;

    int reg_index = processor->code_segment[processor->ip + 1] - 1;  // inside code segment we have reg_code (not index), that's why -1
    if (reg_index < 0 || reg_index >= REGISTERS_AMOUNT) return UNKNOWN_REGISTER;

    long long RAM_index = processor->REGS[reg_index] / 1000;

    if (RAM_index < 0 || RAM_index > RAM_SIZE)  return RAM_INDEX_OUT_OF_RANGE;

    RAM_value = processor->RAM[RAM_index];

    PROCESSOR_STACK_PUSH(processor, RAM_value);
    ProcessError(processor->stk_err_status);

    processor->ip += 2;
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorPOPM(Processor* processor)
{   
    assert(processor);
    // STACK_DUMP(&processor->stk, processor->stk_err_status);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    int reg_index = processor->code_segment[processor->ip + 1] - 1;  // inside code segment we have reg_code (not index), that's why -1
    if (reg_index < 0 || reg_index >= REGISTERS_AMOUNT) return UNKNOWN_REGISTER;
    
    long long RAM_index = processor->REGS[reg_index] / 1000;

    if (RAM_index < 0 || RAM_index > RAM_SIZE)  return RAM_INDEX_OUT_OF_RANGE;

    processor->RAM[RAM_index] = curr_elem;    
    
    processor->ip += 2;
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorPOPV(Processor* processor)
{
    assert(processor);
    // STACK_DUMP(&processor->stk, processor->stk_err_status);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    int reg_index = processor->code_segment[processor->ip + 1] - 1;  // inside code segment we have reg_code (not index), that's why -1
    if (reg_index < 0 || reg_index >= REGISTERS_AMOUNT) return UNKNOWN_REGISTER;
    
    long long VRAM_index = processor->REGS[reg_index] / 1000;

    if (VRAM_index < 0 || VRAM_index > VRAM_SIZE)  return VRAM_INDEX_OUT_OF_RANGE;

    processor->VRAM[VRAM_index] = curr_elem;    
    
    processor->ip += 2;
    return EXECUTION_SUCCESS; 
}

CMD_STATUS ProcessorPAINT(Processor* processor)
{
    for (int i = 0; i < processor->screen_height; i++)
    {
        for (int j = 0; j < processor->screen_width; j++)
        {
            if (IS_POIZON(processor->VRAM[i * processor->screen_width + j]))   printf(". ");

            else printf("* ");
        }

        printf("\n");
    }

    printf("\n");

    processor->ip++;
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorSQRT(Processor* processor)
{
    assert(processor);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    
    stack_elem_t res = (stack_elem_t)(sqrt((double)curr_elem * 1000));
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;    
}

#endif
