#pragma once

#include "../typedefs.h"

namespace math
{
    uint_64 seed = 0;
    const float PI = 3.14159;

    typedef struct
    {
        int x;
        int y;
    } point;
    

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

    double sin(double x)
    {
        double sum = 0;
        for (int n = 0; n <= 10; n++) {
            sum += pow(-1, n) * pow(x, 2*n+1) / fact(2*n+1);
        }
        return sum;
    }

    double cos(double x)
    {
        double sum = 0;
        for (int n = 0; n <= 10; n++) {
            sum += pow(-1, n) * pow(x, 2*n) / fact(2*n);
        }
        return sum;
    }

    double tan(double x)
    {
        double sum = 0;
        double term = x;
        for (int n = 1; n <= 10; n++) {
            sum += term;
            term = pow(-1, n) * 2 * pow(x, 2*n+1) / fact(2*n+1);
        }
        return sum;
    }

    double ceil(double x)
    {
        return __builtin_ceil(x);
    }

    double floor(double x)
    {
        return __builtin_floor(x);
    }

    void srand(uint_64 seed_set)
    {
        seed = seed_set;
    }

    uint_64 rand()
    {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }
};