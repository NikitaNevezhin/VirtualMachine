#ifndef COMMAND_CODES_H

#define COMMAND_CODES_H

#define CMD_TOTAL_NUMBER 25  // including "UNKNOWN"

#include "../UTILS/StrHash.h"
#include "../SPU/ProcessorStruct.h"

enum COMMAND_CODE
{
    UNKNOWN = 0,
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    OUT = 6,
    HLT = 7,
    PUSHr = 8,
    POPr = 9,
    JMP = 10,
    JA = 11,
    JAE = 12,
    JB = 13,
    JBE = 14,
    JE = 15,
    JNE = 16,
    IN = 17,
    CALL = 18,
    RET = 19,
    PUSHM = 20,
    POPM = 21,
    POPV = 22,
    PAINT = 23,
    SQRT = 24
};

enum CMD_STATUS
{
    EXECUTION_SUCCESS,
    COMMAND_NOT_EXIST,
    NO_ARG_FOR_PUSH,
    EXTRA_ARGS,
    UNKNOWN_REGISTER,
    INVALID_INPUT,
    ZERO_DIVISION,
    RAM_INDEX_OUT_OF_RANGE,
    VRAM_INDEX_OUT_OF_RANGE,
    MACHINE_STOP
};

typedef CMD_STATUS (*Command)(Processor*, COMMAND_CODE);

struct CommandInfo
{
    COMMAND_CODE command_code;
    const char* name;
    long long name_hash;
    Command command;
    int arg_amount;
};

extern const CommandInfo OP_TABLE[CMD_TOTAL_NUMBER];

#endif
