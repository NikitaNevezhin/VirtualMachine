#ifndef PROCESSOR_ENGINE_CPP

#define PROCESSOR_ENGINE_CPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ProcessorStruct.h"

#define CMD_TOTAL_NUMBER 7

#define PROCESSOR_STACK_PUSH(processor, value) StackPush(&processor->stk, value, &processor->stk_err_status) 

#define PROCESSOR_STACK_POP(processor)         StackPop(&processor->stk, &processor->stk_err_status)            

enum COMMAND_CODE
{
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MULT = 4,
    DIV = 5,
    OUT = 6,
    HLT = 7
};

enum CMD_STATUS
{
    EXECUTION_SUCCESS,
    COMMAND_NOT_EXIST,
    NO_ARG_FOR_PUSH,
    EXTRA_ARGS,
    MACHINE_STOP
};

const char*     GetErrorString    (CMD_STATUS cmd_status);

CMD_STATUS       RunCommand        (Processor* processor);

int             RunProcessor      (Processor* processor);

const char* GetErrorString(CMD_STATUS cmd_status)
{
    switch (cmd_status)
    {
        case EXECUTION_SUCCESS:     return "EXECUTION_SUCCESS";
        case COMMAND_NOT_EXIST:     return "COMMAND_NOT_EXIST";
        case NO_ARG_FOR_PUSH:       return "NO_ARG_FOR_PUSH";
        case EXTRA_ARGS:            return "EXTRA_ARGS";
        case MACHINE_STOP:          return "MACHINE_STOP";
        default:                    return "UNKNOWN_CMD_STATUS";
    }

    return "meow";  //FIXME: I had warning for "control reaches end of non-void function [-Wreturn-type]"" 
}

CMD_STATUS RunCommand(Processor* processor)
{
    assert(processor);

    COMMAND_CODE curr_command = (COMMAND_CODE)processor->code_segment[processor->ip];  // cast to COMMAND_CODE as code_segment contains ints

    if (curr_command > CMD_TOTAL_NUMBER || curr_command < 0)
        return COMMAND_NOT_EXIST;

    else if (curr_command == OUT)
    {
        printf("Last stack elem is <%d>\n", PROCESSOR_STACK_POP(processor));
        ProcessError(processor->stk_err_status);
        processor->ip++;
        return EXECUTION_SUCCESS;   
    }

    else if (curr_command == HLT)
        return MACHINE_STOP;

    else if (curr_command == PUSH)
    {
        PROCESSOR_STACK_PUSH(processor, processor->code_segment[processor->ip + 1]);
        processor->ip += 2;  // the next command after PUSH is situated after PUSH argument
    }

    else
    {
        stack_elem_t curr_elem = PROCESSOR_STACK_POP(processor);
        ProcessError(processor->stk_err_status);
        stack_elem_t prev_elem = PROCESSOR_STACK_POP(processor);
        ProcessError(processor->stk_err_status);
        stack_elem_t res = 0;

        if (curr_command == ADD)
            res = curr_elem + prev_elem;
        
        else if (curr_command == SUB)
            res = prev_elem - curr_elem;

        else if (curr_command == MULT)
            res = prev_elem * curr_elem;

        else if (curr_command == DIV)
            res = prev_elem / curr_elem;

        else
            return COMMAND_NOT_EXIST;
        
        processor->ip++;
        PROCESSOR_STACK_PUSH(processor, res);
    }

    return EXECUTION_SUCCESS;
}

int RunProcessor(Processor* processor)
{
    CMD_STATUS command_status = EXECUTION_SUCCESS;
    while (processor->ip <= processor->code_length)
    {
        command_status = RunCommand(processor);  // ip is changed inside RunCommand

        if (command_status == EXECUTION_SUCCESS)
            continue;

        else if (command_status == MACHINE_STOP)
        {
            PRINT_GREEN("HLT emergency exit\n");
            break;
        }

        else
        {
            printf("%s\n", GetErrorString(command_status));
            break;
        }
    }

    return 0;
}

#endif