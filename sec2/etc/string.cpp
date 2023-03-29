#pragma once

#include "../typedefs.h"

uint_16 str_len(const char* str)
{
    uint_16 strlen = 0;
    uint_16 strptr = 0;
    while (str[strptr] != 0)
    {
        strlen++;
        strptr++;
    }
    return strlen;
}

uint_8 str_cmp(const char* str1, const char* str2)
{
    uint_16 str1_len = str_len(str1);
    uint_16 str2_len = str_len(str2);

    if (str1_len != str2_len) return 0;

    for (int i = 0; i < str1_len; i++)
    {
        if (str1[i] != str2[i]) return 0;
    }

    return 1;
}