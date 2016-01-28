#include "Motor.h"

using namespace Robot;

motorpins pins = { 1,2, 3, 4 };

Motor motor(pins);

void setup()
{

	Serial.begin(9600);
	motor.go(100, 200, FORWARD);
	motor.go(255, 255, BACKWARD);
	motor.print();
	delete &motor;
  /* add setup code here */

}

void loop()
{

  /* add main program code here */

}
