#line 1 "d:\\Projects\\Arduino\\LineFollower\\LineDetector.cpp"
#include "LineDetector.h"
#include "MyFuncs.h"

struct LightPair
{
    public:
    LightPair(int value, uint8_t index)
    {
        LightPair::value = value;
        LightPair::index = index;
    }
    uint8_t index;
    int value;
    bool wasSet = false;
};

LineDetector::LineDetector(uint8_t *pins, uint8_t lenght)
{
    LineDetector::pins = pins;
    LineDetector::lenght = lenght; 
    values = new int(lenght);
    if (lenght != 1)
        segmentSize = maxSize / (lenght - 1);
    else 
        segmentSize = maxSize;
}
void LineDetector::setup()
{

}
void LineDetector::tick()
{
    for (int i = 0; i < lenght; i++)
    {
        values[i] = map(analogRead(pins[i]), min, max, 1, 1024);
    }

    //searching for lowest value in segment
    int maxLight = 1024; //256*256
    LightPair lowestLight(1024,0);
    LightPair secondLowestLight(1024,1);
    uint8_t startIndex = 0;



    for(int i = 0; i < lenght ; i++)
    {
        int actLight = values[i];

        if(actLight < lowestLight.value)
        {
            lowestLight.value = values[i];
            lowestLight.index = i;
            lowestLight.wasSet = true;
            continue;
        }
        if(actLight < secondLowestLight.value)
        {
            secondLowestLight.value = values[i];
            secondLowestLight.index = i;
            secondLowestLight.wasSet = true;
        }
    }

    if(lowestLight.wasSet == false)
    {
        actVal = 0;
        return;
    }
    if(secondLowestLight.wasSet == false)
    {
         actVal = lowestLight.index * segmentSize;
         return;
    }

    int weightSum = lowestLight.value + secondLowestLight.value;
    float weight = lowestLight.value / (float)weightSum;

    int lowestVal = lowestLight.index * segmentSize;
    int secondVal = secondLowestLight.index * segmentSize;
    int val = lerp(lowestVal, secondVal, weight);
    actVal = val;

}
int LineDetector::GetValue()
{
    //temp solution
    return actVal;
}