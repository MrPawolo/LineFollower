#line 1 "d:\\Projects\\Arduino\\LineFollower\\MotorDriver.h"
#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H


#include "MyFuncs.h"
#include "Arduino.h"

class MotorDriver
{
public:
    //contructor
    MotorDriver(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, int vel_change, int max_val);

    void setupValues(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, int vel_Change, int max_val);
    //need to be called in setup()
    void setup();

    // call this func every loop if you've set accel_val or deaccel_vale greater than 0
    void tick();

    // reguire value between (-255,255)
    void setValueDirectly(int val);
    // doing the same thing but prevent from passing wrong value
    void setValueSmoothed(int val);

    int getValue();

private:
   
    unsigned long lastTime = 0;
    int actVel = 0;
    int desiredVel = 0;
    uint8_t fwdPin;
    uint8_t bwdPin;
    uint8_t pwmPin;
    int velChange = 0;
    int maxVel = 0;
};

#endif