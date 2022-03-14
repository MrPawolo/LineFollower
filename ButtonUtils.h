#ifndef BUTTON_UTILS
#define BUTTON_UTILS

#include "Arduino.h"

//#define DISABLE_PERFORM_DEBOUNCE_CORRECTION

class ButtonUtils
{
public:
    ButtonUtils(int pin, void (*on_positive_edge)(), void (*on_negative_edge)(), void (*on_clicked)()); //pass functions like that "*FOO"

#ifndef DISABLE_PERFORM_DEBOUNCE_CORRECTION
    ButtonUtils(int pin, void (*on_positive_edge)(), void (*on_negative_edge)(), void (*on_clicked)(), int debounceTimeMicros);
#endif

    void setup();
    void tick();//need to be called every loop
    void (*onPositiveEdge)() = nullptr; //function that will be called on Positive edge detect call like that "*FOO"
    void (*onNegativeEdge)() = nullptr; //function that will be called on Positive edge detect call like that "*FOO"
    void (*onClicked)() = nullptr; //function that will be called on Positive edge detect call like that "*FOO"

private:
    bool lastState = false;
    int pin;


//#define PERFORM_DEBOUNCE_CORRECTION //if is uncomented then you need to pass a value to debounce time;
#ifndef DISABLE_PERFORM_DEBOUNCE_CORRECTION
public:
    int debounceTimeMicros = 200;
private:

    bool checkingState = false;
    int prevState = HIGH;
    unsigned long prevTime = 0;
    unsigned long actTime = 0;
    void CheckState(int state);
#endif 


    void PerformEvents(int state);
};

#endif