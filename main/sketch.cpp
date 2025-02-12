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

unsigned long console_start_time = 0;

void loop() {
    operateController();

    if (is_line_follower_enabled)
        follow_line();

    // Console.println("DECISION:");
    // solve_maze();
    // Console.println("++++++++++++++++++++++++++++");
    // Console.println("SENSOR READINGS:");
    // debug_us_sensors();
    // Console.println("-----------------------------");

    if (is_maze_enabled)
        solve_maze();

    if (millis() - console_start_time > 500) {
        debug_us_sensors();
        Console.printf("kp_maze: %f, error_maze: %d\n", kp_maze, error_maze);
        console_start_time = millis();
    }
    vTaskDelay(1);
}
