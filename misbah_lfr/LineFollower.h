#ifndef LINEFOLLOWER_H_
#define LINEFOLLOWER_H_

#include <Motor.h>

#define qtrThreshold 600
#define SETPOINT 3500

#define BAUD 9600

int motorSpeed = 150;

void setup(void);

//PID Components

//Kp kd and ki constans
float kp = 0.0;
float kd = 0.0;
float ki = 0.0;

float error = 0;
float previousError = 0;
float totalError = 0;
float power = 0;

int PWMLeft = 0;
int PWMRight = 0;

int lastRead = 0;


//Motor object
Motor *motor;

//number of sensor
int numberOfSensor = 6;

//First two ints for left motor, second two ints for right motor
motorPins pins = {2, 3, 4, 5};

//Ir sensor pins and reading, reading stands for digital reading and analogReading stands for analog Reading
int sensor[] = {14, 15 , 16, 17, 18, 19};
int reading[] = {0, 0, 0, 0, 0,  0};
int analogReading[] = {0, 0, 0, 0, 0, 0};

int totalActiveSensor = 0;


//Initialize sensors
void setup_ir(void){
	for (int i = 0; i < numberOfSensor; i++){
		pinMode(sensor[i], INPUT);
	}
}



//reads the sensor array and returns approximate position
int readArray(void){
	int read = 0;
	int active = 0;

	for (int i = 0; i < numberOfSensor; i++){
		if (analogRead(sensor[i]) > qtrThreshold){
			reading[i] = 1;
			read += (i + 1) * 1000;
			active++;
		} else reading[i] = 0;
	}

	totalActiveSensor = active;
	read = map(read/active, 0 , 6000, 0, 1023);

	if (!read) return lastRead;
	else {
		lastRead = read;
		return read;
	}
}


void PID(void){
	int avgSensor = readArray();
	previousError = error;
	error = avgSensor - map(SETPOINT, 0, 6000, 0, 1023);
	totalError += error;
	power = (kp * error) + (kd * (error - previousError)) + (ki * totalError);
	if (power >= motorSpeed) power = motorSpeed;
	if (power < motorSpeed) power = -1 * motorSpeed;

	if (power < 0){
		PWMRight = motorSpeed;
		PWMLeft = motorSpeed - abs(int(power));
	} else {
		PWMRight = motorSpeed - int(power);
		PWMLeft = motorSpeed;
	}

	motor->go(PWMLeft, PWMRight, FORWARD);
}

void debug_sensor(void){
	readArray();
	Serial.println("======== DEBUG =========");
	Serial.println("Analog Value of the ir sensors");
	delay(100);
	for (int i = 0; i < numberOfSensor; i++){
		Serial.println("IR [" + String(i) + "] = " + String(analogRead(sensor[i])));
		delay(250);
	}

	Serial.println("========== Analog Value Print Done ======");
	delay(1000);

	readArray();
	Serial.println("Digital Value of the ir sensors");
	delay(100);
	for (int i = 0; i < numberOfSensor; i++){
		Serial.print(reading[i]);
		Serial.print(" ");
	}
	Serial.println("========== Digital Value Print Done ======");
	delay(1000);
}



//void setup function
void setup(void){
	Serial.begin(BAUD);
	setup_ir();
	motor = new Motor(pins);
}



#endif 