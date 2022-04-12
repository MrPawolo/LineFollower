#ifndef DELTA_TIME
#define DELTA_TIME

#include "Arduino.h"

class DeltaTime_
{
public:
    void tick();
    unsigned long deltaTime = 1;

private:
    unsigned long prevTime;
};


#endif

void DeltaTime_::tick()
{
    unsigned long actTime = millis();
    deltaTime = actTime - prevTime;
    prevTime = actTime;
}

DeltaTime_ Time; // to work propperly .tick() need to be invoked every loop, best place is at the end of the loop
