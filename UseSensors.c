#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S3,     gyro,                sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     sonar,               sensorSONAR)
#pragma config(Motor,  motorB,          rightMotor,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorC,          leftMotor,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     motorH,        tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorNone, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//other files in use
#include "MotorControl.c"
#include "ServoControl.c"

//default speed for motors connected to drive wheels
#define DEFAULT_SPEED 75

/*
/*
*servo1 = left bowling ball holder
*servo2 = right bowling ball holder
*servo3 = ball collection servo
*motorD = left drive
*motorE = right drive
*motorH = left crate lift  (needs reversed in pragma configs)
*motorI = right crate lift
*motor? = ball collector
*/

//use the ir sensor
/*void useIR() {
  int irVal = SensorRaw[ir];
  if (irVal < 5) {
      drive(0, -DEFAULT_SPEED);
  } else if (irVal > 5) {
    drive(DEFAULT_SPEED, 0);
  } else {
    drive(DEFAULT_SPEED,-DEFAULT_SPEED);
  }
}*/

float degTurned = 0;

float lastTime = 0;

int degPerS = 0;

/*void useGyro(float degrees) {
  SensorType[S3] = sensorNone;
  wait1Msec(1000);
  SensorType[S3] = gyro;
  if (degrees < 0.0) {
    drive(DEFAULT_SPEED/2, DEFAULT_SPEED/2);
  } else {
    drive(-DEFAULT_SPEED/2, -DEFAULT_SPEED/2);
  }
  while (true) {
    if (lastTime != 0) {
      nxtDisplayTextLine(5, "%f", degTurned);
      float deltaTime = ((float) (time1[T1] - lastTime)) / 1000;
      degTurned += (float) degPerS * deltaTime;
      if (abs(degTurned) >= abs(degrees)) {
        break;
      }
    }
    lastTime = time1[T1];
    degPerS = SensorRaw[gyro] - 600;
    wait1Msec(20);
  }
}*/

//rotate 'degrees' degrees using the gyro
void useGyro(float degrees) {
  if (degrees > 0) {
    drive(-DEFAULT_SPEED,-DEFAULT_SPEED);
  } else {
    drive(DEFAULT_SPEED, DEFAULT_SPEED);
  }
  while (true) {
    if (lastTime != 0) {
      float deltaTime = ((float) (time1[T1] - lastTime)) / 1000;
      degTurned += (float) degPerS * deltaTime;
      if (abs(degTurned) >= abs(degrees)) {
        nxtDisplayTextLine(5, "%f", degTurned);
        drive(0,0);
        lastTime = 0;
        degTurned = 0;
        break;
        /*wait1Msec(5000);
        drive(DEFAULT_SPEED,-DEFAULT_SPEED);
        wait1Msec(1000);
        drive(0,0);
        lastTime = 0;
        degTurned = 0;*/
      }
    }
    lastTime = time1[T1];
    degPerS = SensorRaw[gyro] - 600;
    wait1Msec(20);
  }
}

//maximum distance from sonar sensor before gates close
const int DIST = 15;

//check for the bowling ball
task checkSonarSensor() {
  //setServoPosition(servo3,150);
  setServoPosition(servo1,140);
  setServoPosition(servo2,0);
  while (true) {
    int sVal = SensorValue[sonar];
    nxtDisplayTextLine(1,"Sonar: %d", sVal);
    if (SensorValue[sonar] <= DIST) {
      //setServoPosition(servo3, 15);
      setServoPosition(servo1, 0);
      setServoPosition(servo2,140);
    }
  }
}

/*
*drive down ramp
*turn -90 deg if red team. turn 90 deg in blue team
*drive straight for x amount of time
*turn -45 if red. turn 45 if blue team
*drive forward for x amount of seconds
*if touch sensor activated turn x degrees then follow ir turn 180 degrees, follow ir. if not, turn -x degrees, then follow ir (x varies based on team)
*/

//degrees to turn 90 degrees
const float NINETY_DEG_RIGHT = 79.5;

//degrees to turn 45 degrees
const float FOURTYFIVE_DEG_RIGHT = NINETY_DEG_RIGHT / 2.3;

