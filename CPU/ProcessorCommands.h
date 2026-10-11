#ifndef PROCESSOR_COMMANDS_H

#define PROCESSOR_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ProcessorStruct.h"
#include "CommandCodes.h"

#define CMD_TOTAL_NUMBER 7

#define PROCESSOR_STACK_PUSH(processor, value) StackPush(&processor->stk, value, &processor->stk_err_status) 

#define PROCESSOR_STACK_POP(processor)         StackPop(&processor->stk, &processor->stk_err_status)            

enum CMD_STATUS
{
    EXECUTION_SUCCESS,
    COMMAND_NOT_EXIST,
    NO_ARG_FOR_PUSH,
    EXTRA_ARGS,
    MACHINE_STOP
};

CMD_STATUS      ProcessorPUSH    (Processor* processor);

CMD_STATUS      ProcessorADD     (Processor* processor);

CMD_STATUS      ProcessorSUB     (Processor* processor);

CMD_STATUS      ProcessorMUL     (Processor* processor);

CMD_STATUS      ProcessorDIV     (Processor* processor);

CMD_STATUS      ProcessorOUT     (Processor* processor);

CMD_STATUS      ProcessorHLT     ();

#endif