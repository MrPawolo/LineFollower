# 1 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
# 2 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 3 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2

void LEL();
void LEL1();
void LEL2();

MotorDriver leftMotor(0, 1, 2, 200, 255);
MotorDriver rightMotor(4,5,6,200,255);

ButtonUtils startButton(2,*LEL,nullptr,nullptr);
ButtonUtils stopButton(2,*LEL1,nullptr,nullptr);


void setup()
{
  leftMotor.setup();
  rightMotor.setup();
}

void loop()
{
  leftMotor.tick();
  rightMotor.tick();
  startButton.tick();
  stopButton.tick();
}

void LEL()
{
  int x = 1;
  x+=1;
}
void LEL1()
{
  int x = 1;
  x+=1;
}
void LEL2()
{
  int x = 1;
  x+=1;
}
