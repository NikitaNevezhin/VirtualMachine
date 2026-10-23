#ifndef PROCESSOR_COMMANDS_CPP

#define PROCESSOR_COMMANDS_CPP

#include "ProcessorCommands.h"

CMD_STATUS ProcessorPUSH(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter
    PROCESSOR_STACK_PUSH(processor, processor->code_segment[processor->ip + 1] * INT_ARITH_KOEFF);
    processor->ip += 2;  // the next command after PUSH is situated after PUSH argument
    // STACK_DUMP(&processor->stk, processor->stk_err_status);
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorArithm(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    stack_elem_t res = 0;

    switch(command_code)
    {
        case ADD:   res = prev_elem + curr_elem;
                    break;

        case SUB:   res = prev_elem - curr_elem;
                    break;

        case MUL:   res = (prev_elem * curr_elem) / INT_ARITH_KOEFF;;
                    break;

        case DIV:  if (curr_elem == 0)  return ZERO_DIVISION;
                   res = (INT_ARITH_KOEFF * prev_elem) / curr_elem;
                   break;

        default:   return COMMAND_NOT_EXIST;
    }
    
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorOUT(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter
    printf("Last stack elem is <%lg>\n", (double)PROCESSOR_STACK_POP(processor) / INT_ARITH_KOEFF);
    ProcessError(processor->stk_err_status);
    processor->ip++;
    return EXECUTION_SUCCESS; 
}

CMD_STATUS ProcessorHLT(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);    // just to escape warning of unused parameter
    (void)command_code;   // just to escape warning of unused parameter
    return MACHINE_STOP;
}

CMD_STATUS ProcessorRegCommand(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);

    int register_code = processor->code_segment[processor->ip + 1];

    if (register_code <= 0 || register_code > REGISTERS_AMOUNT)  return UNKNOWN_REGISTER;

    switch(command_code)
    {
        case PUSHr:
        {
            stack_elem_t required_value = processor->REGS[register_code - 1];
            PROCESSOR_STACK_PUSH(processor, required_value);
            ProcessError(processor->stk_err_status);
            break;   
        }

        case POPr:
        {
            stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
            ProcessError(processor->stk_err_status);
            processor->REGS[register_code - 1] = curr_elem; 
            break;  
        }

        default:    return COMMAND_NOT_EXIST;
    }

    processor->ip += 2;
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorJMP(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter
    processor->ip = processor->code_segment[processor->ip + 1];
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorConditionalJMP(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    bool condition = false;

    switch(command_code)
    {
        case JA:    condition = (curr_elem > prev_elem);
                    break;
        
        case JAE:   condition = (curr_elem >= prev_elem);
                    break;

        case JB:    condition = (curr_elem < prev_elem);
                    break;

        case JBE:   condition = (curr_elem <= prev_elem);
                    break;

        case JE:    condition = (curr_elem == prev_elem);
                    break;

        case JNE:   condition = (curr_elem != prev_elem);
                    break;

        default:    return COMMAND_NOT_EXIST;
    }

    if (condition)  return ProcessorJMP(processor, JMP);

    processor->ip += 2;
    
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorIN(Processor* processor, COMMAND_CODE command_code)
{   
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter

    PRINT_BLUE("Processor asks for numeric value: ");

    char buffer[BUFFER_SIZE] = "";
    
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)   return INVALID_INPUT;

    if (strchr(buffer, '\n') == NULL) 
    {
        int ch = 0;
        while ((ch = getchar()) != '\n' && ch != EOF) // clear stdin buffer
            ;
    }

    char* endptr = NULL;
    errno = 0;
    
    long long input_value = strtoll(buffer, &endptr, 10);

    if (errno == ERANGE) 
    {
        PRINT_RED("ERROR: entered value is out of range\n");
        return INVALID_INPUT; 
    }

    if (endptr == buffer)  return INVALID_INPUT;  // strtoll ended scanning at the very beginning - no digits found

    if (input_value > LLONG_MAX / INT_ARITH_KOEFF || input_value < LLONG_MIN / INT_ARITH_KOEFF) 
    {
        PRINT_RED("Error: Number will overflow after fixed-point scaling (*%d)!\n", INT_ARITH_KOEFF);
        return INVALID_INPUT;
    }

    PROCESSOR_STACK_PUSH(processor, input_value * INT_ARITH_KOEFF);
    ProcessError(processor->stk_err_status);

    processor->ip++;
    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorCALL(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter

    processor->ip++;  // go to the argument
    int function_label = processor->code_segment[processor->ip];

    stack_elem_t ret_position = (stack_elem_t)(processor->ip + 1);
    PROCESSOR_CALL_STK_PUSH(processor, ret_position);
    ProcessError(processor->call_stk_err_status);

    processor->ip = function_label;

    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorRET(Processor* processor, COMMAND_CODE command_code)
{   
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter

    int ret_position = (int)PROCESSOR_CALL_STK_POP(processor);
    ProcessError(processor->call_stk_err_status);
    processor->ip = ret_position;

    return EXECUTION_SUCCESS;
}

CMD_STATUS ProcessorMemCommand(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    // STACK_DUMP(&processor->stk, processor->stk_err_status);

    int reg_index = processor->code_segment[processor->ip + 1] - 1;  // inside code segment we have reg_code (not index), that's why -1
    if (reg_index < 0 || reg_index >= REGISTERS_AMOUNT) return UNKNOWN_REGISTER;
    
    long long RAM_index = processor->REGS[reg_index] / INT_ARITH_KOEFF;

    if (RAM_index < 0 || RAM_index > RAM_SIZE)  return RAM_INDEX_OUT_OF_RANGE;

    switch(command_code)
    {
        case PUSHM:
        {
            stack_elem_t RAM_value = 0;     
            RAM_value = processor->RAM[RAM_index];
            PROCESSOR_STACK_PUSH(processor, RAM_value);
            ProcessError(processor->stk_err_status);
            break;
        }

        case POPM:
        {
            stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
            ProcessError(processor->stk_err_status);
            processor->RAM[RAM_index] = curr_elem;
            break;
        }
        
        default:    return COMMAND_NOT_EXIST;
    }

    processor->ip += 2;
    return EXECUTION_SUCCESS;    
}

CMD_STATUS ProcessorPOPV(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter
    // STACK_DUMP(&processor->stk, processor->stk_err_status);
    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);

    int reg_index = processor->code_segment[processor->ip + 1] - 1;  // inside code segment we have reg_code (not index), that's why -1
    if (reg_index < 0 || reg_index >= REGISTERS_AMOUNT) return UNKNOWN_REGISTER;
    
    long long VRAM_index = processor->REGS[reg_index] / INT_ARITH_KOEFF;

    if (VRAM_index < 0 || VRAM_index > VRAM_SIZE)  return VRAM_INDEX_OUT_OF_RANGE;

    processor->VRAM[VRAM_index] = curr_elem;    
    
    processor->ip += 2;
    return EXECUTION_SUCCESS; 
}

CMD_STATUS ProcessorPAINT(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter

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

CMD_STATUS ProcessorSQRT(Processor* processor, COMMAND_CODE command_code)
{
    assert(processor);
    (void)command_code;  // just to escape warning of unused parameter

    stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
    ProcessError(processor->stk_err_status);
    
    stack_elem_t res = (stack_elem_t)(sqrt((double)curr_elem * INT_ARITH_KOEFF));
    processor->ip++;
    PROCESSOR_STACK_PUSH(processor, res);

    return EXECUTION_SUCCESS;    
}


#endif
