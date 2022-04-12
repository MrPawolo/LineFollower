#line 1 "d:\\Projects\\Arduino\\LineFollower\\PID.h"
#ifndef PID_INCLUDED
#define PID_INCLUDED

#include "Arduino.h"

#define FIXED_STEP


class PID
{
public:
    PID(float kp, float ki, float kd);
    void setValues(float kp, float ki, float kd);
    void setValue(int value);
    long tick(int actValue);
    long tick(int actValue, unsigned long deltaTime);

    long value;
    float kp = 0;
    float ki = 0;
    float kd = 0;
    long maxOutput = 0;

    unsigned long fixedStepTime = 10000; //microseconds

private:
    int desiredValue = 0;

    int lastError;
    long errorSum;

    unsigned long lastTime = 0;
};

#endif


PID::PID(float kp, float ki, float kd)
{
    setValues(kp,ki,kd);
}

void PID::setValues(float kp, float ki, float kd)
{
    PID::kp = kp;
    PID::ki = ki;
    PID::kd = kd;
}

long PID::tick(int actValue)
{
    unsigned long actTime = micros();
    unsigned long deltaTime = actTime = lastTime;
    lastTime = actTime;
    return tick(actValue, deltaTime);
}

long PID::tick(int actValue, unsigned long deltaTime)
{
    long error = desiredValue - actValue;
    long out = 0;

    out += error * ki;

    long area = (lastError + error) / 2 * deltaTime;
    errorSum += area;
    out += errorSum * ki;

    long errorChange = (error - lastError) / deltaTime;
    out += errorChange * kd;

    lastError = error;

    value = out;
}