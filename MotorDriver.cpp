#include "MotorDriver.h"

MotorDriver::MotorDriver(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, float accel_val, float deaccel_val, float max_val)
{
    MotorDriver::setup(fwd_pin, bwd_pin,pwm_pin,accel_val,deaccel_val,max_val);
}

void MotorDriver::setup(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, float accel_val, float deaccel_val, float max_val)
{
    MotorDriver::fwd_pin = fwd_pin;
    MotorDriver::bwd_pin = bwd_pin;
    MotorDriver::pwm_pin = pwm_pin;
    MotorDriver::accel_val = accel_val;
    MotorDriver::deaccel_val = deaccel_val;
    MotorDriver::max_val = max_val;
};