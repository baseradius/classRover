/*
 * School class autonomous line-following big weight hauling assistant
 * Designed By: Ahmad Abdullah
 * Programmed By: Base*Radius, Ajman - UAE 
 * 
 * Version 0.1 20-Jan-2016
*/

//Line sensor input pins
#define ls0 A0  //Line Sensor 0 (extreme left)
#define ls1 A1  //Line Sensor 1 
#define ls2 A2  //Line Sensor 2
#define ls3 A3  //Line Sensor 3
#define ls4 A4  //Line Sensor 4
#define ls5 A5  //Line Sensor 5 (extreme right)

//Motor Input Pins
#define lm1 2   //Left Motor input 1 (relay 1 or motor driver input 1)
#define lm2 3   //Left Motor input 2 (relay 2 or motor driver input 2)

#define rm1 4   //Right Motor input 1 (relay 3 or motor driver input 3)
#define rm2 5   //Right Motor input 2 (relay 4 or motor driver input 4)

#define bell 6 /*This is the pin that will activate the bell switch
                 that rings in the classroom. For now, we have only 1 switch for all ringers
               */
//#define production //Use this to mark code in production - all serial communication will be switched off.


//Motor controls
void goForward();
void goReverse();
void stopMotors();
void goLeft();
void goRight();
void go360(int del);
void leftMotor(int start, char direction='f');
void rightMotor(int start, char direction='r');

//Miscellenious functions
void ringBell(int times=1);
int whichClass(char msb); //Decodes class/room number from 5 line sensor's binary data

//Core functions
int line_follower();

//Test functions
void testBell();
void testMotors();
void testLeftMotor();
void testRightMotor();

void setup()
{
  pinMode(lm1,1);
  pinMode(lm2,1);
  pinMode(rm1,1);
  pinMode(rm2,1);
  pinMode(bell,1);    

  #ifndef production
    Serial.begin(115200);
  #endif
}

int state = 0;
int allOn = 0;

void loop()
{
  whichClass();  
}

int line_follower()
{
  int r = 0;
  
  if(digitalRead(ls2) == HIGH)
  {
   leftMotor(1,'f');
  }
  else
  {
    leftMotor(0);
  }
  if(digitalRead(ls3) == HIGH)
  {
    rightMotor(1,'r');
  }
  else
  {
    rightMotor(0);
  }

  if(digitalRead(ls1) == LOW && digitalRead(ls2) == LOW && digitalRead(ls3) == LOW && digitalRead(ls4) == LOW)
  {
    r = 1;
  }

  return r;    
}

/*
 *6 sensors. left most sensor is msb
 *000001 = class 1
 *000010 = class 2 etc..  
 *But, to save black marks... I will invert all sensor readings. So.. a sensor that has black will give 1.. 
 *This gives us 63 addressable locations.
*/
int whichClass()
{
  int cc = 0; //classCode

  cc = cc | !digitalRead(ls0);
  cc = cc << 1;
  cc = cc | !digitalRead(ls1);
  cc = cc << 1;
  cc = cc | !digitalRead(ls2);
  cc = cc << 1;
  cc = cc | !digitalRead(ls3);
  cc = cc << 1;
  cc = cc | !digitalRead(ls4);
  cc = cc << 1;
  cc = cc | !digitalRead(ls5);
  
  #ifndef production
    Serial.println(cc);
  #endif

  return cc; 
}

void goForward()
{
  leftMotor(1,'f');
  rightMotor(1,'f');
}

void goReverse()
{
  leftMotor(1,'r');
  rightMotor(1,'r');
}

void goLeft()
{
  leftMotor(1,'f');
  rightMotor(0);  
}

void goRight()
{
  leftMotor(0);
  rightMotor(1,'f');
}

void go360(int del)
{
  leftMotor(1,'f');
  rightMotor(1,'r');

  delay(del);
}

void ringBell(int times)
{
  for(int i=0;i<times;i++)
  {
    digitalWrite(bell,1);
    delay(250);
    digitalWrite(bell,0);
  }
}

void testBell()
{
  #ifndef production
    Serial.println("Testing bell 2 times");
  #endif
  
  ringBell(2);

  #ifndef production
    Serial.println("Finished ringing bell");  
  #endif
}

void testMotors()
{
  #ifndef production
    Serial.println("Testing both motors");
  #endif
  
  testLeftMotor();
  testRightMotor();

  #ifndef production
    Serial.println("Finished testing motors");
  #endif
}

void testLeftMotor()
{
  #ifndef production
    Serial.println("Testing Left Motor forward");
  #endif
  
  leftMotor(1,'f');
  delay(500);

  #ifndef production
    Serial.println("Testing left Motor reverse");
  #endif
  
  leftMotor(1,'r');
  delay(500);

  #ifndef production
    Serial.println("Stopping Left Motor");
  #endif
  
  leftMotor(0);
}

void testRightMotor()
{
  #ifndef production
    Serial.println("Testing Right Motor forward");
  #endif
  
  rightMotor(1,'f');
  delay(500);

  #ifndef production
    Serial.println("Testing Right Motor reverse");
  #endif
  
  rightMotor(1,'r');
  delay(500);

  #ifndef production
    Serial.println("Stopping Right Motor");
  #endif
  
  rightMotor(0);
}

/*
 * Left Motor controller
 * pins lm1 and lm2
 * 
 * start = 1 to start motor. start = 0 to stop motor
 * direction = 'f' for forward, direction = 'r' for reverse .. defaults to forward
*/
void leftMotor(int start, char direction)
{
  if(start == 1)
  {
    if(direction == 'f')
    {
      digitalWrite(lm1,1);
      digitalWrite(lm2,0);
    }
    else if(direction == 'r')
    {
      digitalWrite(lm1,0);
      digitalWrite(lm2,1);
    }
    else
    {
      digitalWrite(lm1,1);
      digitalWrite(lm2,0);
    }
  }
  else if(start == 0)
  {
    digitalWrite(lm1,0);
    digitalWrite(lm2,0);
  }  
  else
  {
    digitalWrite(lm1,0);
    digitalWrite(lm2,0);
  }
}

/*
 * Right Motor controller
 * pins lm1 and lm2
 * 
 * start = 1 to start motor. start = 0 to stop motor
 * direction = 'f' for forward, direction = 'r' for reverse .. defaults to forward
*/
void rightMotor(int start, char direction)
{
  if(start == 1)
  {
    if(direction == 'f')
    {
      digitalWrite(rm1,1);
      digitalWrite(rm2,0);
    }
    else if(direction == 'r')
    {
      digitalWrite(rm1,0);
      digitalWrite(rm2,1);
    }
    else
    {
      digitalWrite(rm1,1);
      digitalWrite(rm2,0);
    }
  }
  else if(start == 0)
  {
    digitalWrite(rm1,0);
    digitalWrite(rm2,0);
  }  
  else
  {
    digitalWrite(rm1,0);
    digitalWrite(rm2,0);
  }
}

void stopMotors()
{
  leftMotor(0);
  rightMotor(0);  
}

