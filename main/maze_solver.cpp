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

unsigned long ping_duration = 30;

float kp_maze = 0.8;
int sharp_error_maze = 25;
int error_maze = 0;
int offset = 0;
int us_sensor_readings[SONAR_NUM];

NewPing sonar[SONAR_NUM] = {                      // Sensor object array.
    NewPing(LEFT_TRIG, LEFT_ECHO, MAX_DISTANCE),  // Each sensor's trigger pin, echo pin, and max distance to ping.
    NewPing(CENTER_TRIG, CENTER_ECHO, MAX_DISTANCE), NewPing(RIGHT_TRIG, RIGHT_ECHO, MAX_DISTANCE)};

void read_us_sensors() {
    static int i = 0;
    static unsigned long ping_start_time = millis();
    if (millis() - ping_start_time > ping_duration) {
        us_sensor_readings[i] = sonar[i].ping_cm();
        ping_start_time = millis();
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
    read_us_sensors();

    if (us_sensor_readings[0] < MAZE_WIDTH && us_sensor_readings[2] < MAZE_WIDTH)
        error_maze = us_sensor_readings[2] - us_sensor_readings[0] - offset;
    else if (us_sensor_readings[0] < MAZE_WIDTH)
        error_maze = (MAZE_WIDTH - us_sensor_readings[0] - CAR_WIDTH) - us_sensor_readings[0];
    else if (us_sensor_readings[2] < MAZE_WIDTH)
        error_maze = us_sensor_readings[2] - (MAZE_WIDTH - us_sensor_readings[2] - CAR_WIDTH);
    else
        error_maze = 0;

    if (abs(error_maze) > 3) {
        setRightMotor(constrain(maze_speed - kp_maze * error_maze, 0, 255), FORWARD);
        setLeftMotor(constrain(maze_speed + kp_maze * error_maze, 0, 255), FORWARD);
    }

    if (us_sensor_readings[0] >= MAZE_WIDTH) {
        setRightMotor(constrain(maze_speed - kp_maze * -sharp_error_maze, 0, 255), FORWARD);
        setLeftMotor(constrain(maze_speed + kp_maze * -sharp_error_maze, 0, 255), FORWARD);
    }
    else if (us_sensor_readings[1] >= MAZE_WIDTH) {
        setRightMotor(maze_speed, FORWARD);
        setLeftMotor(maze_speed, FORWARD);

    } else if (us_sensor_readings[2] >= MAZE_WIDTH) {
        setRightMotor(constrain(maze_speed - kp_maze * sharp_error_maze, 0, 255), FORWARD);
        setLeftMotor(constrain(maze_speed + kp_maze * sharp_error_maze, 0, 255), FORWARD);
    } else {
        stop();
    }
}