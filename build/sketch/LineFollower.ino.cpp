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
  Wait,
  FollowingLine
};
Mode programMode = Mode::Wait;

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

MotorDriver leftMotor(fwdL, bwdL, pwmL, 255 );
MotorDriver rightMotor(fwdP, bwdP, pwmP, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new VelocitySmoother(maxVelChange);
SmootherBase *rightMotorSmoother = new VelocitySmoother(maxVelChange);

#pragma endregion

//---------------------------------------
//--------------Line Detector------------
//---------------------------------------
LineDetector2 linedetector(opto);
SmootherBase *signalSmoother = new ProportionalSmoother(1.0f);

//---------------------------------------
//--------------PID----------------------
//---------------------------------------
#pragma region  PID

const float kp = 0.24f;
const float ki = 0;//0.6f;
const float kd = 0;//0.5f;
const float maxOutput = 255;
const float maxI = 150;
const float maxD = 100;
PID pid(kp, ki, kd, maxOutput, maxI, maxD);
int lastDir = 0;

#pragma endregion

#pragma region Misc

#line 92 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void print(String msg);
#line 99 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void signal(int count, int lowTime, int highTime);
#line 117 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void setup();
#line 136 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
float GetVal(int prevDir);
#line 143 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void SetMotors(int dir);
#line 171 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void ApplyMotorValues();
#line 183 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void updateTicks();
#line 188 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void followingLine();
#line 201 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void wait();
#line 210 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void loop();
#line 74 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void onButtonPositiveEdge()
{
  if (programMode == Mode::Wait)
  {
    programMode = Mode::FollowingLine;
    signal(5,200,100);
  }
  else if (programMode == Mode::FollowingLine)
  {
    programMode = Mode::Wait;
    signal(2,100,100);
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

void signal(int count, int lowTime, int highTime)
{
  for (int i = 0; i < count; i++)
  {
    rightMotor.setValueDirectly(50);
    leftMotor.setValueDirectly(50);
    digitalWrite(led, 1);
    delay(highTime);
    rightMotor.setValueDirectly(0);
    leftMotor.setValueDirectly(0);
    digitalWrite(led, 0);
    delay(lowTime);
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
  pid.setValue(targetVal);
  linedetector.setMinMax(minNoise, maxNoise);


  signal(3,100,100);
  print("EndSetup");
}



float GetVal(int prevDir)
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
  // leftMotor.setValueDirectly(leftMotorSmoother->tick(leftMotorVal));
  // rightMotor.setValueDirectly(rightMotorSmoother->tick(rightMotorVal));

  leftMotor.setValueDirectly(leftMotorVal);
  rightMotor.setValueDirectly(rightMotorVal);
}


#pragma region LoopRelated

void updateTicks()
{
  modeButton.tick();
}

void followingLine()
{
  float value = GetVal(lastDir);
  //value = signalSmoother->tick(value);
  float dir = pid.tick(value);
  lastDir = dir;
  SetMotors(dir);

  ApplyMotorValues();

  print("Val: " + String(value) + " PID Val: " + String(dir) + F(" MotorR: ") + String(rightMotorVal) + F(" MotorL: ") + String(leftMotorVal));
}

void wait()
{
  leftMotor.setValueDirectly(0);
  rightMotor.setValueDirectly(0);
  analogWrite(led, map(linedetector.getValue(),0,1023,0,255));
}

#pragma endregion

void loop()
{
  updateTicks();
  if (programMode == Mode::FollowingLine)
  {
    followingLine();
  }
  else
  {
    wait();
  }
  delay(10);
  print("--------------:" + String(programMode));
}

