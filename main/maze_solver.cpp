#include "maze_solver.h"
int maze_speed = 100;
int maze_turnspeed = 75;
bool is_maze_enabled = false;

unsigned long turning_duration = 30;
unsigned long turning_right_start_time = 0;
unsigned long turning_left_start_time = 0;
bool turning_timer_expired = false;

bool turning_left = false;
bool prev_turning_left = turning_left;
bool turning_right = false;
bool prev_turning_right = turning_right;
bool about_to_crash = false;

unsigned long turning_timer_cooldown_duration = 500;
unsigned long turning_timer_cooldown_start_time = 0;

unsigned long ping_duration = 30;

float kp_maze = 0.4;
int error_maze = 0;
int offset = 0;

float kd_maze = 0.0;
int prev_error_maze = error_maze;

int us_sensor_readings[SONAR_NUM];

NewPing sonar[SONAR_NUM] = {  // Sensor object array.
    NewPing(LEFT_TRIG, LEFT_ECHO, MAX_DISTANCE),
    NewPing(CENTER_LEFT_TRIG,
            CENTER_LEFT_ECHO,
            MAX_DISTANCE),  // Each sensor's trigger pin, echo pin, and max distance to ping.
    NewPing(CENTER_TRIG, CENTER_ECHO, MAX_DISTANCE), NewPing(CENTER_RIGHT_TRIG, CENTER_RIGHT_ECHO, MAX_DISTANCE),
    NewPing(RIGHT_TRIG, RIGHT_ECHO, MAX_DISTANCE)};

void read_us_sensors() {
    static int i = 0;
    static unsigned long ping_start_time = millis();
    if (millis() - ping_start_time > ping_duration) {
        us_sensor_readings[i] = sonar[i].ping_cm();
        ping_start_time = millis();
        if (us_sensor_readings[i] <= 0)
            us_sensor_readings[i] = 8;

        i++;

        if (i == SONAR_NUM)
            i = 0;
    }
}
void debug_us_sensors() {
    read_us_sensors();
    for (uint8_t i = 0; i < SONAR_NUM; i++) {  // Loop through each sensor and display results.
        Console.print(i);
        Console.print("=");
        Console.print(us_sensor_readings[i]);
        Console.print("cm ");
    }
    Console.println();
}

void solve_maze() {
    // read_us_sensors();
    debug_us_sensors();

    if (us_sensor_readings[2] >= MAZE_WIDTH * 0.8 && !about_to_crash)
        forward(maze_speed);
    else if (us_sensor_readings[0] >= MAZE_WIDTH * 1.1 && !turning_right) {
        turning_left = true;
        left(maze_turnspeed);
        // delay(30);
    } else if (us_sensor_readings[4] >= MAZE_WIDTH * 1.1 && !turning_left) {
        turning_right = true;
        right(maze_turnspeed);
        // delay(30);
    }
    if (us_sensor_readings[1] <= 15 && !turning_left) {
        turning_right = true;
        about_to_crash = true;
        turning_right_start_time = millis();
        right(maze_turnspeed);
        // delay(10);
    }
    else if (us_sensor_readings[0] < 8) {
        right(maze_turnspeed);
        // delay(10);
    }
    if (us_sensor_readings[3] <= 15 && !turning_right) {
        turning_left = true;
        about_to_crash = true;
        turning_left_start_time = millis();
        left(maze_turnspeed);
        // delay(10);
    }
    else if (us_sensor_readings[4] < 8) {
        left(maze_turnspeed);
        // delay(10);
    }
    
    if (turning_left_start_time > 0 && millis() - turning_left_start_time > turning_duration) {
        turning_left = false;
    }
    if (turning_right_start_time > 0 && millis() - turning_right_start_time > turning_duration) {
        turning_right = false;
    }
    if (us_sensor_readings[1] > 10 && us_sensor_readings[3] > 10)
        about_to_crash = false;
    if (us_sensor_readings[0] < 20)
        turning_left = false;
    if (us_sensor_readings[2] < 20)
        turning_right = false;
}