//degrees to turn 90 degrees
const float NINETY_DEG_LEFT = -67;

//degrees to turn 45 degrees
const float FOURTYFIVE_DEG_LEFT = NINETY_DEG_LEFT / 2.5;

//start from red side, position a
void posRedA() {
  PlaySound(soundBeepBeep);
  synchMotors(motorD, motorE);
  drive(-DEFAULT_SPEED,DEFAULT_SPEED);
  wait1Msec(1075);
  drive(DEFAULT_SPEED,-DEFAULT_SPEED);
  wait1Msec(100);
  drive(0,0);
  wait1Msec(500);
  useGyro(NINETY_DEG_LEFT);
  SensorType[S3] = sensorNone;
  wait1Msec(100);
  setServoPosition(servo1,140);
  setServoPosition(servo2,0);
  StartTask(checkSonarSensor);
  drive(-DEFAULT_SPEED, DEFAULT_SPEED);
  wait1Msec(875);
  drive(0,0);
  wait1Msec(200);
  useGyro(FOURTYFIVE_DEG_LEFT/1.9);
  wait1Msec(800);
  drive(-DEFAULT_SPEED,DEFAULT_SPEED);
  wait1Msec(2000);
}

//start from red side, position b
void posRedB() {
  PlaySound(soundBeepBeep);
  synchMotors(motorD, motorE);
  drive(-DEFAULT_SPEED,DEFAULT_SPEED);
  wait1Msec(975);
  drive(DEFAULT_SPEED,-DEFAULT_SPEED);
  wait1Msec(100);
  drive(0,0);
  wait1Msec(500);
  useGyro(NINETY_DEG_LEFT);
  SensorType[S3] = sensorNone;
  wait1Msec(100);
  setServoPosition(servo1,140);
  setServoPosition(servo2,0);
  StartTask(checkSonarSensor);
  drive(-DEFAULT_SPEED, DEFAULT_SPEED);
  wait1Msec(400);
  drive(0,0);
  wait1Msec(200);
  useGyro(FOURTYFIVE_DEG_LEFT/1.3);
  wait1Msec(500);
  drive(-DEFAULT_SPEED,DEFAULT_SPEED);
  wait1Msec(2000);
}

//start from blue side, position a
void posBlueA() {
  PlaySound(soundBeepBeep);
  synchMotors(motorD, motorE);
  drive(-DEFAULT_SPEED,DEFAULT_SPEED);
  wait1Msec(1075);
  drive(DEFAULT_SPEED,-DEFAULT_SPEED);
  wait1Msec(100);
  drive(0,0);
  wait1Msec(500);
  useGyro(NINETY_DEG_RIGHT);
  SensorType[S3] = sensorNone;
  wait1Msec(100);
  setServoPosition(servo1,140);
  setServoPosition(servo2,0);
  StartTask(checkSonarSensor);
  drive(-DEFAULT_SPEED, DEFAULT_SPEED);
  wait1Msec(875);
  drive(0,0);
  wait1Msec(200);
  useGyro(FOURTYFIVE_DEG_RIGHT);
  wait1Msec(800);
  drive(-DEFAULT_SPEED,DEFAULT_SPEED);
  wait1Msec(2500);
}

//start from blue side, position b
void posBlueB() {
  PlaySound(soundBeepBeep);
  synchMotors(motorD, motorE);
  drive(-DEFAULT_SPEED,DEFAULT_SPEED);
  wait1Msec(975);
  drive(DEFAULT_SPEED,-DEFAULT_SPEED);
  wait1Msec(100);
  drive(0,0);
  wait1Msec(500);
  useGyro(NINETY_DEG_RIGHT);
  SensorType[S3] = sensorNone;
  wait1Msec(100);
  setServoPosition(servo1,140);
  setServoPosition(servo2,0);
  StartTask(checkSonarSensor);
  drive(-DEFAULT_SPEED, DEFAULT_SPEED);
  wait1Msec(400);
  drive(0,0);
  wait1Msec(200);
  useGyro(FOURTYFIVE_DEG_RIGHT/1.3);
  wait1Msec(500);
  drive(-DEFAULT_SPEED,DEFAULT_SPEED);
  wait1Msec(2500);
}
