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

