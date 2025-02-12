#include "maze_solver.h"
int maze_speed = 100;
int maze_turnspeed = 70;
bool is_maze_enabled = false;

unsigned long turning_duration = 3000;
unsigned long turning_start_time = 0;
bool turning_timer_expired = false;

bool turning_left = false;
bool turning_right = false;

unsigned long turning_timer_cooldown_duration = 500;
unsigned long turning_timer_cooldown_start_time = 0;

unsigned long ping_duration = 30;

float kp_maze = 0.6;
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

    if (us_sensor_readings[0] >= MAZE_WIDTH * 1.5 && !turning_right) {
        print("TURNING LEFT....");
        turning_start_time = millis();
        turning_left = true;
        move(maze_speed, -maze_speed * kp_maze * 1.2, FORWARD);
    } else if (us_sensor_readings[1] >= MAZE_WIDTH && (!turning_right && !turning_left)) {
        print("PID....");
        error_maze = us_sensor_readings[2] - us_sensor_readings[0] - offset;

        error_maze = constrain(error_maze, -MAZE_WIDTH, MAZE_WIDTH);

        if (abs(error_maze) > MAZE_WIDTH) {
        } else {
            error_maze = 0;
        }

        setRightMotor(constrain(maze_speed - kp_maze * error_maze, 0, 255), FORWARD);
        setLeftMotor(constrain(maze_speed + kp_maze * error_maze, 0, 255), FORWARD);

    } else if (us_sensor_readings[2] >= MAZE_WIDTH * 1.5 && !turning_left) {
        print("TURNING RIGHT....");
        turning_start_time = millis();
        turning_right = true;
        move(maze_speed, maze_speed * kp_maze * 1.2, FORWARD);
    }
    // else {
    //     stop();
    // }

    // if (us_sensor_readings[0] < MAZE_WIDTH && us_sensor_readings[1] < MAZE_WIDTH / 2 &&
    //     us_sensor_readings[2] < MAZE_WIDTH) {
    //     right(maze_speed);
    // }

    if (us_sensor_readings[0] < MAZE_WIDTH)
        turning_left = false;
    if (us_sensor_readings[2] < MAZE_WIDTH)
        turning_right = false;

    if (turning_start_time > 0 && millis() - turning_start_time > turning_duration) {
        print("TURNING_TIMER_EXPIRED!");
        turning_timer_expired = true;
        turning_start_time = 0;
        turning_timer_cooldown_start_time = millis();
    }
    if (turning_timer_cooldown_start_time > 0 &&
        millis() - turning_timer_cooldown_start_time > turning_timer_cooldown_duration) {
        print("TURNING_TIMER_COOLED_DOWN!");

        turning_timer_expired = false;
        turning_timer_cooldown_start_time = 0;
    }
}