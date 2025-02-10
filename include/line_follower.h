#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H
#include "pins.h"
#include <Arduino.h>
#include <Bluepad32.h>
#include "motors.h"

#define NUM_OF_SENSORS 5
const int ir_sensor_pins[NUM_OF_SENSORS] = {LL, CL, CC, CR, RR};
extern int ir_sensor_readings[NUM_OF_SENSORS];
extern bool is_line_follower_enabled;
extern float error;
extern float prevError;
extern float kp;
extern float kd;
extern float sharpError;
extern unsigned long spin_start_time;
extern unsigned long spin_duration;

extern int baseSpeed;

void read_ir_sensors();

void debug_ir_sensors();

void follow_line();

void sharpleft();
void left();
void forward();
void right();
void sharpright();

#endif