#ifndef PROCESSOR_COMMANDS_CPP

#define PROCESSOR_COMMANDS_CPP

#include "ProcessorCommands.h"

CMD_STATUS ProcessorPUSH(Processor* processor)
{
    assert(processor);
    PROCESSOR_STACK_PUSH(processor, processor->code_segment[processor->ip + 1]);
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
    
    stack_elem_t res = prev_elem * curr_elem;
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
    
    stack_elem_t res = prev_elem / curr_elem;
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;     
}

CMD_STATUS ProcessorOUT(Processor* processor)
{
    assert(processor);
    printf("Last stack elem is <%d>\n", PROCESSOR_STACK_POP(processor));
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

    REGISTER_CODE register_code = (REGISTER_CODE)processor->code_segment[processor->ip + 1];

    switch (register_code)
    {   
        case UNKNOWN_REG:   return UNKNOWN_REGISTER;

        case AX:            required_value = processor->ax;
                            break;
        case BX:            required_value = processor->bx;
                            break;
        case CX:            required_value = processor->cx;
                            break;
        case DX:            required_value = processor->dx;
                            break;

        default:            return UNKNOWN_REGISTER;
    }

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

    REGISTER_CODE register_code = (REGISTER_CODE)processor->code_segment[processor->ip + 1];

    switch (register_code)
    {   
        case UNKNOWN_REG:   return UNKNOWN_REGISTER;

        case AX:            processor->ax = curr_elem;
                            break;
        case BX:            processor->bx = curr_elem;
                            break;
        case CX:            processor->cx = curr_elem;
                            break;
        case DX:            processor->dx = curr_elem;
                            break;

        default:            return UNKNOWN_REGISTER;
    }
    
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

    PROCESSOR_STACK_PUSH(processor, input_value);
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

#endif