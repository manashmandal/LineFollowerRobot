#ifndef MOTOR_H_
#define MOTOR_H_
#include <Arduino.h>

#define FRONT_PIN 0
#define BACK_PIN 1


typedef uint8_t uint;


typedef struct motorPins {
	uint leftMotor[2];
	uint rightMotor[2];
} motorpins;

enum Direction { FORWARD, BACKWARD, LEFT, RIGHT, ANTICLOCKWISE, CLOCKWISE, NOWHERE };

class Motor {
public:
	Motor(motorPins &pins);
	Motor(uint *leftMotor, uint *rightMotor);
	~Motor();

	void go(int leftSpeed, int rightSpeed, Direction dir);
	void print(void);

private:
	uint *leftMotor;
	uint *rightMotor;

	uint lspeed;
	uint rspeed;

	void leftMotorGoForward(int speed);
	void leftMotorGoBackward(int speed);
	void rightMotorGoForward(int speed);
	void rightMotorGoBackward(int speed);
	void leftMotorStop(void);
	void rightMotorStop(void);

};

void Motor::go(int leftSpeed, int rightSpeed, Direction dir) {
	lspeed = leftSpeed;
	rspeed = rightSpeed;

	if (dir == Direction::FORWARD) {
		leftMotorGoForward(leftSpeed);
		rightMotorGoForward(rightSpeed);
	}
	else if (dir == Direction::BACKWARD) {
		leftMotorGoBackward(leftSpeed);
		rightMotorGoBackward(rightSpeed);
	}
	else if (dir == Direction::LEFT) {
		leftMotorStop();
		rightMotorGoForward(rightSpeed);
	}
	else if (dir == Direction::RIGHT) {
		rightMotorStop();
		leftMotorGoForward(leftSpeed);
	}
	else if (dir == Direction::CLOCKWISE) {
		leftMotorGoForward(leftSpeed);
		rightMotorGoBackward(rightSpeed);
	}
	else if (dir == Direction::ANTICLOCKWISE) {
		leftMotorGoBackward(leftSpeed);
		rightMotorGoForward(rightSpeed);
	}
	else {
		leftMotorStop();
		rightMotorStop();
	}
}

void Motor::leftMotorGoForward(int speed) {
	analogWrite(leftMotor[FRONT_PIN], speed);
	analogWrite(leftMotor[BACK_PIN], 0);
}

void Motor::leftMotorGoBackward(int speed) {
	analogWrite(leftMotor[BACK_PIN], speed);
	analogWrite(leftMotor[FRONT_PIN], 0);
}

void Motor::rightMotorGoForward(int speed) {
	analogWrite(rightMotor[FRONT_PIN], speed);
	analogWrite(rightMotor[BACK_PIN], 0);
}

void Motor::rightMotorGoBackward(int speed) {
	analogWrite(rightMotor[FRONT_PIN], 0);
	analogWrite(rightMotor[BACK_PIN], speed);
}

void Motor::leftMotorStop(void) {
	analogWrite(leftMotor[FRONT_PIN], 0);
	analogWrite(leftMotor[BACK_PIN], 0);
}

void Motor::rightMotorStop(void) {
	analogWrite(rightMotor[FRONT_PIN], 0);
	analogWrite(rightMotor[BACK_PIN], 0);
}

//Ctor
Motor::Motor(motorPins &pins) {

	leftMotor = new uint[2];
	rightMotor = new uint[2];

	for (uint i = 0; i < 2; i++) {
		leftMotor[i] = pins.leftMotor[i];
		rightMotor[i] = pins.rightMotor[i];
		pinMode(leftMotor[i], OUTPUT);
		pinMode(rightMotor[i], OUTPUT);
	}
}

Motor::Motor(uint *leftMotor, uint *rightMotor) {
	leftMotor = new uint[2];
	rightMotor = new uint[2];

	for (uint i = 0; i < 2; i++) {
		this->leftMotor[i] = leftMotor[i];
		this->rightMotor[i] = rightMotor[i];
		pinMode(leftMotor[i], OUTPUT);
		pinMode(rightMotor[i], OUTPUT);
	}
}


//Dtor
Motor::~Motor() {
	delete[] leftMotor;
	delete[] rightMotor;

	//Debugging
	Serial.println("Motor Object is destroyed!");
}

//Debugging
void Motor::print(void) {

	//Printing about LEFT Motor
	Serial.println("LEFT MOTOR:");
	Serial.println("Front & Back Pins: " + String(leftMotor[FRONT_PIN]) + " & " + String(leftMotor[BACK_PIN]));
	Serial.println("Speed: " + String(lspeed));

	//Printing about RIGHT Motor
	Serial.println("RIGHT MOTOR:");
	Serial.println("Front & Back Pins: " + String(rightMotor[FRONT_PIN]) + " & " + String(rightMotor[BACK_PIN]));
	Serial.println("Speed: " + String(rspeed));
}


#endif
