#include "ASMparser.h"

// .\build/assembler.exe source/ASM_factorial.txt source/factorial.bin source/factorial.txt
// .\build/assembler.exe source/ASM_fibonacci.txt source/fibonacci.bin source/fibonacci.txt
// .\build/assembler.exe source/ASM_code.txt source/byte_code.bin source/byte_code.txt

// g++ ASM/assembler.cpp ASM/ASMparser.cpp CPU/CommandCodes.cpp CPU/ProcessorCommands.cpp CPU/ProcessorEngine.cpp CPU/ProcessorStruct.cpp STACK/Stack.cpp STACK/StackErrorProcessor.cpp UTILS/StrHash.cpp UTILS/FloatTools.cpp UTILS/FileTools.cpp -o build/assembler.exe -Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-security -Wformat=2 -Wignored-qualifiers -Winit-self -Winline -Wlogical-op -Wmain -Wmissing-declarations -Wno-missing-field-initializers -Wmissing-include-dirs -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wtype-limits -Wundef -Wunreachable-code -Wwrite-strings -fexceptions -g -pipe -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC -Wno-format -Wno-format-security

int main(int argc, char* argv[])
{   
    if (argc != 4)
    {   
        PRINT_RED("ERROR: two arguments required!\n"
                "Fuck you! I need three file names: the one I read from and " 
                 "the one I write into bin info and the one I write into normal info.\n");
        return EXIT_FAILURE;
    }

    const char* ASMcode = argv[1];
    const char* BinCode = argv[2];
    const char* ByteCode = argv[3];

    WriteByteCode(ASMcode, BinCode, true);    // is_bin = true
    WriteByteCode(ASMcode, ByteCode, false);  // is_bin = false

    return EXIT_SUCCESS;
}


