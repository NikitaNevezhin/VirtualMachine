#include <stdio.h>
#include <stdlib.h>

#include "CPU/ProcessorStruct.h"
#include "CPU/ProcessorEngine.h"

int main(int argc, char* argv[])
{   
    if (argc != 2)  
    {   
        PRINT_RED("ERROR: one argument required!\n"
                   "Fuck you! I need just one file name: the one I read from\n");
        return EXIT_FAILURE;
    }

    const char* byte_code = argv[1];

    Processor processor = {};
    ProcessorInit(&processor, byte_code);

    RunProcessor(&processor);

    PROCESSOR_DUMP(&processor);

    return EXIT_SUCCESS;
}

// g++ main.cpp CPU/ProcessorCommands.cpp CPU/ProcessorEngine.cpp CPU/ProcessorStruct.cpp STACK/Stack.cpp STACK/StackErrorProcessor.cpp UTILS/FloatTools.cpp UTILS/FileTools.cpp -o build/main.exe -Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-security -Wformat=2 -Wignored-qualifiers -Winit-self -Winline -Wlogical-op -Wmain -Wmissing-declarations -Wno-missing-field-initializers -Wmissing-include-dirs -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wtype-limits -Wundef -Wunreachable-code -Wwrite-strings -fexceptions -g -pipe -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC -Wno-format -Wno-format-security