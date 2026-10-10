#ifndef FileTools_cpp

#define FileTools_cpp

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <errno.h>

#include "UTILS/PrintColor.h"
#include  "STACK\stack.cpp"

#include "ProcessorStruct.h"

struct FileInfo
{
    int lines;
    const char* filename;
    int size;
    char* buffer;
    char** indexes;
};

int     StrCount            (char* string, char symb);

int     GetFileSize         (const char* filename);

int     ReadFromFile        (const char* filename, char destination[], int size);

void    WriteDelimeter      (const char* filename);

void    CreateFileInfo      (FileInfo *file_info, const char* filename);

void    PrintFileInfo       (FileInfo *file_info);

int     CreateCodeSegment   (Processor* processor, FileInfo *code_info);


int StrCount(char* string, char symb)  // helper function for ReadFromFile
{
    assert(string);

    int count = 0;
    int length = strlen(string);

    for (int i = 0; i < length; i++)
    {
        if (string[i] == symb)
            count++;
    }

    return count;
}

int GetFileSize(const char* filename)  //returns byte size (including '\r' and ' \n')
{   
    assert(filename);

    struct stat FileInfo = {};

    FILE* file = fopen(filename, "rb");

    if (file == NULL)
    {
        perror("ERROR: GetFileSize -> fopen()");
        abort();
    }

    stat(filename, &FileInfo);

    fclose(file);

    return FileInfo.st_size;
}

int ReadFromFile(const char* filename, char destination[], int size)
{   
    assert(filename);
    assert(destination);

    int read_bytes = 0;

    FILE* file = fopen(filename, "rb");

    if (file == NULL)
    {
        perror("ERROR: GetFileSize -> fopen()");
        abort();
    }

    read_bytes = fread(destination, sizeof(char), size, file);
    
    destination[size] = '\0';

    fclose(file);

    return read_bytes;
}

void WriteDelimeter(const char* filename)
{   
    assert(filename);

    FILE* file = fopen(filename, "a");

    if (file == NULL)
    {
        perror("ERROR: GetFileSize -> fopen()");
        abort();
    }

    fputc('\n', file);
    fputs("DELIMETER", file);

    for (int i = 0; i < 40; i++)
        fputc('*', file);
    fputc('\n', file);

    fclose(file);    
}

void CreateFileInfo(FileInfo *file_info, const char* filename)
{
    file_info->filename = filename;
    file_info->size = GetFileSize(filename); 
    file_info->buffer = (char*)calloc(file_info->size + 1, sizeof(char));  // file_size + 1 was made for adding '\0' at the end of the buffer

    if (file_info->buffer == NULL)
    {
        PRINT_RED("calloc returned NULL pointer in CreateFileInfo\n");
        abort();
    }

    ReadFromFile(filename, file_info->buffer, file_info->size);

    file_info->lines = StrCount(file_info->buffer, '\n') + 1;  // there is no '\n' for the last line of the file. That's why +1

    file_info->indexes = (char**)calloc(file_info->lines, sizeof(char*));

    if (file_info->indexes == NULL)
    {
        PRINT_RED("calloc returned NULL pointer in CreateFileInfo\n");
        abort();
    }

    file_info->indexes[0] = file_info->buffer;
    char* next_slash_r = NULL;  
    char* next_slash_n = NULL;

    for (int i = 0; i < file_info->lines - 1; i++)
    {   
        next_slash_r = strchr(file_info->indexes[i], '\r');
        next_slash_n = next_slash_r + 1;
        (*next_slash_r) = '\0';
        (*next_slash_n) = '\0';
        file_info->indexes[i + 1] = next_slash_n + sizeof(char);
    }
}

void PrintFileInfo(FileInfo *file_info)
{
    printf("File name: %s\n", file_info->filename);
    printf("File size: %d\n", file_info->size);
    printf("Lines in file: %d\n", file_info->lines);
}

int CreateCodeSegment(Processor* processor, FileInfo *code_info)
{
    // we have max 2 numbers in a single line. total_elems of code segment is less than lines * 2
    int* code_segment = (int*)calloc(code_info->lines * 2, sizeof(int));

    int read_args = 0;  // the amount of arguments found by sscanf in a line: 1 for ADD, SUB, MULT etc and 2 for PUSH, e.g., PUSH 20
    stack_elem_t command_arg = 0; // the argument for the command (for PUSH)
    int command = 0;

    int ip = 0;  // instruction pointer

    for (int i = 0; i < code_info->lines; i++)
    {
        read_args = sscanf(code_info->indexes[i], "%d %d", &command, &command_arg);

        if (read_args == 0) 
        {
            PRINT_RED("Failed at reading %s:%d in %s\n", code_info->filename, i + 1, __func__);
            abort();
        }

        code_segment[ip++] = command;
        if (read_args == 2) code_segment[ip++] = command_arg;
    }

    processor->code_length = ip;
    processor->code_segment = code_segment;

    return 0;
}

#endif