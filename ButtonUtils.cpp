#include "ButtonUtils.h"

ButtonUtils::ButtonUtils(int pin, void (*on_positive_edge)(), void (*on_negative_edge)(), void (*on_clicked)())
{
    onPositiveEdge = on_positive_edge;
    onClicked = on_clicked;
    onNegativeEdge = on_negative_edge;
    ButtonUtils::pin = pin;
}

#ifndef DISABLE_PERFORM_DEBOUNCE_CORRECTION
ButtonUtils::ButtonUtils(int pin, void (*on_positive_edge)(), void (*on_negative_edge)(), void (*on_clicked)(), int debounceTimeMicros)
{
    ButtonUtils(pin, *on_positive_edge, *on_negative_edge, *on_clicked);
    ButtonUtils::debounceTimeMicros = debounceTimeMicros;
}
#endif

void ButtonUtils::setup()
{
    pinMode(pin,INPUT_PULLUP);
}

#ifndef DISABLE_PERFORM_DEBOUNCE_CORRECTION
void ButtonUtils::CheckState(int state) 
{
    if(checkingState)
    {
        unsigned long actTime = micros();
        unsigned long time = actTime - prevTime;
        prevTime = actTime;

        if(time > debounceTimeMicros)
        {
            checkingState = false;
        }
        
        return;
    }

    if(state == prevState)
        return;

    prevTime = micros();
    checkingState = true;
    prevState = state;
    PerformEvents(state);
}
#endif

void ButtonUtils::tick()
{
    int currentState = digitalRead(pin);

#ifndef DISABLE_PERFORM_DEBOUNCE_CORRECTION
    CheckState(currentState);
#else
    PerformEvents(currentState);
#endif
}   

void ButtonUtils::PerformEvents(int state)
{
    if (state == LOW)
    {
        if (onClicked != nullptr)
            onClicked();

        if (lastState)
            return;

        lastState = true;
        if (onPositiveEdge != nullptr)
            onPositiveEdge();
    }
    else
    {
        if (!lastState)
            return;

        lastState = false;
        if(onNegativeEdge != nullptr)
            onNegativeEdge();
    }
}