#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H
#include <NewPing.h>
#include <Bluepad32.h>
#include "pins.h"
#include "motors.h"
#include "controller.h"

#define SONAR_NUM 5       // Number of sensors.
#define MAX_DISTANCE 300  // Maximum distance (in cm) to ping.

#define MAZE_WIDTH 35
#define CAR_WIDTH 16

extern int maze_speed;
extern int maze_turnspeed;
extern bool is_maze_enabled;
extern int us_sensor_readings[SONAR_NUM];

extern bool turning_right;
extern bool prev_turning_right;
extern bool turning_left;
extern bool prev_turning_left;
extern bool about_to_crash;

extern unsigned long turning_left_start_time;
extern unsigned long turning_right_start_time;
extern unsigned long turning_duration;
extern bool turning_timer_expired;

extern unsigned long turning_timer_cooldown_start_time;
extern unsigned long turning_timer_cooldown_duration;

extern unsigned long ping_duration;

extern float kp_maze;
extern int error_maze;
extern int prev_error_maze;
extern float kd_maze;
extern int offset;


void read_us_sensors();

extern NewPing sonar[SONAR_NUM];

void debug_us_sensors();

void solve_maze();

#endif