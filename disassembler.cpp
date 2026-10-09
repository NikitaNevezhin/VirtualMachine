#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PrintColor.h"
#include "FileTools.cpp"

enum COMMAND_CODE
{   
    UNKNOWN = -1,
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MULT = 4,
    DIV = 5,
    OUT = 6,
    HLT = 7
};

const char*     GetCommandByCode        (COMMAND_CODE command_code);

void            WriteASMcode            (const char* input_file, const char* output_file);

int main(int argc, char* argv[])
{   
    if (argc != 3)
    {   
        PRINT_RED("ERROR: two arguments required!\n"
                "Fuck you! I need two file names: the one I read from and the one I write into.\n");
        return EXIT_FAILURE;
    }

    const char* ByteCode = argv[1];
    const char* ASMcode = argv[2];

    WriteASMcode(ByteCode, ASMcode);

    return EXIT_SUCCESS;
}


const char* GetCommandByCode(COMMAND_CODE command_code)
{
    switch (command_code)
    {
        case UNKNOWN:   return NULL;
        case PUSH:      return "PUSH";
        case ADD:       return "ADD";
        case SUB:       return "SUB";
        case MULT:      return "MULT";
        case DIV:       return "DIV";
        case OUT:       return "OUT";
        case HLT:       return "HLT";
        default:        return NULL;
    }
}

void WriteASMcode(const char* input_file, const char* output_file)
{
    assert(input_file);
    assert(output_file);

    FileInfo ByteCode = {};

    CreateFileInfo(&ByteCode, input_file);

    int read_args = 0;  // the amount of arguments found by sscanf in a line: 1 for ADD, SUB, MULT etc and 2 for PUSH, e.g., PUSH 20
    double command_arg = 0; // the argument for the command (for PUSH)
    int command_code = UNKNOWN;  // COMMAND_CODE type has problems with sscanf %d
    
    FILE *ASMcode = fopen(output_file, "w");

    if (ASMcode == NULL)
    {
        perror("ERROR: WriteByteCode -> fopen()");
        abort();
    }

    for (int i = 0; i < ByteCode.lines; i++)
    {
        read_args = sscanf(ByteCode.indexes[i], "%d %lg", &command_code, &command_arg);

        if (read_args == 0)
        {
            PRINT_RED("Failed at reading file %s:%d\n", input_file, i + 1);  // +1 as line numeration starts from 1, while i starts from 0
            fclose(ASMcode);
            abort();
        }

        const char* command = GetCommandByCode((COMMAND_CODE)command_code);

        if (command == NULL)
        {
            PRINT_RED("Could not read command from %s:%d\n", input_file, i + 1);  // +1 as line numeration starts from 1, while i starts from 0
            fclose(ASMcode);
            abort();
        }

        if (read_args == 1)
        {
            if (strcmp(command, "PUSH") == 0)
            {
                PRINT_RED("Command read failure in %s:%d -> PUSH requires argument\n", input_file, i + 1); // +1 as line numeration starts from 1, while i starts from 0
                fclose(ASMcode);
                abort();
            }

            fprintf(ASMcode, "%s\n", command);
        }

        else if (read_args == 2)
        {
            if (strcmp(command, "PUSH")) // if command is not PUSH
            {
                PRINT_RED("Command read failure in %s:%d -> command %s, %d has no arguments\n", 
                    input_file, i + 1, command, command_code); // +1 as line numeration starts from 1, while i starts from 0
                    
                fclose(ASMcode);
                abort();
            }

            fprintf(ASMcode, "%s %lg\n", command, command_arg);
        }
    }

    fclose(ASMcode);
}