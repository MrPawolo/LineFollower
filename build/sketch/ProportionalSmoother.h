#line 1 "d:\\Projects\\Arduino\\LineFollower\\ProportionalSmoother.h"
#ifndef PROP_SMOOTHER
#define PROP_SMOOTHER

#include "SmootherBase.h"

class ProportionalSmoother : public SmootherBase
{
    public:
    ProportionalSmoother(float weight);
    float tick(float actVal, long deltaTime);
    private:
    float prevVal;
};
#endif

ProportionalSmoother::ProportionalSmoother(float weight)
{
    ProportionalSmoother::weight = weight;
}

float safeLerp(float a, float b, float t)
{
  if(t > 1)
    t = 1;
  return a * (1-t) + b *t;
}

float ProportionalSmoother::tick(float actVal, long deltaTime) //deltaTime millis
{
    prevVal = safeLerp(prevVal, actVal, deltaTime * weight);
    return prevVal;
}