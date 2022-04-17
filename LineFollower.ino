#include "MotorDriver.h"
#include "ButtonUtils.h"
#include "LineDetector2.h"
#include "PID.h"
#include "Smoothers.h"

#define debugBuild

#define opto A0
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

Mode programMode = Mode::FollowingLine;

void onButtonPositiveEdge();

ButtonUtils modeButton(button, *onButtonPositiveEdge, nullptr, nullptr,200);

MotorDriver leftMotor(fwdL, bwdL, pwmL, 255);
MotorDriver rightMotor(fwdP, bwdP, pwmP, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new VelocitySmoother(10.0f);
SmootherBase *rightMotorSmoother = new VelocitySmoother(10.0f);

LineDetector2 linedetector(opto);
float kp = 5;
float ki = 0;
float kd = 0;
PID pid(kp, ki, kd, 255);
int lastDir = 0;

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

void updateTicks()
{
  modeButton.tick();
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
  print("  LeftM Val: " + String(leftMotorVal));
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
    SetMotors(dir);


    ApplyMotorValues();
    #ifdef debugBuild
    Serial.print("Dir:");
    Serial.print(lastDir);
    Serial.print("  Motor:");
    Serial.println(rightMotor.getValue());
#endif
  }
  else if (programMode == Mode::Calibrate)
  {
    leftMotor.setValueDirectly(0);
    rightMotor.setValueDirectly(0);
    linedetector.calibrate();
    print("Calibration");
  }
  delay(50);
  // Serial.println("");
}
