#include "line_follower.h"

int ir_sensor_readings[NUM_OF_SENSORS] = {0};
bool is_line_follower_enabled = false;
float error = 0;
float prevError = 0;
int kp = 35;
int kd = 31;
float sharpError = 2.1;
bool spinning_left = false;
int baseSpeed = 100;

void read_ir_sensors() {
    ir_sensor_readings[0] = !digitalRead(ir_sensor_pins[0]);
    ir_sensor_readings[1] = digitalRead(ir_sensor_pins[1]);
    ir_sensor_readings[2] = digitalRead(ir_sensor_pins[2]);
    ir_sensor_readings[3] = digitalRead(ir_sensor_pins[3]);
    ir_sensor_readings[4] = !digitalRead(ir_sensor_pins[4]);
}

void debug_ir_sensors() {
    read_ir_sensors();

    for (int i = 0; i < NUM_OF_SENSORS; i++)
        Console.printf("Sensor (%d): %d | ", i, ir_sensor_readings[i]);
    Console.print("\n");
}

void follow_line() {
    read_ir_sensors();
    if (ir_sensor_readings[0] == LOW) {
        spinning_left = true;
        error = -sharpError;
    } else if (ir_sensor_readings[1] == LOW) {
        error = -1;
    } else if (ir_sensor_readings[2] == LOW ) {
        spinning_left = false;
        error = 0;
    } else if (ir_sensor_readings[3] == LOW && !spinning_left) {
        error = 1;
    } else if (ir_sensor_readings[4] == LOW && !spinning_left) {
        error = sharpError;
    }


    if (error == -sharpError) {
        setRightMotor(kp * sharpError, true);
        setLeftMotor(kp * sharpError, false);
    } else if (error == sharpError) {
        setRightMotor(kp * sharpError, false);
        setLeftMotor(kp * sharpError, true);
    } else {
        setRightMotor(constrain(baseSpeed - kp * error - kd * (error - prevError), 0, 255), true);
        setLeftMotor(constrain(baseSpeed + kp * error + kd * (error - prevError), 0, 255), true);
    }

    prevError = error;
}