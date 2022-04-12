#line 1 "d:\\Projects\\Arduino\\LineFollower\\SmootherBase.h"
#ifndef SMOOTHERBASE
#define SMOOTHERBASE

#include "Arduino.h"

class SmootherBase
{
    public:
    float virtual tick(float actVal);
    float virtual tick(float actVal, long deltaTime) = 0;
    float weight = 1;

    private:
    unsigned long prevTime;
    unsigned long getDeltaTime();
};

float SmootherBase::tick(float actVal)
{
    long deltaTime = getDeltaTime();
    return tick(actVal, deltaTime);
}

unsigned long SmootherBase::getDeltaTime()
{
    unsigned long actTime = millis();
    unsigned long deltaTime = actTime - prevTime;
    prevTime = actTime;
    return deltaTime;
}

#endif

