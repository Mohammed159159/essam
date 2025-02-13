#include "motors.h"

int motorSpeed = 0;
int diff = 0;
bool isFwd = true;
int correction = 1;
int leftSpeed = 0;
int rightSpeed = 0;

void setRightMotor(unsigned int speed, bool isFwd) {
    digitalWrite(IN1, !isFwd);
    digitalWrite(IN2, isFwd);
    analogWrite(ENA, speed);
}

void setLeftMotor(unsigned int speed, bool isFwd) {
    digitalWrite(IN3, !isFwd);
    digitalWrite(IN4, isFwd);
    analogWrite(ENB, speed);
}

void move(int speed, int diff, bool isFwd) {
    if (diff >= 0) {
        rightSpeed = speed - diff - correction < 0 ? 0 : speed - diff - correction;
        leftSpeed = speed;
    } else {
        rightSpeed = speed - correction < 0 ? 0 : speed - correction;
        leftSpeed = speed - abs(diff) < 0 ? 0 : speed - abs(diff);
    }

    setRightMotor(rightSpeed, isFwd);
    setLeftMotor(leftSpeed, isFwd);
}

void left(int speed) {
    setRightMotor(speed, true);
    setLeftMotor(speed, false);
    Console.println("LEFT");
}
void forward(int speed) {
    setRightMotor(speed, true);
    setLeftMotor(speed, true);
    Console.println("FORWARD");
}

void backward(int speed) {
    setRightMotor(speed, false);
    setLeftMotor(speed, false);
    Console.println("BACKWARD");
}

void right(int speed) {
    setRightMotor(speed, false);
    setLeftMotor(speed, true);
    Console.println("RIGHT");
}
void stop() {
    setRightMotor(0, true);
    setLeftMotor(0, true);
    Console.println("STOP");
}