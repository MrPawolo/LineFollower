#include <Arduino.h>
#line 1 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
#include "defines.h"
#include "MotorDriver.h"
#include "ButtonUtils.h"
#include "LineDetector2.h"
#include "PID.h"
#include "Smoothers.h"

//---------------------------------------
//--------------Mode---------------------
//---------------------------------------
#pragma region Mode

enum Mode
{
  Calibrate,
  FollowingLine
};
Mode programMode = Mode::FollowingLine;

#pragma endregion


//---------------------------------------
//---------------Button------------------
//---------------------------------------
#pragma region Button

void onButtonPositiveEdge();

ButtonUtils modeButton(button, *onButtonPositiveEdge, nullptr, nullptr, 200);



#pragma endregion

//---------------------------------------
//--------------Motor--------------------
//---------------------------------------
#pragma region  Motor

MotorDriver leftMotor(fwdL, bwdL, pwmL, 255);
MotorDriver rightMotor(fwdP, bwdP, pwmP, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new VelocitySmoother(10.0f);
SmootherBase *rightMotorSmoother = new VelocitySmoother(10.0f);

#pragma endregion

//---------------------------------------
//--------------Line Detector------------
//---------------------------------------
LineDetector2 linedetector(opto);

//---------------------------------------
//--------------PID----------------------
//---------------------------------------
#pragma region  PID

float kp = 5;
float ki = 0;
float kd = 0;
PID pid(kp, ki, kd, 255);
int lastDir = 0;

#pragma endregion

#pragma region Misc

#line 88 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void print(String msg);
#line 95 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void enableSignal();
#line 113 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void setup();
#line 131 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
int GetVal(int prevDir);
#line 138 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void SetMotors(int dir);
#line 166 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void ApplyMotorValues();
#line 177 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void updateTicks();
#line 182 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void followingLine();
#line 195 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void callibrating();
#line 205 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void loop();
#line 70 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void onButtonPositiveEdge()
{
  if (programMode == Mode::Calibrate)
  {
    linedetector.acceptCalibrateValues();
    programMode = Mode::FollowingLine;
  }
  else if (programMode == Mode::FollowingLine)
  {
    programMode = Mode::Calibrate;
    linedetector.resetCalibration();
  }
#ifdef debugBuild
  Serial.print("Mode:");
  Serial.print(programMode);
#endif
}

void print(String msg)
{
#ifdef debugBuild
  Serial.println(msg);
#endif
}

void enableSignal()
{
  for (int i = 0; i < 3; i++)
  {
    rightMotor.setValueDirectly(50);
    leftMotor.setValueDirectly(50);
    digitalWrite(led, 1);
    delay(100);
    rightMotor.setValueDirectly(0);
    leftMotor.setValueDirectly(0);
    digitalWrite(led, 0);
    delay(100);
  }
}

#pragma endregion


void setup()
{
  Serial.begin(19200);
  print("BeginSetup");

  modeButton.setup();
  leftMotor.setup();
  rightMotor.setup();
  pinMode(led, OUTPUT);
  pid.setValue(500);


  enableSignal();
  print("EndSetup");
}



int GetVal(int prevDir)
{
  int val = linedetector.getValue();
  print("Value " + String(val));
  return val;
}

void SetMotors(int dir) // value between -180 and 180 where 0 is forward
{
  dir = clamp(dir, -180, 180);

  if (dir == 0)
  {
    rightMotorVal = 255;
    leftMotorVal = 255;
    return;
  }

  if (dir < 0)
  {
    rightMotorVal = 255;
    int val = (0.01111f * dir + 1) * 255;
    leftMotorVal = val;
    return;
  }

  if (dir > 0)
  {
    leftMotorVal = 255;
    int val = (-0.01111f * dir + 1) * 255;
    rightMotorVal = val;
    return;
  }
}

void ApplyMotorValues()
{
  float leftVal = leftMotorSmoother->tick(leftMotorVal);
  leftMotor.setValueDirectly(leftVal);
  print(" LeftM Val: " + String(leftMotorVal));
  rightMotor.setValueDirectly(rightMotorSmoother->tick(rightMotorVal));
}


#pragma region LoopRelated

void updateTicks()
{
  modeButton.tick();
}

void followingLine()
{
  int value = GetVal(lastDir);
  int dir = pid.tick(value);
  lastDir = dir;
  SetMotors(dir);

  ApplyMotorValues();

  print(" Dir: " + String(value) + F(" PID Val: ") + String(dir) + F(" MotorR: ") 
   + String(rightMotor.getValue()) + F(" MotorL: ") + String(leftMotor.getValue()));
}

void callibrating()
{
  leftMotor.setValueDirectly(0);
  rightMotor.setValueDirectly(0);
  linedetector.calibrate();
  print(F("Calibration"));
}

#pragma endregion

void loop()
{
  updateTicks();
  if (programMode == Mode::FollowingLine)
  {
    followingLine();
  }
  else if (programMode == Mode::Calibrate)
  {
    callibrating();
  }
  delay(50);
}

