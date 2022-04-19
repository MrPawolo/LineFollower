#ifndef LINE_DETECTOR_2
#define LINE_DETECTOR_2

#include "MyFuncs.h"

class LineDetector
{
public:
    LineDetector(uint8_t pin);
    void calibrate();
    void resetCalibration();
    void acceptCalibrateValues();
    void setMinMax(int min, int max);
    int getValue();
private:
    uint8_t pin;
    int lowNoiseLevel = 0;
    int highNoiseLevel = 1023;

    int calibrateLowNoise = 1023;
    int calibrateHighNoise = 0;
};
#endif

LineDetector::LineDetector(uint8_t pin)
{
    LineDetector::pin = pin;
}

void LineDetector::setMinMax(int min, int max)
{
    LineDetector::lowNoiseLevel = min;
    LineDetector::highNoiseLevel = max;
}

void LineDetector::resetCalibration()
{
    lowNoiseLevel = 0;
    highNoiseLevel = 1023;
    calibrateHighNoise = 0;
    calibrateLowNoise = 1023;
}
void LineDetector::calibrate()
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
void LineDetector::acceptCalibrateValues()
{
    lowNoiseLevel = calibrateLowNoise;
    highNoiseLevel = calibrateHighNoise;
}

int LineDetector::getValue()
{
    int val = analogRead(pin);
    val = clamp(val,lowNoiseLevel,highNoiseLevel);
    val = map(val, lowNoiseLevel, highNoiseLevel, 0 , 1023);
    return val;
}