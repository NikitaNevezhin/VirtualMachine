#ifndef PROCESSOR_COMMANDS_H

#define PROCESSOR_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ProcessorStruct.h"
#include "CommandCodes.h"

#define CMD_TOTAL_NUMBER 10  // including "UNKNOWN"

#define PROCESSOR_STACK_PUSH(processor, value) StackPush(&processor->stk, value, &processor->stk_err_status) 

#define PROCESSOR_STACK_POP(processor)         StackPop(&processor->stk, &processor->stk_err_status)            

enum CMD_STATUS
{
    EXECUTION_SUCCESS,
    COMMAND_NOT_EXIST,
    NO_ARG_FOR_PUSH,
    EXTRA_ARGS,
    UNKNOWN_REGISTER,
    MACHINE_STOP
};

CMD_STATUS      ProcessorPUSH    (Processor* processor);

CMD_STATUS      ProcessorADD     (Processor* processor);

CMD_STATUS      ProcessorSUB     (Processor* processor);

CMD_STATUS      ProcessorMUL     (Processor* processor);

CMD_STATUS      ProcessorDIV     (Processor* processor);

CMD_STATUS      ProcessorOUT     (Processor* processor);

CMD_STATUS      ProcessorHLT     ();

CMD_STATUS      ProcessorPUSHr   (Processor* processor);

CMD_STATUS      ProcessorPOPr    (Processor* processor);

CMD_STATUS      ProcessorJMP     (Processor* processor);

CMD_STATUS      ProcessorJA      (Processor* processor);

CMD_STATUS      ProcessorJAE     (Processor* processor);

CMD_STATUS      ProcessorJB      (Processor* processor);

CMD_STATUS      ProcessorJBE     (Processor* processor);

CMD_STATUS      ProcessorJE      (Processor* processor);

CMD_STATUS      ProcessorJNE     (Processor* processor);

#endif