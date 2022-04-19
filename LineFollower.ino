#include "defines.h"
#include "MotorDriver.h"
#include "ButtonUtils.h"
#include "LineDetector.h"
#include "PID.h"
#include "Smoothers.h"

//---------------------------------------
//--------------Mode---------------------
//---------------------------------------
#pragma region Mode

#ifdef debugBuild
#define P(a) (a)
#else
#define P(a) ;
#endif

#define LINEAR

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
#pragma region Motor

MotorDriver leftMotor(fwdL, bwdL, pwmL, minPWM, 255);
MotorDriver rightMotor(fwdP, bwdP, pwmP, minPWM, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new ProportionalSmoother(propChange);
SmootherBase *rightMotorSmoother = new ProportionalSmoother(propChange);

#pragma endregion

//---------------------------------------
//--------------Line Detector------------
//---------------------------------------
LineDetector linedetector(opto);
SmootherBase *signalSmoother = new ProportionalSmoother(0.03f);

//---------------------------------------
//--------------PID----------------------
//---------------------------------------
#pragma region PID

const float kp = 0.4f;
const float ki = 0.3f; // 0.6f;
const float kd = 0.5f; // 0.5f;
const float maxOutput = 255;
const float maxI = 10;
const float maxD = 255;
PID pid(kp, ki, kd, maxOutput, maxI, maxD);
int signalCompensation = +70;

int lastDir = 0;
int lastValue = 0;
bool lowering = false; 

#pragma endregion

#pragma region Misc

void onButtonPositiveEdge()
{
  if (programMode == Mode::Wait)
  {
    programMode = Mode::FollowingLine;
    signal(5, 200, 100);
  }
  else if (programMode == Mode::FollowingLine)
  {
    programMode = Mode::Wait;
    signal(2, 100, 100);
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
    rightMotor.noise();
    leftMotor.noise();
    digitalWrite(led, 1);
    delay(highTime);
    rightMotor.stopNoise();
    leftMotor.stopNoise();
    digitalWrite(led, 0);
    delay(lowTime);
  }
}

#pragma endregion

void setup()
{
  #ifdef debugBuild
  Serial.begin(19200);
  #endif
  P(print("BeginSetup"));

  modeButton.setup();
  leftMotor.setup();
  rightMotor.setup();
  pinMode(led, OUTPUT);
  pid.setValue(targetVal);
  linedetector.setMinMax(minNoise, maxNoise);

  signal(3, 100, 100);
  P(print("EndSetup"));
}

float GetVal(int prevDir)
{
  int val = linedetector.getValue();
  P(print("Value " + String(val)));
  int out = val;
  return out;
}

void SetMotors(int dir) // value between -180 and 180 where 0 is forward
{
  dir = clamp(dir, -limitRot, limitRot);

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
   leftMotor.setValueDirectly(leftMotorSmoother->tick(leftMotorVal));
   rightMotor.setValueDirectly(rightMotorSmoother->tick(rightMotorVal));
}

#pragma region LoopRelated

void updateTicks()
{
  modeButton.tick();
}

void followingLine()
{
  float value = GetVal(lastDir) - signalCompensation;
  float dir = pid.tick(value);
  lastDir = dir;
  SetMotors(dir);

  ApplyMotorValues();

  P(print("Val: " + String(value) + " PID Val: " + String(dir) + F(" MotorR: ") + String(rightMotorVal) + F(" MotorL: ") + String(leftMotorVal)));
}

void wait()
{
  leftMotor.setValueDirectly(0);
  rightMotor.setValueDirectly(0);
  analogWrite(led, map(linedetector.getValue(), 0, 1023, 0, 255));
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
  P(print("--------------:" + String(programMode)));
}
