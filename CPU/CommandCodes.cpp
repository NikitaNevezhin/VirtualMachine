#include "CommandCodes.h"

extern CMD_STATUS      ProcessorPUSH    (Processor* processor);

extern CMD_STATUS      ProcessorADD     (Processor* processor);

extern CMD_STATUS      ProcessorSUB     (Processor* processor);

extern CMD_STATUS      ProcessorMUL     (Processor* processor);

extern CMD_STATUS      ProcessorDIV     (Processor* processor);

extern CMD_STATUS      ProcessorOUT     (Processor* processor);

extern CMD_STATUS      ProcessorHLT     (Processor* processor);

extern CMD_STATUS      ProcessorPUSHr   (Processor* processor);

extern CMD_STATUS      ProcessorPOPr    (Processor* processor);

extern CMD_STATUS      ProcessorJMP     (Processor* processor);

extern CMD_STATUS      ProcessorJA      (Processor* processor);

extern CMD_STATUS      ProcessorJAE     (Processor* processor);

extern CMD_STATUS      ProcessorJB      (Processor* processor);

extern CMD_STATUS      ProcessorJBE     (Processor* processor);

extern CMD_STATUS      ProcessorJE      (Processor* processor);

extern CMD_STATUS      ProcessorJNE     (Processor* processor);

const CommandInfo OP_TABLE[CMD_TOTAL_NUMBER] =
{
    {UNKNOWN,    NULL,              NULL,  0},
    { PUSH,    "PUSH",     ProcessorPUSH,  1},
    {  ADD,     "ADD",      ProcessorADD,  0},
    {  SUB,     "SUB",      ProcessorSUB,  0},
    {  MUL,     "MUL",      ProcessorMUL,  0},
    {  DIV,     "DIV",      ProcessorDIV,  0},
    {  OUT,     "OUT",      ProcessorOUT,  0},
    {  HLT,     "HLT",      ProcessorHLT,  0},
    {PUSHr,   "PUSHr",    ProcessorPUSHr,  1},
    { POPr,    "POPr",     ProcessorPOPr,  1},
    {  JMP,     "JMP",      ProcessorJMP,  1},
    {   JA,      "JA",       ProcessorJA,  1},
    {  JAE,     "JAE",      ProcessorJAE,  1},
    {   JB,      "JB",       ProcessorJB,  1},
    {  JBE,     "JBE",      ProcessorJBE,  1},
    {   JE,      "JE",       ProcessorJE,  1},
    {  JNE,     "JNE",      ProcessorJNE,  1}
};
