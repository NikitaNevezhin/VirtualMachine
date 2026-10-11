#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../UTILS/PrintColor.h"
#include "../UTILS/FileTools.h"
#include "../CPU/CommandCodes.h"

// g++ ASM/assembler.cpp CPU/ProcessorCommands.cpp CPU/ProcessorEngine.cpp CPU/ProcessorStruct.cpp STACK/Stack.cpp STACK/StackErrorProcessor.cpp UTILS/FloatTools.cpp UTILS/FileTools.cpp -o build/assembler.exe -Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-security -Wformat=2 -Wignored-qualifiers -Winit-self -Winline -Wlogical-op -Wmain -Wmissing-declarations -Wno-missing-field-initializers -Wmissing-include-dirs -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wtype-limits -Wundef -Wunreachable-code -Wwrite-strings -fexceptions -g -pipe -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC -Wno-format -Wno-format-security

COMMAND_CODE    GetCodeByCommand    (char* command);

void            WriteByteCode       (const char* input_file, const char* output_file);

int main(int argc, char* argv[])
{   
    if (argc != 3)
    {   
        PRINT_RED("ERROR: two arguments required!\n"
                "Fuck you! I need two file names: the one I read from and the one I write into.\n");
        return EXIT_FAILURE;
    }

    const char* ASMcode = argv[1];
    const char* ByteCode = argv[2];

    WriteByteCode(ASMcode, ByteCode);

    return EXIT_SUCCESS;
}


COMMAND_CODE GetCodeByCommand(char* command)
{
    assert(command);

    if (strcmp(command, "PUSH") == 0)    return PUSH;
    if (strcmp(command, "ADD") == 0)     return ADD;
    if (strcmp(command, "SUB") == 0)     return SUB;
    if (strcmp(command, "MUL") == 0)     return MUL;
    if (strcmp(command, "DIV") == 0)     return DIV;
    if (strcmp(command, "OUT") == 0)     return OUT;
    if (strcmp(command, "HLT") == 0)     return HLT;

    return UNKNOWN;
}

void WriteByteCode(const char* input_file, const char* output_file)
{
    assert(input_file);
    assert(output_file);

    FileInfo ASMcode = {};

    CreateFileInfo(&ASMcode, input_file);

    int read_args = 0;  // the amount of arguments found by sscanf in a line: 1 for ADD, SUB, MULT etc and 2 for PUSH, e.g., PUSH 20
    double command_arg = 0; // the argument for the command (for PUSH)
    char* command = (char*)calloc(20, sizeof(char));  // we expect command name length to be less than 4
    
    FILE *ByteCode = fopen(output_file, "w");

    if (ByteCode == NULL)
    {
        perror("ERROR: WriteByteCode -> fopen()");
        abort();
    }

    for (int i = 0; i < ASMcode.lines; i++)
    {
        read_args = sscanf(ASMcode.indexes[i], "%s %lg", command, &command_arg);

        if (read_args == 0)
        {
            PRINT_RED("Failed at reading file %s:%d\n", input_file, i + 1);  // +1 as line numeration starts from 1, while i starts from 0
            fclose(ByteCode);
            abort();
        }

        COMMAND_CODE command_code = GetCodeByCommand(command);

        if (command_code == UNKNOWN)
        {
            PRINT_RED("Could not read command from %s:%d\n", input_file, i + 1);  // +1 as line numeration starts from 1, while i starts from 0
            fclose(ByteCode);
            abort();
        }

        if (read_args == 1)
        {
            if (command_code == PUSH)
            {
                PRINT_RED("Command read failure in %s:%d -> PUSH requires argument\n", input_file, i + 1); // +1 as line numeration starts from 1, while i starts from 0
                fclose(ByteCode);
                abort();
            }

            fprintf(ByteCode, "%d\n", command_code);
        }

        else if (read_args == 2)
        {
            if (command_code != PUSH)
            {
                PRINT_RED("Command read failure in %s:%d -> command %s, %d has no arguments\n", input_file, i + 1, command, command_code); // +1 as line numeration starts from 1, while i starts from 0
                fclose(ByteCode);
                abort();
            }

            fprintf(ByteCode, "%d %lg\n", command_code, command_arg);
        }
    }

    fclose(ByteCode);
}