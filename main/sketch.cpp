#include <Arduino.h>
#include "controller.h"

#include "line_follower.h"
#include "maze_solver.h"

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(LL, INPUT);
    pinMode(CL, INPUT);
    pinMode(CC, INPUT);
    pinMode(CR, INPUT);
    pinMode(RR, INPUT);

    connectController();
}

void loop() {
    operateController();

    if (is_line_follower_enabled)
        follow_line();

    // Console.println("SENSOR READINGS:");
    // debug_us_sensors();
    // Console.println("-----------------------------");
    // solve_maze();
    // Console.println("++++++++++++++++++++++++++++");
    
    if (is_maze_enabled)
        solve_maze();

    // Console.printf("kp: %d | kd: %d | sharpError: %f\n | baseSpeed: %d", kp, kd, sharpError, baseSpeed);
}
