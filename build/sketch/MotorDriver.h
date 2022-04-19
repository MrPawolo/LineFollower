#line 1 "d:\\Projects\\Arduino\\LineFollower\\MotorDriver.h"

#include "MyFuncs.h"
#include "Arduino.h"

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

class MotorDriver
{
public:
    //contructor
    MotorDriver(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, int min_val,  int max_val);
    void setupValues(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin,int min_val,   int max_val);
    //need to be called in setup()
    void setup();
    void noise();
    void stopNoise();
    // reguire value between (-255,255)
    void setValueDirectly(int val);
    int getValue();
private: 
    uint8_t fwdPin;
    uint8_t bwdPin;
    uint8_t pwmPin;
    int maxVel = 0;
    int minVel = 0;
    int actVel;
};
#endif
