#ifndef PROCESSOR_COMMANDS_CPP

#define PROCESSOR_COMMANDS_CPP

#include "ProcessorCommands.h"

CMD_STATUS ProcessorPUSH(Processor* processor)
{
    PROCESSOR_STACK_PUSH(processor, processor->code_segment[processor->ip + 1]);
    processor->ip += 2;  // the next command after PUSH is situated after PUSH argument
    // STACK_DUMP(&processor->stk, processor->stk_err_status);
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorADD(Processor* processor)
{
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
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    
    stack_elem_t res = prev_elem / curr_elem;
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;     
}

CMD_STATUS ProcessorOUT(Processor* processor)
{
    printf("Last stack elem is <%d>\n", PROCESSOR_STACK_POP(processor));
    ProcessError(processor->stk_err_status);
    processor->ip++;
    return EXECUTION_SUCCESS; 
}

CMD_STATUS ProcessorHLT()
{
    return MACHINE_STOP;
}

CMD_STATUS ProcessorPUSHr(Processor* processor)
{   
    stack_elem_t required_value = 0;

    REGISTER_CODE register_code = (REGISTER_CODE)processor->code_segment[processor->ip + 1];

    switch (register_code)
    {
        case AX:    required_value = processor->ax;
                    break;
        case BX:    required_value = processor->bx;
                    break;
        case CX:    required_value = processor->cx;
                    break;
        case DX:    required_value = processor->dx;
                    break;
        default:    return UNKNOWN_REGISTER;
    }

    PROCESSOR_STACK_PUSH(processor, required_value);
    ProcessError(processor->stk_err_status);

    processor->ip += 2;
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorPOPr(Processor* processor)
{   
    // STACK_DUMP(&processor->stk, processor->stk_err_status);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    REGISTER_CODE register_code = (REGISTER_CODE)processor->code_segment[processor->ip + 1];

    switch (register_code)
    {
        case AX:    processor->ax = curr_elem;
                    break;
        case BX:    processor->bx = curr_elem;
                    break;
        case CX:    processor->cx = curr_elem;
                    break;
        case DX:    processor->dx = curr_elem;
                    break;
        default:    return UNKNOWN_REGISTER;
    }
    
    processor->ip += 2;
    return EXECUTION_SUCCESS;
}


#endif