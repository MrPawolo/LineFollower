#ifndef BUTTON_UTILS
#define BUTTON_UTILS

#include "Arduino.h"

class ButtonUtils
{
public:
    ButtonUtils(int pin, void (*on_positive_edge)(), void (*on_negative_edge)(), void (*on_clicked)());

    void tick();

    void (*onPositiveEdge)() = nullptr;
    void (*onNegativeEdge)() = nullptr;
    void (*onClicked)() = nullptr;

private:
    bool lastState = false;
    int pin;
    
};

#endif