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
# 21 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
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

MotorDriver leftMotor(13, 12, 11, 110, 255);
MotorDriver rightMotor(8, 9, 10, 110, 255);
int leftMotorVal = 0;
int rightMotorVal = 0;
SmootherBase *leftMotorSmoother = new ProportionalSmoother(0.01f);
SmootherBase *rightMotorSmoother = new ProportionalSmoother(0.01f);

#pragma endregion

//---------------------------------------
//--------------Line Detector------------
//---------------------------------------
LineDetector2 linedetector(A0);
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




}

void print(String msg)
{



}

void signal(int count, int lowTime, int highTime)
{
  for (int i = 0; i < count; i++)
  {
    rightMotor.noise();
    leftMotor.noise();
    digitalWrite(6, 1);
    delay(highTime);
    rightMotor.stopNoise();
    leftMotor.stopNoise();
    digitalWrite(6, 0);
    delay(lowTime);
  }
}

#pragma endregion

void setup()
{



  ;;

  modeButton.setup();
  leftMotor.setup();
  rightMotor.setup();
  pinMode(6, 0x1);
  pid.setValue(500);
  linedetector.setMinMax(28, 400);

  signal(3, 100, 100);
  ;;
}

float GetVal(int prevDir)
{
  int val = linedetector.getValue();
  ;;

  //val = signalSmoother->tick(val);
  int out = val;

  // if(val >= lastValue +1)
  // {
  //   lowering = false;
  // }

  // if(val < lastValue  && lastDir < 0)
  // {
  //   lowering = true;
  // }


  // if(lowering == true)
  // {
  //   out = 1023;
  // }

  // // if(val >= lastValue && lastDir > 0)
  // // {
  // //   out = 0;
  // // }

  // lastValue = val;
  return out;
}

void SetMotors(int dir) // value between -180 and 180 where 0 is forward
{
  dir = clamp(dir, -140, 140);

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

  //leftMotor.setValueDirectly(leftMotorVal);
  //rightMotor.setValueDirectly(rightMotorVal);
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

  ;;
}

void wait()
{
  leftMotor.setValueDirectly(0);
  rightMotor.setValueDirectly(0);
  analogWrite(6, map(linedetector.getValue(), 0, 1023, 0, 255));
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
  ;;
}
