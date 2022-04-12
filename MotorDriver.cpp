
#include "MotorDriver.h"

MotorDriver::MotorDriver(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, int max_val)
{
    MotorDriver::setupValues(fwd_pin, bwd_pin, pwm_pin, max_val);
}

void MotorDriver::setupValues(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, int max_val)
{
    MotorDriver::fwdPin = fwd_pin;
    MotorDriver::bwdPin = bwd_pin;
    MotorDriver::pwmPin = pwm_pin;
    MotorDriver::maxVel = clamp(max_val, 0, 255);
};

void MotorDriver::setup()
{
    pinMode(fwdPin, OUTPUT);
    pinMode(bwdPin, OUTPUT);
    pinMode(pwmPin, OUTPUT);
}



void MotorDriver::setValueDirectly(int val)
{
    if (val == 0)
    {
        digitalWrite(bwdPin, LOW);
        digitalWrite(fwdPin, LOW);
        analogWrite(pwmPin, 0);
        return;
    }

    if (val > 0)
    {
        digitalWrite(bwdPin, LOW);
        digitalWrite(fwdPin, HIGH);
    }
    else
    {
        digitalWrite(fwdPin, LOW);
        digitalWrite(bwdPin, HIGH);
    }
    val = abs(val);
    actVel = map(val, 0,255,0,maxVel);
    analogWrite(pwmPin, actVel);
}

int MotorDriver::getValue()
{
    return actVel;
}
