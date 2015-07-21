#include "LineFollower.h"
#include <SoftwareSerial.h>


using namespace pdlf;

byte s[] = { 54, 55, 56, 57, 58, 59, 60, 61 };
byte lm[] = { 11, 10 };
byte rm[] = { 13, 12 };
//Put m for mega
// and u for uno
char mcu = 'm';

Robot robot(lm, rm, s, 8 , mcu);

void setup()
{
	Serial.begin(9600);
	robot.printAnalog();
	//robot.initializeComponents();
	robot.updateWeightedValue();
	robot.generateWeight(8);
	robot.printWeight();
	robot.printDigital();
	robot.printWeightedValue();

}

void loop()
{
	/*
	Serial.println("----------------------");
	robot.printWeightedValue();
	Serial.println("\n--------------------");
	Serial.println();
	Serial.println();
	Serial.println(robot.weightedValueSum());
	*/
	//robot.pidLineFollow();
	//analogWrite(lm[0], 150);
	//analogWrite(lm[1], 0);
	//analogWrite(rm[0], 150);
	//analogWrite(rm[1], 0);
/// Setting constants on the go
        if (robot.bluetooth->available()){
          float kp = robot.bluetooth->parseFloat();
          float kd = robot.bluetooth->parseFloat();
          
          robot.setKd(kd);
          robot.setKp(kp); 
        }

	robot.pdLineFollow();
	
}
