//Coded by Ahmed Alzubairi (AOE Fall 2018)
#include <Servo.h>
class Target {
private:
 bool isHit;
 int AP;
 int DP;
 int limit;
 Servo myServo;
 
public:
 Target(int a, int b);
 bool getIsHit();
 int getAnalogPin();
 int getDigitalPin();
 void setLimit(int x);
 int getLimit();
 void hide();
 void expose();
 void servoTime();
 void setHit(bool x);
 bool getHit();
};

Target::Target(int digital, int analog)
{
 this->DP = digital;
 this->AP = analog;
 this->isHit=false;
 this->limit=0;
 //this->myServo.attach(digital);
}

void Target::hide()
{
myServo.write(90);
}
void Target::expose()
{
myServo.write(240);
}
void Target::servoTime(){
 myServo.attach(this->DP);
}
void Target::setLimit(int x)
{
this->limit=x;
}
int Target::getLimit()
{
return this->limit;
}
void Target::setHit(bool x){
this->isHit=x;
}
bool Target::getHit()
{
 return this->isHit;
}

int Target::getAnalogPin()
{
  return this->AP;
}
int Target::getDigitalPin()
{
  return this->DP;
}
