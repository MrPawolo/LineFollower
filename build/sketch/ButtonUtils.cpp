#line 1 "d:\\Projects\\Arduino\\LineFollower\\ButtonUtils.cpp"
#include "ButtonUtils.h"

ButtonUtils::ButtonUtils(int pin, void (*on_positive_edge)(), void (*on_negative_edge)(), void (*on_clicked)())
{
    onPositiveEdge = on_positive_edge;
    onClicked = on_clicked;
    onNegativeEdge = on_negative_edge;
    ButtonUtils::pin = pin;
}

void ButtonUtils::tick()
{
    if (digitalRead(pin) == LOW)
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