#include "StrHash.h"

long long StrHash(const char* str) 
{
    long long hash = 5381;
    while (*str)
    {
        hash = hash + (hash << 5) + *str;
        str++;
    }

    return hash;
}