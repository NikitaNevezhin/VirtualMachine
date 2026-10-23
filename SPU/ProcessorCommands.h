#ifndef PROCESSOR_COMMANDS_H

#define PROCESSOR_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include "ProcessorStruct.h"
#include "../common/commands_info.h"

#define BUFFER_SIZE 100  // buffer is used in ProcessorIN

#define INT_ARITH_KOEFF 1000

#define PROCESSOR_STACK_PUSH(processor, value)      StackPush(&processor->stk, value, &processor->stk_err_status) 

#define PROCESSOR_STACK_POP(processor)              StackPop(&processor->stk, &processor->stk_err_status)

#define PROCESSOR_CALL_STK_PUSH(processor, value)   StackPush(&processor->call_stk, value, &processor->call_stk_err_status)

#define PROCESSOR_CALL_STK_POP(processor)           StackPop(&processor->call_stk, &processor->call_stk_err_status)

CMD_STATUS      ProcessorPUSH               (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorArithm             (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorOUT                (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorHLT                (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorRegCommand         (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorJMP                (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorConditionalJMP     (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorIN                 (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorCALL               (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorRET                (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorMemCommand         (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorPOPV               (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorPAINT              (Processor* processor, COMMAND_CODE command_code);

CMD_STATUS      ProcessorSQRT               (Processor* processor, COMMAND_CODE command_code);

#endif
