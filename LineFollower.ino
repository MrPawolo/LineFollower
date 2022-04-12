#include "MotorDriver.h"
#include "ButtonUtils.h"
#include "LineDetector2.h"
#include "PID.h"
#include "Smoothers.h"

#define debugBuild


#define opto A5
#define button 7
#define led 6

#define fwdP 8
#define bwdP 9
#define pwmP 10

#define fwdL 13
#define bwdL 12
#define pwmL 11

enum Mode
{
  Calibrate,
  FollowingLine
};

Mode programMode;


void onButtonPositiveEdge();


ButtonUtils modeButton(button, *onButtonPositiveEdge, nullptr, nullptr);

MotorDriver leftMotor(fwdL, bwdL, pwmL, 255);
MotorDriver rightMotor(fwdP, bwdP, pwmP, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new VelocitySmoother(10.0f);
SmootherBase *rightMotorSmoother = new VelocitySmoother(10.0f);

LineDetector2 linedetector(A0);
float kp = 1;
float ki = 0;
float kd = 0;
PID pid(kp, ki, kd,255);
int lastDir = 0;


void onButtonPositiveEdge()
{
  if(programMode = Mode::Calibrate)
  {
    linedetector.acceptCalibrateValues();
  programMode = Mode::FollowingLine;
  }
  else if(programMode = Mode::FollowingLine)
  {
    programMode = Mode::Calibrate;
    linedetector.resetCalibration();
  }
}

void setup()
{
  modeButton.setup();
  leftMotor.setup();
  rightMotor.setup();
#ifdef debugBuild
  Serial.begin(9600);
#endif
}

void updateTicks()
{
  modeButton.tick();
}

int GetVal(int prevDir)
{
  return linedetector.getValue();
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
  leftMotor.setValueDirectly(leftMotorSmoother->tick(leftMotorVal));
  rightMotor.setValueDirectly(rightMotorSmoother->tick(rightMotorVal));
}
void loop()
{
  updateTicks();
  if (programMode == Mode::FollowingLine)
  {
    int value = GetVal(lastDir);
    int dir = pid.tick(value);
    lastDir = dir;

    ApplyMotorValues();
  }else if(programMode == Mode::Calibrate)
  {
    linedetector.calibrate();
  }
}
