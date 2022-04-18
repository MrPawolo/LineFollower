#line 1 "d:\\Projects\\Arduino\\LineFollower\\PID.h"
#ifndef PID_INCLUDED
#define PID_INCLUDED

#include "Arduino.h"

class PID
{
public:
    PID(float kp, float ki, float kd, float maxOutput, float maxI, float maxD);
    void setValues(float kp, float ki, float kd, float maxOutput, float maxI, float maxD);
    void setValue(float value);
    float tick(float actValue);
    float tick(float actValue, unsigned long deltaTime);

    float value;
    float kp = 0;
    float ki = 0;
    float kd = 0;
    float maxOutput = 0;
    float maxI = 0;
    float maxD = 0;

    unsigned long fixedStepTime = 10000; // microseconds

private:
    float desiredValue = 0;

    float lastError;
    float errorSum;

    unsigned long lastTime = 0;
};

#endif

PID::PID(float kp, float ki, float kd, float maxOutput, float maxI, float maxD)
{
    setValues(kp, ki, kd, maxOutput,  maxI,  maxD);
}
void PID::setValue(float value)
{
    desiredValue = value;
}

void PID::setValues(float kp, float ki, float kd, float maxOutput, float maxI, float maxD)
{
    PID::kp = kp;
    PID::ki = ki;
    PID::kd = kd;
    PID::maxOutput = maxOutput;
    PID::maxI = maxI;
    PID::maxD = maxD;
}

float PID::tick(float actValue)
{
    unsigned long actTime = micros();
    unsigned long deltaTime = actTime - lastTime;
    lastTime = actTime;
    //Serial.println("DT: " + String(deltaTime));
    return tick(actValue, deltaTime);
}

float PID::tick(float actValue, unsigned long deltaTime)
{
    float out = 0;

    float error = desiredValue - actValue;
//---------------P---------------------
    out += error * kp;

#ifndef onlyP

//--------------I-----------------------
    float area = (lastError + error) / 2 * deltaTime / 1000000.0f;
    float sum = area * ki;

#ifdef clampI
    float diff = maxI - abs(errorSum + sum);
    errorSum += diff > 0 ? sum : 0;
#else
    errorSum += sum;
#endif
    out += errorSum;


//--------------D-----------------------
if(deltaTime != 0){
    float errorChange = (error - lastError) / deltaTime / 1000000.0f;
#ifdef clampD
    float diff = maxD - abs(sum);
    out += diff > 0 ? errorChange * kd : 0;
#else
    out += (errorChange * kd);
#endif
}

     lastError = error;
 #endif

#ifdef clampPIDout
    out = clamp(out, -maxOutput, maxOutput);
#endif



    value = out;
    return out;
}