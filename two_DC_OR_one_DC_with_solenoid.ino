/* This code used for two purposes:
 *  first - two DC motors that located face to face acting as bidireciobal lead screw
 *  second - one DC motor with selonoid attached to it an work as one direction stopper
 *  all writen by Ran Cohen
 */

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *leftMotor  = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);
Adafruit_DCMotor *pullyMotor = AFMS.getMotor(1);
Adafruit_DCMotor *solenoid   = AFMS.getMotor(2);

String inputString = "";
bool stringComplete = false;

void setup()
{
  Serial.begin(115200);
  inputString.reserve(200);
  AFMS.begin(); 
  leftMotor->setSpeed(255);
  rightMotor->setSpeed(255);
  pullyMotor->setSpeed(255);
  solenoid->setSpeed(255);
}

void loop()
{
  if (stringComplete)
  {
    // if(inputString.equals("o\n") || inputString.equals("O\n"))
    if(inputString == "o" || inputString =="O")
    {
      Serial.println("open");
      leftMotor->run(BACKWARD);
      rightMotor->run(FORWARD);
    }
    else if(inputString == "c" || inputString == "C")
    {
      Serial.println("closed");
      leftMotor->run(FORWARD);
      rightMotor->run(BACKWARD);
    }
    else if(inputString == "d" || inputString == "D")
    {
      Serial.println("Going Down");
      pullyDown();
      Serial.println("Done");
    }
    else if(inputString == "s" || inputString == "S")
    {
      openSolenoid();
    }
    else
    {
      Serial.println("STOP");
      leftMotor->run(RELEASE);
      rightMotor->run(RELEASE);
      pullyMotor->run(RELEASE);
      closeSolenoid();
    }
  }
  inputString = "";
  stringComplete = false;
  delay(10);
}


void serialEvent()
{
  while(Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n')
    {
      inputString.trim();
      stringComplete = true;
    }
  }
}

void openSolenoid()
{
  solenoid->run(BACKWARD);
  Serial.println("The solenoid is open");
}

void closeSolenoid()
{
  solenoid->run(RELEASE);
  Serial.println("The solenoid is closed");
}

void pullyDown()
{
  pullyMotor->run(FORWARD);
  openSolenoid();
  delay(100);
  pullyMotor->run(RELEASE);
  delay(100);
  pullyMotor->run(BACKWARD);
}
