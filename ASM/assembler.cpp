#include "ASMparser.h"

// .\build/assembler.exe source/ASM_factorial.txt source/factorial.bin source/factorial.txt
// .\build/assembler.exe source/ASM_fibonacci.txt source/fibonacci.bin source/fibonacci.txt
// .\build/assembler.exe source/ASM_code.txt source/byte_code.bin source/byte_code.txt


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


