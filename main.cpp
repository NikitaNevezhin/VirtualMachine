#include <stdio.h>
#include <stdlib.h>

#include "FileTools.cpp"
#include "Stack.cpp"

#include "CommandEngine.cpp"

int main(int argc, char* argv[])
{   
    if (argc != 2)
    {   
        printf(RED "ERROR: one argument required!\n"
                   "Fuck you! I need just one file name: the one I read from\n" RESET);
        return EXIT_FAILURE;
    }

    const char* code = argv[1];

    FileInfo CodeInfo = {};

    CreateFileInfo(&CodeInfo, code);

    stack_t stk = {};
    ERROR_STATUS stk_err_status = OKAY;
    STACK_INIT(&stk, 20, &stk_err_status);

    CMD_ERROR curr_cmd_status = EXECUTION_SUCCESS;  // as there were no commands, everything executed successfully

    for (int i = 0; i < CodeInfo.lines; i++)
    {
        curr_cmd_status = ExecuteCommand(CodeInfo.indexes[i], &stk, &stk_err_status);
                // STACK_DUMP(&stk, stk_err_status);
        if (curr_cmd_status == EXECUTION_SUCCESS)
            continue;
        
        else if (curr_cmd_status == MACHINE_STOP)
        {
            printf(GREEN "HLT emergency exit\n" RESET);
            abort();
        }
        // TODO: CMD_ERROR processing
    }

    return EXIT_SUCCESS;
}
