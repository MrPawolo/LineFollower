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

ButtonUtils modeButton(7, *onButtonPositiveEdge, nullptr, nullptr, 200);



#pragma endregion

//---------------------------------------
//--------------Motor--------------------
//---------------------------------------
#pragma region Motor

MotorDriver leftMotor(13, 12, 11, 255 );
MotorDriver rightMotor(8, 9, 10, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new VelocitySmoother(400);
SmootherBase *rightMotorSmoother = new VelocitySmoother(400);

#pragma endregion

//---------------------------------------
//--------------Line Detector------------
//---------------------------------------
LineDetector2 linedetector(A0);
SmootherBase *signalSmoother = new ProportionalSmoother(1.0f);

//---------------------------------------
//--------------PID----------------------
//---------------------------------------
#pragma region PID

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

  Serial.print("Mode:");
  Serial.print(programMode);

}

void print(String msg)
{

  Serial.println(msg);

}

void signal(int count, int lowTime, int highTime)
{
  for (int i = 0; i < count; i++)
  {
    rightMotor.setValueDirectly(50);
    leftMotor.setValueDirectly(50);
    digitalWrite(6, 1);
    delay(highTime);
    rightMotor.setValueDirectly(0);
    leftMotor.setValueDirectly(0);
    digitalWrite(6, 0);
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
  pinMode(6, 0x1);
  pid.setValue(300);
  linedetector.setMinMax(28, 400);


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

  print("Val: " + String(value) + " PID Val: " + String(dir) + (reinterpret_cast<const __FlashStringHelper *>(
# 198 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 198 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                                              " MotorR: "
# 198 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                                              ); &__c[0];}))
# 198 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                                              )) + String(rightMotorVal) + (reinterpret_cast<const __FlashStringHelper *>(
# 198 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                                                                                       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 198 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                                                                                       " MotorL: "
# 198 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 3
                                                                                                       ); &__c[0];}))
# 198 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
                                                                                                       )) + String(leftMotorVal));
}

void wait()
{
  leftMotor.setValueDirectly(0);
  rightMotor.setValueDirectly(0);
  analogWrite(6, map(linedetector.getValue(),0,1023,0,255));
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
