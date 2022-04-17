#line 1 "d:\\Projects\\Arduino\\LineFollower\\PID.h"
#ifndef PID_INCLUDED
#define PID_INCLUDED

#include "Arduino.h"

#define FIXED_STEP
#define onlyP

class PID
{
public:
    PID(float kp, float ki, float kd, long maxOutput);
    void setValues(float kp, float ki, float kd, long maxOutput);
    void setValue(float value);
    float tick(float actValue);
    float tick(float actValue, unsigned long deltaTime);

    long value;
    float kp = 0;
    float ki = 0;
    float kd = 0;
    long maxOutput = 0;

    unsigned long fixedStepTime = 10000; // microseconds

private:
    int desiredValue = 0;

    int lastError;
    long errorSum;

    unsigned long lastTime = 0;
};

#endif

PID::PID(float kp, float ki, float kd, long maxOutput)
{
    setValues(kp, ki, kd, maxOutput);
}
void PID::setValue(float value)
{
    desiredValue = value;
}

void PID::setValues(float kp, float ki, float kd, long maxOutput)
{
    PID::kp = kp;
    PID::ki = ki;
    PID::kd = kd;
    PID::maxOutput = maxOutput;
}

float PID::tick(float actValue)
{
    unsigned long actTime = micros();
    unsigned long deltaTime = actTime = lastTime;
    lastTime = actTime;
    return tick(actValue, deltaTime);
}

float PID::tick(float actValue, unsigned long deltaTime)
{
    long error = desiredValue - actValue;
    long out = 0;

    out += error * ki;

#ifndef onlyP
    long area = (lastError + error) / 2 * deltaTime;
    errorSum += area;
    // if(abs(out + errorSum * ki) < abs(maxOutput))
    out += errorSum * ki;

    long errorChange = (error - lastError) / deltaTime;
    // if(abs(out + errorChange * kd) < abs(maxOutput))
    out += errorChange * kd;

    lastError = error;

#endif

    value = out;
}