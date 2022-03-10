# 1 "d:\\Projects\\Arduino\\LineFollower\\LineFollower.ino"
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
