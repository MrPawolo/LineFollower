#line 1 "d:\\Projects\\Arduino\\LineFollower\\LineDetector2.h"
#ifndef LINE_DETECTOR_2
#define LINE_DETECTOR_2

#include "MyFuncs.h"

class LineDetector2
{
public:
    LineDetector2(uint8_t pin);
    void calibrate();
    void resetCalibration();
    void acceptCalibrateValues();
    int getValue();
private:
    uint8_t pin;
    int lowNoiseLevel = 0;
    int highNoiseLevel = 1023;

    int calibrateLowNoise = 1023;
    int calibrateHighNoise = 0;
};
#endif

LineDetector2::LineDetector2(uint8_t pin)
{
    LineDetector2::pin = pin;
}

void LineDetector2::resetCalibration()
{
    lowNoiseLevel = 0;
    highNoiseLevel = 1023;
    calibrateHighNoise = 0;
    calibrateLowNoise = 1023;
}
void LineDetector2::calibrate()
{
    int val = analogRead(pin);
    if(val < calibrateLowNoise)
    {
        calibrateLowNoise = val;
        return;
    }
    if(val > calibrateHighNoise)
    {
        calibrateHighNoise = val;
        return;
    }
}
void LineDetector2::acceptCalibrateValues()
{
    lowNoiseLevel = calibrateLowNoise;
    highNoiseLevel = calibrateHighNoise;
}

int LineDetector2::getValue()
{
    int val = analogRead(pin);
    val = clamp(val,lowNoiseLevel,highNoiseLevel);
    val = map(val, lowNoiseLevel, highNoiseLevel, 0 , 1023);
    return val;
}