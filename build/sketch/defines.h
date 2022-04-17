#line 1 "d:\\Projects\\Arduino\\LineFollower\\defines.h"
#ifndef DEFINES
#define DEFINES

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


//Line detector
#define minNoise 28
#define maxNoise 400


#define targetVal 300

//PID
//#define onlyP
#define clampI
//#define clampD

//Motor
#define maxVelChange 400

#endif