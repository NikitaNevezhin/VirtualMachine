#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../UTILS/PrintColor.h"
#include "../UTILS/FileTools.h"
#include "../common/commands_info.h"
#include "../SPU/ProcessorStruct.h"


// g++ ASM/disassembler.cpp CPU/CommandCodes.cpp CPU/ProcessorCommands.cpp CPU/ProcessorEngine.cpp CPU/ProcessorStruct.cpp STACK/Stack.cpp STACK/StackErrorProcessor.cpp UTILS/FloatTools.cpp UTILS/FileTools.cpp -o build/disassembler.exe -Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-security -Wformat=2 -Wignored-qualifiers -Winit-self -Winline -Wlogical-op -Wmain -Wmissing-declarations -Wno-missing-field-initializers -Wmissing-include-dirs -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wtype-limits -Wundef -Wunreachable-code -Wwrite-strings -fexceptions -g -pipe -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC -Wno-format -Wno-format-security

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

void WriteASMcode(const char* input_file, const char* output_file)
{
    assert(input_file);
    assert(output_file);

    FileInfo ByteCode = {};

    CreateFileInfo(&ByteCode, input_file);
    
    FILE *ASMcode = fopen(output_file, "w");

    if (ASMcode == NULL)
    {
        perror("ERROR: WriteByteCode -> fopen()");
        abort();
    }

    bool* needed_labels = (bool*)calloc(2 * ByteCode.lines + 1, sizeof(bool));
    // -----------------------------------------------------
    // FIRST iteration: looking for jumps to memorize labels
    // -----------------------------------------------------

    int curr_ip = 0;
    for (int i = 0; i < ByteCode.lines; i++)
    {
        int command_code = UNKNOWN;
        double command_arg = 0;

        int read_res = sscanf(ByteCode.indexes[i], "%d %lg", &command_code, &command_arg);

        if (read_res <= 0) continue;  // skip empty lines

        if (command_code > UNKNOWN && command_code < CMD_TOTAL_NUMBER)
        {
            if (command_code >= JMP && command_code <= JNE && read_res == 2)
            {
                int target_ip = (int)command_arg;
                if (target_ip >= 0 && target_ip < 2 * ByteCode.lines + 1)
                {
                    needed_labels[target_ip] = true;  // this ip needs a label in ASMcode.txt
                }
            }

            curr_ip += 1 + OP_TABLE[command_code].arg_amount; // +1 for adding a function + the amount of arguments for the function
        }
    }

    curr_ip = 0;
    for (int i = 0; i < ByteCode.lines; i++)
    {
        int command_code = UNKNOWN;
        double command_arg = 0;

        int read_args = sscanf(ByteCode.indexes[i], "%d %lg", &command_code, &command_arg);
        if (read_args <= 0) continue; // skip empty lines

        if (command_code <= UNKNOWN || command_code >= CMD_TOTAL_NUMBER)
        {
            PRINT_RED("Could not read from %s:%d: invalid opcode %d\n", input_file, i + 1, command_code);
            fclose(ASMcode);
            abort();
        }

        const CommandInfo* cmd_info = &OP_TABLE[command_code];

        if (curr_ip < 2 * ByteCode.lines + 1 && needed_labels[curr_ip]) // checks if we have label on this line
        {
            fprintf(ASMcode, ":%d\n", curr_ip);
        }
        // checking if command gets the required amount of arguments
        int actual_args_in_file = read_args - 1;
        if (actual_args_in_file != cmd_info->arg_amount)
        {
            PRINT_RED("Could not read from %s:%d -> Command %s expects %d arguments, but got %d\n", 
                      input_file, i + 1, cmd_info->name, cmd_info->arg_amount, actual_args_in_file);
            fclose(ASMcode);
            abort();
        }

        if (cmd_info->arg_amount == 0)  // ADD, SUB, MUL, DIV, OUT, HLT
        {
            fprintf(ASMcode, "%s\n", cmd_info->name);
        }

        else
        {
            if (command_code == PUSHr || command_code == POPr)
            {
                const char* reg_name = GetNameByRegCode((REGISTER_CODE)command_arg);
                if (reg_name == NULL)
                {
                    PRINT_RED("Could not read from %s:%d: invalid register code %d\n", 
                              input_file, i + 1, (int)command_arg);
                    fclose(ASMcode);
                    abort();
                }

                fprintf(ASMcode, "%s %s\n", cmd_info->name, reg_name);
            }
            else  // PUSH and JMP family
            {
                if (command_code == PUSH)
                    fprintf(ASMcode, "%s %lg\n", cmd_info->name, command_arg);
                else
                    fprintf(ASMcode, "%s :%lg\n", cmd_info->name, command_arg);
            }
        }

        curr_ip += 1 + cmd_info->arg_amount;
    }

    free(needed_labels);
    fclose(ASMcode);
}