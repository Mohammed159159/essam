#include "maze_solver.h"

int maze_speed = 100;
int maze_turnspeed = 70;
bool is_maze_enabled = false;

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
    if (us_sensor_readings[1] > 20) {
        forward(maze_speed);

        if (us_sensor_readings[0] > 10 && us_sensor_readings[0] < 20) {
            forward(maze_speed);
        }
        if (us_sensor_readings[0] >= 20) {
            left(maze_turnspeed);
            delay(30);
            forward(maze_speed);
            delay(30);
        }
        if (us_sensor_readings[0] < 10 && us_sensor_readings[0] > 0) {
            right(maze_turnspeed);
            delay(30);
            forward(maze_speed);
        }

        if (us_sensor_readings[2] < 10 && us_sensor_readings[2] > 0) {
            left(maze_turnspeed);
            delay(30);
            forward(maze_speed);
        }
    }
    
    if (us_sensor_readings[1] <= 20 && us_sensor_readings[2] > 20) {
        right(maze_turnspeed);
        delay(300);
    }

    if (us_sensor_readings[1] <= 20 && us_sensor_readings[2] < 20) {
        right(maze_turnspeed);
        delay(300);
    }
}