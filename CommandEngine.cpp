#include <stdio.h>
#include <stdlib.h>

#include "FileTools.cpp"
#include "Stack.cpp"

enum CMD_ERROR
{
    EXECUTION_SUCCESS,
    FILE_READ_FAILURE,
    COMMAND_NOT_EXIST
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
        default:                    return "UNKNOWN_CMD_ERROR";
    }

    return "meow";  //FIXME: I had warning for "control reaches end of non-void function [-Wreturn-type]"" 
}

CMD_ERROR ExecuteCommand(char* input_str, stack_t* stk, ERROR_STATUS* stk_err_status)
{
    int code_args = 0;  // the amount of arguments found by sscanf in a line: 1 for ADD, SUB, MULT etc and 2 for PUSH, e.g., PUSH 20
    double command_arg = 0; // the argument for the command (for PUSH)
    char* command = (char*)calloc(20, sizeof(char));  // expected length of command word is 4 or less
    assert(command);

    code_args = sscanf(input_str, "%s %lg", command, &command_arg);

    if (code_args == 0) 
        return FILE_READ_FAILURE;

    else if (code_args == 1)
    {   
        if (strcmp(command, "OUT") == 0)
        {
            printf("Last stack elem is <%lg>\n", StackPop(stk, stk_err_status));
            return EXECUTION_SUCCESS;
        }

        double curr_elem = StackPop(stk, stk_err_status);
        double prev_elem = StackPop(stk, stk_err_status);
        double res = 0;
        
        if (strcmp(command, "ADD") == 0)
            res = curr_elem + prev_elem;

        else if (strcmp(command, "SUB") == 0)
            res = prev_elem - curr_elem;

        else if (strcmp(command, "MULT") == 0)
            res = prev_elem * curr_elem;

        else if (strcmp(command, "DIV") == 0)
            res = prev_elem / curr_elem;

        StackPush(stk, res, stk_err_status);
    }

    else if (code_args == 2)
    {
        if (strcmp(command, "PUSH") == 0)
            StackPush(stk, command_arg, stk_err_status);
    }

    return EXECUTION_SUCCESS;
}
