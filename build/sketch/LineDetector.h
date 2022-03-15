#line 1 "d:\\Projects\\Arduino\\LineFollower\\LineDetector.h"
#ifndef LINE_DETECTOR
#define LINE_DETECTOR

#include "Arduino.h"

class LineDetector
{
    public: 
        uint8_t * pins; //pins that represents light sensors from left to right
        void tick();
        int GetValue(); //value beetween 0-1000 calculates the normalized line position from left to right
    
    private:
        bool lastWasLeft = false;;
};


#endif