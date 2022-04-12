#line 1 "d:\\Projects\\Arduino\\LineFollower\\LineDetector.h"
#ifndef LINE_DETECTOR
#define LINE_DETECTOR

#include "Arduino.h"

class LineDetector
{
    public: 
        LineDetector(uint8_t * pins, uint8_t lenght);
        uint8_t * pins; //pins that represents light sensors from left to right
        void setup();
        void tick();
        int GetValue(); //value beetween 0-1000 calculates the normalized line position from left to right
    
    private:
        
        bool lastWasLeft = false;
        uint8_t lenght;
        int * values;
        uint8_t minNoise;
        uint8_t maxNoise;
        int maxSize = 1000;
        int segmentSize;
        int actVal;
        int max = 1023;
        int min = 0;
};


#endif