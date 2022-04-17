# 1 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
# 2 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 3 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 4 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 5 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 6 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 21 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
enum Mode
{
  Calibrate,
  FollowingLine
};

Mode programMode = Mode::FollowingLine;

void onButtonPositiveEdge();

ButtonUtils modeButton(7, *onButtonPositiveEdge, nullptr, nullptr,200);

MotorDriver leftMotor(13, 12, 11, 255);
MotorDriver rightMotor(8, 9, 10, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new VelocitySmoother(10.0f);
SmootherBase *rightMotorSmoother = new VelocitySmoother(10.0f);

LineDetector2 linedetector(A0);
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

  Serial.print("Mode:");
  Serial.print(programMode);

}

void print(String msg)
{

  Serial.println(msg);

}

void enableSignal()
{
  for (int i = 0; i < 3; i++)
  {
    rightMotor.setValueDirectly(50);
    leftMotor.setValueDirectly(50);
    digitalWrite(6, 1);
    delay(100);
    rightMotor.setValueDirectly(0);
    leftMotor.setValueDirectly(0);
    digitalWrite(6, 0);
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
  pinMode(6, 0x1);
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

    Serial.print("Dir:");
    Serial.print(lastDir);
    Serial.print("  Motor:");
    Serial.println(rightMotor.getValue());

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
