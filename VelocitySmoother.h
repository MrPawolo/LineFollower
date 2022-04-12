#ifndef VELOCITYSMOOTHER
#define VELOCITYSMOOTHER

#include "SmootherBase.h"
#include "MyFuncs.h"

class VelocitySmoother : public SmootherBase
{
public:
    VelocitySmoother(float maxVel);
    float tick(float actVal, long deltaTime);

    float maxVel = 1;

private:
    float prevVal;
};

#endif

VelocitySmoother::VelocitySmoother( float maxVel)
{
    VelocitySmoother::maxVel = maxVel;
}

float VelocitySmoother::tick(float actVal, long deltaTime) //deltaTime millis
{
    float velocity = (actVal - prevVal) / deltaTime ; 
    velocity = clamp(velocity, -maxVel * deltaTime, maxVel * deltaTime);

    bool actDir = actVal > prevVal;
    bool futureDir = actVal > (prevVal + velocity);

    if (futureDir != actDir)
        prevVal = actVal;
    else
        prevVal += velocity;

    return prevVal;
}