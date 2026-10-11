#ifndef PROCESSOR_COMMANDS_CPP

#define PROCESSOR_COMMANDS_CPP

#include "ProcessorCommands.h"

CMD_STATUS ProcessorPUSH(Processor* processor)
{
    PROCESSOR_STACK_PUSH(processor, processor->code_segment[processor->ip + 1]);
    processor->ip += 2;  // the next command after PUSH is situated after PUSH argument
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


#endif