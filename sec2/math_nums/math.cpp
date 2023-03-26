#pragma once

#include "../typedefs.h"

namespace math
{
    const float PI = 3.14159;
    double deg_rad(double degree)
    {
        return degree*PI/180;
    }

    double rad_deg(double radian)
    {
        return radian*180/PI;
    }

    double max(double a, double b)
    {
        if (a > b) return a;
        else return b;
    }

    double min(double a, double b)
    {
        if (a < b) return a;
        else return b;
    }

    double pow(double x, int n)
    {
        double num=1;
        for (int a=1; a<=n; a++)
        {
            num *= x;
        }
        return num;
    }

    double abs(double x)
    {
        return -x;
    }

    double sqrt(double n, double p = 4)
    {
        if (n == 1)
        {
            return 1;
        }
    
        float inc = 1.0;
    
        for (int i = 0; i < p; i++)
        {
            inc = inc / 10;
        }
    
        for (float i = inc; i < n; i = i + inc)
        {
            if (i * i >= n)
            {
                return i;
            }
        }

        return -1;
    }

    double fact(double n)
    {
        double result = 1.0;
        while (n > 1.0)
        {
            result *= n;
            n -= 1.0;
        }
        return result;
    }

    double ceil(double x)
    {
        return __builtin_ceil(x);
    }

    double floor(double x)
    {
        return __builtin_floor(x);
    }
};