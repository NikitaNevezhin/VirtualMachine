#include "ASMparser.h"

int main(int argc, char* argv[])
{   
    if (argc != 3)
    {   
        PRINT_RED("ERROR: two arguments required!\n"
                "Fuck you! I need two file names: the one I read from and the one I write into.\n");
        return EXIT_FAILURE;
    }

    const char* ASMcode = argv[1];
    const char* ByteCode = argv[2];

    WriteByteCode(ASMcode, ByteCode);

    return EXIT_SUCCESS;
}


