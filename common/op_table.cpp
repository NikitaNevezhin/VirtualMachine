#include "commands_info.h"
#include "../SPU/ProcessorCommands.h"


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// USER! GET THE FUCK OUT OF HERE!
// DO NOT CHANGE THE ORDER OF COMMANDS IN OP_TABLE
// I SAID DO NOT CHANGE THE ORDER OF COMMANDS IN OP_TABLE
// AND GET YOUR ASS OUT OF THIS FILE, RIGHT NOW, please
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const CommandInfo OP_TABLE[CMD_TOTAL_NUMBER] =
{
    {UNKNOWN,      NULL,                   0,                         NULL,      0},
    {   PUSH,    "PUSH",     StrHash("PUSH"),                ProcessorPUSH,      1},
    {    ADD,     "ADD",      StrHash("ADD"),              ProcessorArithm,      0},
    {    SUB,     "SUB",      StrHash("SUB"),              ProcessorArithm,      0},
    {    MUL,     "MUL",      StrHash("MUL"),              ProcessorArithm,      0},  
    {    DIV,     "DIV",      StrHash("DIV"),              ProcessorArithm,      0},
    {    OUT,     "OUT",      StrHash("OUT"),                 ProcessorOUT,      0},
    {    HLT,     "HLT",      StrHash("HLT"),                 ProcessorHLT,      0},
    {  PUSHr,   "PUSHr",    StrHash("PUSHr"),          ProcessorRegCommand,      1},
    {   POPr,    "POPr",     StrHash("POPr"),          ProcessorRegCommand,      1},
    {    JMP,     "JMP",      StrHash("JMP"),                 ProcessorJMP,      1},
    {     JA,      "JA",       StrHash("JA"),      ProcessorConditionalJMP,      1},
    {    JAE,     "JAE",      StrHash("JAE"),      ProcessorConditionalJMP,      1},
    {     JB,      "JB",       StrHash("JB"),      ProcessorConditionalJMP,      1},
    {    JBE,     "JBE",      StrHash("JBE"),      ProcessorConditionalJMP,      1},
    {     JE,      "JE",       StrHash("JE"),      ProcessorConditionalJMP,      1},
    {    JNE,     "JNE",      StrHash("JNE"),      ProcessorConditionalJMP,      1},
    {     IN,      "IN",       StrHash("IN"),                  ProcessorIN,      0},
    {   CALL,    "CALL",     StrHash("CALL"),                ProcessorCALL,      1},
    {    RET,     "RET",      StrHash("RET"),                 ProcessorRET,      0},
    {  PUSHM,   "PUSHM",    StrHash("PUSHM"),          ProcessorMemCommand,      1},
    {   POPM,    "POPM",     StrHash("POPM"),          ProcessorMemCommand,      1},
    {   POPV,    "POPV",     StrHash("POPV"),                ProcessorPOPV,      1},
    {  PAINT,   "PAINT",    StrHash("PAINT"),               ProcessorPAINT,      0},
    {   SQRT,    "SQRT",     StrHash("SQRT"),                ProcessorSQRT,      0}
};