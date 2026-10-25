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
        case EX:            return "EX";

        default:            return NULL;
    }

    return NULL;
}

void WriteASMcode(const char* input_file, const char* output_file)
{
    assert(input_file);
    assert(output_file);

    FILE* BinFile = fopen(input_file, "rb");
    if (BinFile == NULL)
    {
        perror("ERROR: LoadFromByteCode -> fopen()");
        return;
    }

    int byte_length = GetFileSize(input_file);

    if (byte_length % 4 != 0)
    {
        printf("%s is not aligned to int size (4 bytes)\n", input_file);
        fclose(BinFile);
        abort();
    }

    int code_length = byte_length / 4;
    
    int* code_segment = (int*)calloc(code_length, sizeof(int));

    int read_elems = fread(code_segment, sizeof(int), code_length, BinFile);

    if (read_elems == 0)
    {
        PRINT_RED("Failed at reading %s. Check if the file is empty\n", input_file);
        abort();
    }

    fclose(BinFile);
    
    FILE *ASMcode = fopen(output_file, "w");

    if (ASMcode == NULL)
    {
        perror("ERROR: WriteByteCode -> fopen()");
        abort();
    }


    bool* needed_labels = (bool*)calloc(code_length, sizeof(bool));
    // ----------------------------------------------------------------
    // FIRST iteration: looking for jumps (and call) to memorize labels
    // ----------------------------------------------------------------

    int curr_ip = 0;
    while (curr_ip < code_length)
    {
        int curr_command_code = code_segment[curr_ip];

        if((curr_command_code >= JMP && curr_command_code <= JNE) || curr_command_code == CALL)
        {
            int label_ip = code_segment[curr_ip + 1];
            
            if(label_ip < 0 || label_ip >= code_length)
            {
                PRINT_RED("Invalid label on command number %d\n", curr_ip);
                fclose(ASMcode);
                abort();
            }

            needed_labels[label_ip] = true;
        }

        curr_ip += 1 + OP_TABLE[curr_command_code].arg_amount;
    }

    // ---------------------------------------------------
    // SECOND iteration: creating ASM code with labels
    // ---------------------------------------------------

    curr_ip = 0;
    while(curr_ip < code_length)
    {
        if (needed_labels[curr_ip])    fprintf(ASMcode, ":%d\n", curr_ip);  // put label before current command

        int curr_command_code = code_segment[curr_ip];

        if (curr_command_code <= UNKNOWN || curr_command_code >= CMD_TOTAL_NUMBER)
        {
            PRINT_RED("Could not read from %s, command number %d: invalid opcode %d\n", input_file, curr_ip, curr_command_code);
            fclose(ASMcode);
            abort();
        }

        const CommandInfo* command_info = &OP_TABLE[curr_command_code];  // using pointer to avoid copying

        if (command_info->arg_amount == 0)
        {
            fprintf(ASMcode, "%s\n", command_info->name);
        }

        else if (command_info->arg_amount == 1)
        {
            int curr_command_arg = code_segment[curr_ip + 1];

            if ((curr_command_code >= JMP && curr_command_code <= JNE) || curr_command_code == CALL)
            {
                fprintf(ASMcode, "%s :%d\n", command_info->name, curr_command_arg);
            }

            else if (curr_command_code == PUSHr || curr_command_code == POPr)
            {
                const char* arg_reg_name = GetNameByRegCode((REGISTER_CODE)curr_command_arg);
                // printf("curr_command is <%s>, code <%d>\n", command_info->name,);
                // printf("I was here, arg_reg_name is <%s>, command_arg is <%d>\n", arg_reg_name, curr_command_arg);
                fprintf(ASMcode, "%s %s\n", command_info->name, arg_reg_name);
            }

            else if (curr_command_code == PUSHM || curr_command_code == POPM || curr_command_code == POPV)
            {
                const char* arg_reg_name = GetNameByRegCode((REGISTER_CODE)curr_command_arg);
                fprintf(ASMcode, "%s [%s]\n", command_info->name, arg_reg_name);
            }

            else fprintf(ASMcode, "%s %d\n", command_info->name, curr_command_arg);
        }

        curr_ip += 1 + OP_TABLE[curr_command_code].arg_amount;
    }
 
    free(needed_labels);
    fclose(ASMcode);
}