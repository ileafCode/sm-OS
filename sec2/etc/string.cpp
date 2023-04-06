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

    char* str_lower(char* str)
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

    char* str_upper(char* str)
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

    char chr_upper(char chr)
    {
        if(chr >= 'a' && chr <= 'z') 
		{ 
            chr = chr - 32; 
        }
        return chr;
    }

    char chr_lower(char chr)
    {
        if(chr >= 'A' && chr <= 'Z') 
		{ 
            chr = chr + 32; 
        }
        return chr;
    }

    char* sub_str(char* str, uint_64 start, uint_64 len)
    {
        char* res = 0;

        uint_16 x = 0;

        start += 1;

        for (int i = start; i < (start + len); i++)
        {
            res[x] = str[i];
            x++;
        }
        return res;
    }

    char* str_tok(char* str, const char* delim)
    {
        char* last_token = NULL;
        char* token = NULL;

        if (str != NULL)
        {
            last_token = str;
        }
        else if (last_token == NULL)
        {
            return NULL;
        }

        token = last_token;
        while (*last_token != '\0')
        {
            for (const char* d = delim; *d != '\0'; ++d)
            {
                if (*last_token == *d)
                {
                    *last_token = '\0';
                    last_token++;
                    return token;
                }
            }
            last_token++;
        }

        return token;
    }

    char* str_chr(char* str, int c)
    {
        while (*str != 0)
        {
            if (*str == c) return str;
            str++;
        }
        return nullptr;
    }

    void str_cpy(char *str1, char* str2)
    {
        int n = str_len(str2);
        for (int i = 0; i <= n; i++)
        {
            str1[i] = str2[i];
        }
    }


    uint_64 str_int(const char* input)
    {
        int i = 0;

        for (char c = 0; c < str_len(input); c++)
        {
            if (input[c] >= '0' && input[c] <= '9') 
            {
                i = i * 10 + (input[c] - '0');
            }
            else
            {
                return 1;
            }
        }

        return i;
    }

    uint_64 str_backspace(char* str, char c)
    {
	    uint_64 i = str_len(str);
	    i--;
	    while(i)
	    {
		    i--;
		    if(str[i] == c)
		    {
			    str[i + 1] = 0;
			    return 1;
		    }
	    }
	    return 0;
    }

    uint_64 str_split(char* str, char delim)
    {
	    uint_64 n = 0;
	    uint_32 i = 0;
	    while(str[i])
	    {
		    if(str[i] == delim)
		    {
			    str[i] = 0;
			    n++;
		    }
		    i++;
	    }
	    n++;
	    return n;
    }
}