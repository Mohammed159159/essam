#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sdkconfig.h"

#include <Arduino.h>
#include <Bluepad32.h>
#include "motors.h"
#include "line_follower.h"
#include "maze_solver.h"
#include "pins.h"

extern ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void print(String str);

void connectController();

void control_robot();

void onConnectedController(ControllerPtr ctl);

void onDisconnectedController(ControllerPtr ctl);

void dumpGamepad(ControllerPtr ctl);

void processGamepad(ControllerPtr ctl);

void dumpKeyboard(ControllerPtr ctl);

void processKeyboard(ControllerPtr ctl);

void processControllers();

#endif