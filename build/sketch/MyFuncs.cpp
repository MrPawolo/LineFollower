#line 1 "d:\\Projects\\Arduino\\LineFollower\\MyFuncs.cpp"
#include "MyFuncs.h"

int clamp(int inVal, int min, int max)
{
    if (inVal < min)
    {
        return min;
    }

    if (inVal > max)
    {
        return max;
    }

    return inVal;
}
float clamp(float inVal, float min, float max)
{
    if (inVal < min)
    {
        return min;
    }

    if (inVal > max)
    {
        return max;
    }

    return inVal;
}

int lerp(int a, int b, float t)
{
    return a*(1-t) +  b*t;
}
