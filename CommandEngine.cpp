#include <stdio.h>
#include <stdlib.h>

#include "FileTools.cpp"
#include "STACK/Stack.cpp"

#define CMD_TOTAL_NUMBER 7

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

enum CMD_ERROR
{
    EXECUTION_SUCCESS,
    FILE_READ_FAILURE,
    COMMAND_NOT_EXIST,
    NO_ARG_FOR_PUSH,
    EXTRA_ARGS,
    MACHINE_STOP
};

const char*     GetErrorString    (CMD_ERROR cmd_status);

CMD_ERROR       ExecuteCommand    (char* input_str, stack_t* stk, ERROR_STATUS* stk_err_status);

const char* GetErrorString(CMD_ERROR cmd_status)
{
    switch (cmd_status)
    {
        case EXECUTION_SUCCESS:     return "EXECUTION_SUCCESS";
        case FILE_READ_FAILURE:     return "FILE_READ_FAILURE";
        case COMMAND_NOT_EXIST:     return "COMMAND_NOT_EXIST";
        case NO_ARG_FOR_PUSH:       return "NO_ARG_FOR_PUSH";
        case EXTRA_ARGS:            return "EXTRA_ARGS";
        case MACHINE_STOP:          return "MACHINE_STOP";
        default:                    return "UNKNOWN_CMD_ERROR";
    }

    return "meow";  //FIXME: I had warning for "control reaches end of non-void function [-Wreturn-type]"" 
}

CMD_ERROR ExecuteCommand(char* input_str, stack_t* stk, ERROR_STATUS* stk_err_status)
{
    assert(input_str);
    assert(stk);
    assert(stk_err_status);

    int read_args = 0;  // the amount of arguments found by sscanf in a line: 1 for ADD, SUB, MULT etc and 2 for PUSH, e.g., PUSH 20
    stack_elem_t command_arg = 0; // the argument for the command (for PUSH)
    int command = 0;

    read_args = sscanf(input_str, "%d %d", &command, &command_arg);

    if (read_args == 0) 
        return FILE_READ_FAILURE;

    else if (read_args == 1)
    {   
        if (command == HLT)
            return MACHINE_STOP;

        else if (command == OUT) // "OUT"
        {
            printf("Last stack elem is <%d>\n", StackPop(stk, stk_err_status));
            return EXECUTION_SUCCESS;
        }

        else if (command == PUSH)
            return NO_ARG_FOR_PUSH;

        else if (command > CMD_TOTAL_NUMBER || command < 0)
            return COMMAND_NOT_EXIST;

        stack_elem_t curr_elem = StackPop(stk, stk_err_status);
        stack_elem_t prev_elem = StackPop(stk, stk_err_status);
        stack_elem_t res = 0;

        switch (command)
        {
            case ADD:   res = curr_elem + prev_elem;  // "ADD"
                        break;
            case SUB:   res = prev_elem - curr_elem;  // "SUB"
                        break;
            case MULT:  res = prev_elem * curr_elem;  // "MULT"
                        break;
            case DIV:   res = prev_elem / curr_elem;  // "DIV"
                        break;
            default:    return COMMAND_NOT_EXIST;
        }

        StackPush(stk, res, stk_err_status);
    }

    else if (read_args == 2)
    {   
        if (command > CMD_TOTAL_NUMBER || command < 0)
            return COMMAND_NOT_EXIST;

        if (command == PUSH)  // "PUSH"
            StackPush(stk, command_arg, stk_err_status);

        else
            return EXTRA_ARGS;
    }

    return EXECUTION_SUCCESS;
}
