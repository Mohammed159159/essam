#include "maze_solver.h"

int maze_speed = 100;
int maze_turnspeed = 70;
bool is_maze_enabled = false;

bool turning_right = false;
unsigned long turning_right_duration = 150;
unsigned long turning_right_start_time = 0;

bool forward_priority = false;
unsigned long forward_priority_duration = 1000;
unsigned long forward_priority_start_time = 0;

int us_sensor_readings[SONAR_NUM] = {0};

void read_us_sensors() {
    us_sensor_readings[0] = sonar[0].ping_cm();
    us_sensor_readings[1] = sonar[1].ping_cm();
    us_sensor_readings[2] = sonar[2].ping_cm();
}

NewPing sonar[SONAR_NUM] = {                      // Sensor object array.
    NewPing(LEFT_TRIG, LEFT_ECHO, MAX_DISTANCE),  // Each sensor's trigger pin, echo pin, and max distance to ping.
    NewPing(CENTER_TRIG, CENTER_ECHO, MAX_DISTANCE), NewPing(RIGHT_TRIG, RIGHT_ECHO, MAX_DISTANCE)};

void debug_us_sensors() {
    for (uint8_t i = 0; i < SONAR_NUM; i++) {  // Loop through each sensor and display results.
        delay(50);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
        Console.print(i);
        Console.print("=");
        Console.print(sonar[i].ping_cm());
        Console.print("cm ");
    }
    Console.println();

    delay(100);
}

void solve_maze() {
    read_us_sensors();

    if (us_sensor_readings[0] >= 20 && !turning_right && !forward_priority) {
        left(maze_turnspeed);
        delay(300);
        forward(maze_speed);
        forward_priority = true;
        forward_priority_start_time = millis();
    }

    if (us_sensor_readings[1] > 20) {
        forward(maze_speed);

        if (us_sensor_readings[0] > 10 && us_sensor_readings[0] < 20) {
            forward(maze_speed);
        }
        if (us_sensor_readings[0] < 10 && us_sensor_readings[0] > 0) {
            right(maze_turnspeed);
            delay(10);
        }
        if (us_sensor_readings[2] < 10 && us_sensor_readings[2] > 0) {
            left(maze_turnspeed);
            delay(10);
        }
    }

    if (us_sensor_readings[1] <= 20 && us_sensor_readings[2] > 20) {
        right(maze_turnspeed);
        turning_right = true;
        turning_right_start_time = millis();
    }

    if (us_sensor_readings[1] <= 20 && us_sensor_readings[2] < 20) {
        right(maze_turnspeed);
    }

    if (turning_right_start_time > 0 && millis() - turning_right_start_time > turning_right_duration)
    {
        turning_right = false;
        turning_right_start_time = 0;
    }

    if (forward_priority_start_time > 0 && millis() - forward_priority_start_time > forward_priority_duration)
    {
        forward_priority = false;
        forward_priority_start_time = 0;
    }
}