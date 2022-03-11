#line 1 "d:\\Projects\\Arduino\\LineFollower\\MotorDriver.cpp"

#include "MotorDriver.h"

MotorDriver::MotorDriver(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, int accel_val, int max_val)
{
    MotorDriver::setupValues(fwd_pin, bwd_pin, pwm_pin, accel_val, max_val);
}

void MotorDriver::setupValues(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, int accel_val, int max_val)
{
    MotorDriver::fwdPin = fwd_pin;
    MotorDriver::bwdPin = bwd_pin;
    MotorDriver::pwmPin = pwm_pin;
    MotorDriver::velChange = max(accel_val, 0);
    MotorDriver::maxVel = clamp(max_val, 0, 255);
};

void MotorDriver::setup()
{
    pinMode(fwdPin, OUTPUT);
    pinMode(bwdPin, OUTPUT);
    pinMode(pwmPin, OUTPUT);
}

void MotorDriver::tick()
{
    unsigned long time = millis();
    int deltaTime = time - lastTime;
    lastTime = time;
    int velDifference = desiredVel - actVel; // accelerating 100 = 255 - 100; or 100 = -255 - -100  deaccelerating -100 = 0 - 100  100 = 0 - -100;

    if (velDifference == 0)
        return;

    int deltaVel = sign(velDifference) * velChange;

    if (abs(deltaVel) > abs(velDifference))
    {
        actVel = desiredVel;
    }
    else
    {
        actVel += deltaVel;
    }

    setValueDirectly(actVel);
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
    analogWrite(pwmPin, min(val, maxVel));
}


void MotorDriver::setValueSmoothed(int val)
{
    desiredVel = val;
}

int MotorDriver::getValue()
{
    return actVel;
}
