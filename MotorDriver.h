#ifndef MOTOR_DRIVER
#define MOTOR_DRIVER

class MotorDriver
{
public:
    MotorDriver(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, float accel_val, float deaccel_val, float max_val);

    void setup(uint8_t fwd_pin, uint8_t bwd_pin, uint8_t pwm_pin, float accel_val, float deaccel_val, float max_val);

    // call this func every loop if you've set accel_val or deaccel_vale greater than 0
    void tick();

    // require value between (-1,1)
    void setNormalizedValue(float val);
    // doing the same thing but prevent from passing wrong value
    void setNormalizedValueSafe(float val);

    // reguire value between (-255,255)
    void setValue(float val);
    // doing the same thing but prevent from passing wrong value
    void setValueSafe(float val);
    float getValue();

    uint8_t fwd_pin;
    uint8_t bwd_pin;
    uint8_t pwm_pin;
    float accel_val;
    float deaccel_val;
    float max_val;
};

#endif