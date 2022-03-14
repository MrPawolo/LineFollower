# 1 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
# 2 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2

# 4 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2
# 5 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino" 2


# 6 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void onStartButtonPositiveEdge();
void onStopButtonPositiveEdge();

MotorDriver leftMotor(0, 1, 2, 200, 255);
MotorDriver rightMotor(4, 5, 6, 200, 255);


ButtonUtils startButton(2, *onStartButtonPositiveEdge, nullptr, nullptr);
ButtonUtils stopButton(2, *onStopButtonPositiveEdge, nullptr, nullptr);


void setup()
{
  leftMotor.setup();
  rightMotor.setup();
}

void updateTicks()
{
  leftMotor.tick();
  rightMotor.tick();
  startButton.tick();
  stopButton.tick();
}

void loop()
{
  updateTicks();

}

void onStartButtonPositiveEdge()
{
  int x = 1;
  x += 1;
}
void onStopButtonPositiveEdge()
{
  int x = 1;
  x += 1;
}
