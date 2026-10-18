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

extern CMD_STATUS      ProcessorIN      (Processor* processor);

extern CMD_STATUS      ProcessorCALL    (Processor* processor);

extern CMD_STATUS      ProcessorRET     (Processor* processor);

extern CMD_STATUS      ProcessorPUSHM   (Processor* processor);

extern CMD_STATUS      ProcessorPOPM    (Processor* processor);

extern CMD_STATUS      ProcessorPOPV    (Processor* processor);

extern CMD_STATUS      ProcessorPAINT   (Processor* processor);

extern CMD_STATUS      ProcessorSQRT    (Processor* processor);

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// USER! GET THE FUCK OUT OF HERE!
// DO NOT CHANGE THE ORDER OF COMMANDS IN OP_TABLE
// I SAID DO NOT CHANGE THE ORDER OF COMMANDS IN OP_TABLE
// AND GET YOUR ASS OUT OF THIS FILE, RIGHT NOW, please
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const CommandInfo OP_TABLE[CMD_TOTAL_NUMBER] =
{
    {UNKNOWN,    NULL,                0,             NULL,  0},
    { PUSH,    "PUSH",  StrHash("PUSH"),    ProcessorPUSH,  1},
    {  ADD,     "ADD",   StrHash("ADD"),     ProcessorADD,  0},
    {  SUB,     "SUB",   StrHash("SUB"),     ProcessorSUB,  0},
    {  MUL,     "MUL",   StrHash("MUL"),     ProcessorMUL,  0},
    {  DIV,     "DIV",   StrHash("DIV"),     ProcessorDIV,  0},
    {  OUT,     "OUT",   StrHash("OUT"),     ProcessorOUT,  0},
    {  HLT,     "HLT",   StrHash("HLT"),     ProcessorHLT,  0},
    {PUSHr,   "PUSHr", StrHash("PUSHr"),   ProcessorPUSHr,  1},
    { POPr,    "POPr",  StrHash("POPr"),    ProcessorPOPr,  1},
    {  JMP,     "JMP",   StrHash("JMP"),     ProcessorJMP,  1},
    {   JA,      "JA",    StrHash("JA"),      ProcessorJA,  1},
    {  JAE,     "JAE",   StrHash("JAE"),     ProcessorJAE,  1},
    {   JB,      "JB",    StrHash("JB"),      ProcessorJB,  1},
    {  JBE,     "JBE",   StrHash("JBE"),     ProcessorJBE,  1},
    {   JE,      "JE",    StrHash("JE"),      ProcessorJE,  1},
    {  JNE,     "JNE",   StrHash("JNE"),     ProcessorJNE,  1},
    {   IN,      "IN",    StrHash("IN"),      ProcessorIN,  0},
    { CALL,    "CALL",  StrHash("CALL"),    ProcessorCALL,  1},
    {  RET,     "RET",   StrHash("RET"),     ProcessorRET,  0},
    {PUSHM,   "PUSHM", StrHash("PUSHM"),   ProcessorPUSHM,  1},
    {POPM,     "POPM",  StrHash("POPM"),    ProcessorPOPM,  1},
    { POPV,    "POPV",  StrHash("POPV"),    ProcessorPOPV,  1},
    {PAINT,   "PAINT", StrHash("PAINT"),   ProcessorPAINT,  0},
    { SQRT,    "SQRT",  StrHash("SQRT"),    ProcessorSQRT,  0}
};
