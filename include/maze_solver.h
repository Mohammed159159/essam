#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H
#include <NewPing.h>
#include "pins.h"
#include <Bluepad32.h>

#define SONAR_NUM 3       // Number of sensors.
#define MAX_DISTANCE 200  // Maximum distance (in cm) to ping.

extern NewPing sonar[SONAR_NUM];

void debug_us_sensors();

#endif