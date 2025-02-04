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



void sharpleft(){
    setRightMotor(120, true);
    setLeftMotor(120, false);

}
void left(){
    setRightMotor(170, true);
    setLeftMotor(80, true);
}
void forward(){
    setRightMotor(100, true);
    setLeftMotor(100, true);
}
void right(){
    setRightMotor(80, true);
    setLeftMotor(170, true);
}
void sharpright(){
    setRightMotor(120, false);
    setLeftMotor(120, true);
}