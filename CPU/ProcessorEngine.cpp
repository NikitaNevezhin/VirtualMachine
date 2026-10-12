#ifndef PROCESSOR_ENGINE_CPP

#define PROCESSOR_ENGINE_CPP

#include "ProcessorEngine.h"

const char* GetErrorString(CMD_STATUS cmd_status)
{
    switch (cmd_status)
    {
        case EXECUTION_SUCCESS:     return "EXECUTION_SUCCESS";
        case COMMAND_NOT_EXIST:     return "COMMAND_NOT_EXIST";
        case NO_ARG_FOR_PUSH:       return "NO_ARG_FOR_PUSH";
        case EXTRA_ARGS:            return "EXTRA_ARGS";
        case UNKNOWN_REGISTER:      return "UNKNOWN_REGISTER";
        case MACHINE_STOP:          return "MACHINE_STOP";
        default:                    return "UNKNOWN_CMD_STATUS";
    }

    return "UNKNOWN_CMD_STATUS";
}

CMD_STATUS RunCommand(Processor* processor)
{
    assert(processor);

    COMMAND_CODE curr_command = (COMMAND_CODE)processor->code_segment[processor->ip];  // cast to COMMAND_CODE as code_segment contains ints

    switch(curr_command)
    {
        case UNKNOWN:   return COMMAND_NOT_EXIST;
        case PUSH:      return ProcessorPUSH(processor);
        case ADD:       return ProcessorADD(processor);
        case SUB:       return ProcessorSUB(processor);
        case MUL:       return ProcessorMUL(processor);
        case DIV:       return ProcessorDIV(processor);
        case OUT:       return ProcessorOUT(processor);
        case HLT:       return ProcessorHLT();
        case PUSHr:     return ProcessorPUSHr(processor);
        case POPr:      return ProcessorPOPr(processor);
        default:        return COMMAND_NOT_EXIST;
    }

    return COMMAND_NOT_EXIST;
}

int RunProcessor(Processor* processor)
{
    assert(processor);
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