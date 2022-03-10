#include <Arduino.h>
#line 1 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
class LOL
{
    int xd;
};

struct LOLz
{
    int lol;
};

LOL * lol = new LOL();
LOLz * lolz = new LOLz();

#line 14 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void setup();
#line 22 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void loop();
#line 14 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
void setup()
{
	Serial.begin(9600);
    int i = 0;
    Serial.println((long)sizeof(*lol));
    Serial.println((long)sizeof(*lolz));
}

void loop()
{
}

