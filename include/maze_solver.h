#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H
#include <NewPing.h>
#include "pins.h"
#include <Bluepad32.h>
#include "motors.h"

#define SONAR_NUM 3       // Number of sensors.
#define MAX_DISTANCE 200  // Maximum distance (in cm) to ping.

extern int maze_speed;
extern int maze_turnspeed;
extern bool is_maze_enabled;
extern int us_sensor_readings[SONAR_NUM];

void read_us_sensors();

extern NewPing sonar[SONAR_NUM];

void debug_us_sensors();

void solve_maze();

#endif