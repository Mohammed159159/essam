#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "pins.h"
#include <Bluepad32.h>

#define FORWARD true
#define BACKWARD false

extern int motorSpeed;
extern int diff;
extern bool isFwd;
extern int correction;
extern int leftSpeed;
extern int rightSpeed;

const int minSpeed = 70;
const int minDiff = 20;

void setRightMotor(unsigned int speed, bool isFwd);

void setLeftMotor(unsigned int speed, bool isFwd);

void move(int speed, int diff, bool isFwd);

void forward(int speed);
void right(int speed);
void left(int speed);
void stop();

#endif