#line 1 "d:\\Projects\\Arduino\\LineFollower\\MyFuncs.h"
#ifndef MY_FUNCS
#define MY_FUNCS

#include "Arduino.h"

int clamp(int inVal, int min, int max);
#define sign(x) (x>0?1:-1)
//#define millisToSecs 1000;
//#define secsToMillis 0.001;

const float millisToSecs = 1000;
const float secsToMillis = 0.001;

int myAbs(int val);

#endif 