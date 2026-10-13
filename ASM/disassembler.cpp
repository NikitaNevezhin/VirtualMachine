#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../UTILS/PrintColor.h"
#include "../UTILS/FileTools.h"
#include "../CPU/CommandCodes.h"
#include "../CPU/ProcessorStruct.h"

// g++ ASM/disassembler.cpp CPU/ProcessorCommands.cpp CPU/ProcessorEngine.cpp CPU/ProcessorStruct.cpp STACK/Stack.cpp STACK/StackErrorProcessor.cpp UTILS/FloatTools.cpp UTILS/FileTools.cpp -o build/disassembler.exe -Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-security -Wformat=2 -Wignored-qualifiers -Winit-self -Winline -Wlogical-op -Wmain -Wmissing-declarations -Wno-missing-field-initializers -Wmissing-include-dirs -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wtype-limits -Wundef -Wunreachable-code -Wwrite-strings -fexceptions -g -pipe -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC -Wno-format -Wno-format-security

const char*     GetCommandByCode        (COMMAND_CODE command_code);

const char*     GetNameByRegCode        (REGISTER_CODE reg_code);

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
        case MUL:       return "MUL";
        case DIV:       return "DIV";
        case OUT:       return "OUT";
        case HLT:       return "HLT";
        case PUSHr:     return "PUSHr";
        case POPr:      return "POPr";
        case JMP:       return "JMP";
        case JA:        return "JA";
        case JAE:       return "JAE";
        case JB:        return "JB";
        case JBE:       return "JBE";
        case JE:        return "JE";
        case JNE:       return "JNE";
        default:        return NULL;
    }
}

const char* GetNameByRegCode(REGISTER_CODE reg_code)
{
    switch (reg_code)
    {   
        case UNKNOWN_REG:   return NULL;

        case AX:            return "AX";
        case BX:            return "BX";
        case CX:            return "CX";
        case DX:            return "DX";

        default:            return NULL;
    }

    return NULL;
}

void WriteASMcode(const char* input_file, const char* output_file)  //FIXME: fix this fucking function: it's shit
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
            switch (command_code)
            {
                case PUSH:
                    fprintf(ASMcode, "%s %lg\n", command, command_arg);
                    break;
                case PUSHr:
                case POPr:
                {
                    const char* reg_name = GetNameByRegCode((REGISTER_CODE)command_arg);
                    fprintf(ASMcode, "%s %s\n", command, reg_name);
                    break;
                }
                case JMP:
                case JA:
                case JAE:
                case JB:
                case JBE:
                case JE:
                case JNE:
                {
                    fprintf(ASMcode, "%s %lg\n", command, command_arg);
                    break;
                }
                default: break;
            }
        }
    }

    fclose(ASMcode);
}