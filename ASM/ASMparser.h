#ifndef ASM_PARSER_H

#define ASM_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../UTILS/PrintColor.h"
#include "../UTILS/FileTools.h"
#include "../CPU/CommandCodes.h"
#include "../CPU/ProcessorStruct.h"

#define LABELS_AMOUNT 100
#define COMMAND_LEN   20
#define REG_NAME_LEN  5

// g++ ASM/assembler.cpp ASM/ASMparser.cpp CPU/CommandCodes.cpp CPU/ProcessorCommands.cpp CPU/ProcessorEngine.cpp CPU/ProcessorStruct.cpp STACK/Stack.cpp STACK/StackErrorProcessor.cpp UTILS/FloatTools.cpp UTILS/FileTools.cpp -o build/assembler.exe -Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-security -Wformat=2 -Wignored-qualifiers -Winit-self -Winline -Wlogical-op -Wmain -Wmissing-declarations -Wno-missing-field-initializers -Wmissing-include-dirs -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wtype-limits -Wundef -Wunreachable-code -Wwrite-strings -fexceptions -g -pipe -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC -Wno-format -Wno-format-security

void            RemoveComments          (char* line);

COMMAND_CODE    GetCodeByCommand        (const char* command_name);

REGISTER_CODE   GetRegCodeByName        (char* reg_name);

bool            ParseLabelDeclaration   (const char* line, int curr_ip, int* labels);

void            ParseJumpCallCommand    (COMMAND_CODE command_code, char* line, char* command_name, 
                                        int* labels, FILE* ByteCode, int line_num);

void            ParseRegisterCommand    (COMMAND_CODE command_code, char* line, char* command_name, 
                                         FILE* ByteCode, int line_num);

void            ParseStandardCommand    (COMMAND_CODE command_code, char* line, char* command_name, 
                                         FILE* ByteCode, int line_num);                                         

void            WriteByteCode           (const char* input_file, const char* output_file);

#endif
