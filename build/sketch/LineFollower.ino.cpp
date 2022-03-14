#include <Arduino.h>
#line 1 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
#include "MotorDriver.h"
#define DISABLE_PERFORM_DEBOUNCE_CORRECTION
#include "ButtonUtils.h"
#include "string.h"

void onStartButtonPositiveEdge();
void onStopButtonPositiveEdge();

MotorDriver leftMotor(0, 1, 2, 200, 255);
MotorDriver rightMotor(4, 5, 6, 200, 255);


ButtonUtils startButton(2, *onStartButtonPositiveEdge, nullptr, nullptr);
ButtonUtils stopButton(2, *onStopButtonPositiveEdge, nullptr, nullptr);


#line 17 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void setup();
#line 23 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void updateTicks();
#line 31 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void loop();
#line 17 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void setup()
{
  leftMotor.setup();
  rightMotor.setup();
}

void updateTicks()
{
  leftMotor.tick();
  rightMotor.tick();
  startButton.tick();
  stopButton.tick();
}

void loop()
{
  updateTicks();
  
}

void onStartButtonPositiveEdge()
{
  int x = 1;
  x += 1;
}
void onStopButtonPositiveEdge()
{
  int x = 1;
  x += 1;
}

