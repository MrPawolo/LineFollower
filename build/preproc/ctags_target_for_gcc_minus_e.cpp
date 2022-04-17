# 1 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
# 2 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 3 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 4 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 5 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 6 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 7 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2

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

ButtonUtils modeButton(7, *onButtonPositiveEdge, nullptr, nullptr, 200);



#pragma endregion

//---------------------------------------
//--------------Motor--------------------
//---------------------------------------
#pragma region Motor

MotorDriver leftMotor(13, 12, 11, 255);
MotorDriver rightMotor(8, 9, 10, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new VelocitySmoother(10.0f);
SmootherBase *rightMotorSmoother = new VelocitySmoother(10.0f);

#pragma endregion

//---------------------------------------
//--------------Line Detector------------
//---------------------------------------
LineDetector2 linedetector(A0);

//---------------------------------------
//--------------PID----------------------
//---------------------------------------
#pragma region PID

float kp = 5;
float ki = 0;
float kd = 0;
PID pid(kp, ki, kd, 255);
int lastDir = 0;

#pragma endregion

#pragma region Misc

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

#pragma endregion


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

  print(" Dir: " + String(value) + (reinterpret_cast<const __FlashStringHelper *>(
# 191 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                  (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 191 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                  " PID Val: "
# 191 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                  ); &__c[0];}))
# 191 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                  )) + String(dir) + (reinterpret_cast<const __FlashStringHelper *>(
# 191 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                                                  (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 191 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                                                  " MotorR: "
# 191 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                                                  ); &__c[0];}))
# 191 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                                                  ))
   + String(rightMotor.getValue()) + (reinterpret_cast<const __FlashStringHelper *>(
# 192 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 192 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                    " MotorL: "
# 192 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                    ); &__c[0];}))
# 192 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                    )) + String(leftMotor.getValue()));
}

void callibrating()
{
  leftMotor.setValueDirectly(0);
  rightMotor.setValueDirectly(0);
  linedetector.calibrate();
  print((reinterpret_cast<const __FlashStringHelper *>(
# 200 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 200 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
       "Calibration"
# 200 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
       ); &__c[0];}))
# 200 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
       )));
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
