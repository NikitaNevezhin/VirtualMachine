#include "ASMparser.h"

COMMAND_CODE GetCodeByCommand(const char* command_name)
{
    assert(command_name);
    
    for (int i = 1; i < CMD_TOTAL_NUMBER; i++)
    {
        if (OP_TABLE[i].name && strcmp(OP_TABLE[i].name, command_name) == 0)
        {
            return OP_TABLE[i].command_code;
        }
    }
    return UNKNOWN;
}

REGISTER_CODE GetRegCodeByName(char* name)
{
    if (strcmp(name, "AX") == 0)     return AX;
    if (strcmp(name, "BX") == 0)     return BX;
    if (strcmp(name, "CX") == 0)     return CX;
    if (strcmp(name, "DX") == 0)     return DX;

    return UNKNOWN_REG;
}

// returns true if parsed line as label
bool ParseLabelDeclaration(const char* line, int curr_ip, int* labels)
{
    if (line[0] == ':')
    {
        int label_num = 0;
        if (sscanf(line, ":%d", &label_num) == 1)
        {
            if (label_num >= 0 && label_num < LABELS_AMOUNT) 
            {
                labels[label_num] = curr_ip;
                return true;
            }
        }
    }

    return false;
}

void ParseJumpCommand(COMMAND_CODE command_code, char* line, char* command_name, 
                      int* labels, FILE* ByteCode, int line_num)
{
    int jump_label = 0;

    if (sscanf(line, "%*s :%d", &jump_label) != 1)
    {
        PRINT_RED("Could not read from file at line %d: %s requires label argument\n", line_num, command_name);
        fclose(ByteCode);
        abort();
    }

    if (jump_label < 0 || jump_label >= LABELS_AMOUNT || labels[jump_label] == -1)
    {
        PRINT_RED("Invalid label = %d at line %d: ", jump_label, line_num);
        fclose(ByteCode);
        abort();
    }

    fprintf(ByteCode, "%d %d\n", (int)command_code, labels[jump_label]);
}

void ParseRegisterCommand(COMMAND_CODE command_code, char* line, char* command_name, 
                          FILE* ByteCode, int line_num)
{
    char reg_name[REG_NAME_LEN] = {};
    if (sscanf(line, "%*s %s", reg_name) != 1) 
    {
        PRINT_RED("Could not read from file at line %d: %s requires register argument\n", line_num, command_name);
        fclose(ByteCode);
        abort();
    }

    REGISTER_CODE reg_code = GetRegCodeByName(reg_name);
    if (reg_code == UNKNOWN_REG) 
    {
        PRINT_RED("Invalid register = %s at line %d: ", reg_name, line_num);
        fclose(ByteCode);
        abort();
    }

    fprintf(ByteCode, "%d %d\n", command_code, (int)reg_code);
}

void ParseStandardCommand(COMMAND_CODE command_code, char* line, char* command_name, 
                           FILE* ByteCode, int line_num) 
{
    int expected_args = OP_TABLE[command_code].arg_amount;

    if (expected_args == 0) 
    {
        fprintf(ByteCode, "%d\n", command_code);
    } 

    else if (expected_args == 1)
    {
        double command_arg = 0;
        if (sscanf(line, "%*s %lg", &command_arg) != 1) 
        {
            PRINT_RED("Could not read from file at line %d: %s requires number argument\n", line_num, command_name);
            fclose(ByteCode);
            abort();
        }

        fprintf(ByteCode, "%d %lg\n", command_code, command_arg);
    }
}
void WriteByteCode(const char* input_file, const char* output_file)
{
    assert(input_file);
    assert(output_file);

    FileInfo ASMcode = {};
    CreateFileInfo(&ASMcode, input_file);

    FILE *ByteCode = fopen(output_file, "w");
    if (ByteCode == NULL)
    {
        perror("ERROR: WriteByteCode -> fopen()");
        abort();
    }

    int labels[LABELS_AMOUNT] = {};

    // if label is not found in ByteCode, put -1 in labels
    for (int i = 0; i < LABELS_AMOUNT; i++) 
        labels[i] = -1;

    // -------------------------------------------------
    // FIRST iteration: looking for all the labels first
    // -------------------------------------------------
    char command_name[COMMAND_LEN] = {};
    int curr_ip = 0;
    for (int i = 0; i < ASMcode.lines; i++)
    {   
        char* curr_line = ASMcode.indexes[i];

        if (sscanf(ASMcode.indexes[i], "%s", command_name) == 0) continue;  // skip empty lines

        if (ParseLabelDeclaration(curr_line, curr_ip, labels)) continue;  // no increasing of ip - we found label

        COMMAND_CODE command_code = GetCodeByCommand(command_name);
        if (command_code == UNKNOWN)
        {
            PRINT_RED("Could not read from file: line %d\n", i + 1);
            fclose(ByteCode);
            abort();
        }

        curr_ip += 1 + OP_TABLE[command_code].arg_amount; // add 1 for the command and other for amount of args for this command
    }

    // -----------------------------------------------------------------
    // SECOND iteration: assembling ByteCode as now we know all the labels
    // ------------------------------------------------------------------

    for (int i = 0; i < ASMcode.lines; i++)
    {
        char* curr_line = ASMcode.indexes[i];

        if (sscanf(curr_line, "%s", command_name) == 0) continue; // skip empty lines
        if (command_name[0] == ':') continue;                     // skip if it is label

        COMMAND_CODE command_code = GetCodeByCommand(command_name);

        if (command_code == UNKNOWN)
        {
            PRINT_RED("Could not read from file: line %d\n", i + 1);
            fclose(ByteCode);
            abort();
        }

        if (command_code == PUSHr || command_code == POPr)
            ParseRegisterCommand(command_code, curr_line, command_name, ByteCode, i + 1);
        
        else if (command_code >= JMP && command_code <= JNE)
            ParseJumpCommand(command_code, curr_line, command_name, labels, ByteCode, i + 1);

        else 
            ParseStandardCommand(command_code, curr_line, command_name, ByteCode, i + 1);
    }
}