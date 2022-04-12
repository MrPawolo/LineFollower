#line 1 "d:\\Projects\\Arduino\\LineFollower\\ButtonUtils.cpp"
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
    onPositiveEdge = on_positive_edge;
    onClicked = on_clicked;
    onNegativeEdge = on_negative_edge;
    ButtonUtils::pin = pin;
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
    actState = state;
    //PerformEvents(state);
}
#endif

void ButtonUtils::tick()
{
    int currentState = digitalRead(pin);

#ifndef DISABLE_PERFORM_DEBOUNCE_CORRECTION
    CheckState(currentState);
    PerformEvents(actState);
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