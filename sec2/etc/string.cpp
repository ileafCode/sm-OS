#pragma once

#include "../typedefs.h"

namespace string
{
    uint_16 str_len(const char* str)
    {
        uint_16 strlen = 0;
        while (str[strlen] != 0)
        {
            strlen++;
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

    const char* str_lower(char* str)
    {
        int i = 0;
 
        while(str[i] != 0) 
        { 
            if(str[i] >= 'A' && str[i] <= 'Z') 
	    	{ 
                str[i] = str[i] + 32; 
            } 
            i++; 
        }
        return str;
    }

    const char* str_upper(char* str)
    {
        int i = 0; 
 
        while(str[i] != 0) 
        { 
            if(str[i] >= 'a' && str[i] <= 'z') 
		    { 
                str[i] = str[i] - 32; 
            } 
            i++; 
        }
        return str;
    }
}