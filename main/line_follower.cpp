#include "line_follower.h"

int ir_sensor_readings[NUM_OF_SENSORS] = {0};
bool is_line_follower_enabled = false;
float error = 0;
float prevError = 0;
float kp = 37.7;
float kd = 0;
float sharpError = 2.1;
int baseSpeed = 100;
unsigned long spin_start_time = 0;
unsigned long spin_duration = 50;

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
    // read_ir_sensors();
    debug_ir_sensors();


    if (ir_sensor_readings[0] == LOW) {
        spin_start_time = millis();
        error = -sharpError;
    } else if (ir_sensor_readings[1] == LOW) {
        error = -1;
    } else if (ir_sensor_readings[2] == LOW) {
        error = 0;
    } else if (ir_sensor_readings[3] == LOW) {
        error = 1;
    } else if (ir_sensor_readings[4] == LOW) {
        error = sharpError;
    }

    if (spin_start_time) {
        if (millis() - spin_start_time < spin_duration)
            error = -sharpError;
        else
            spin_start_time = 0;
    }

    if (error == -sharpError) {
        setRightMotor(kp * sharpError, true);
        Console.printf("Right motor - FWD: %f\n", kp * sharpError);
        setLeftMotor(kp * sharpError, false);
        Console.printf("Left motor - BWD: %f\n", kp * sharpError);
    } else if (error == sharpError) {
        setRightMotor(kp * sharpError, false);
        Console.printf("Right motor - BWD: %f\n", kp * sharpError);
        setLeftMotor(kp * sharpError, true);
        Console.printf("Left motor - FWD: %f\n", kp * sharpError);

    } else {
        if (prevError == abs(sharpError))
            prevError = error;

        setRightMotor(constrain(baseSpeed - kp * error - kd * (error - prevError), 0, 255), true);
        Console.printf("Right motor: %f\n", baseSpeed - kp * error - kd * (error - prevError));
        setLeftMotor(constrain(baseSpeed + kp * error + kd * (error - prevError), 0, 255), true);
        Console.printf("Left motor: %f\n", baseSpeed + kp * error + kd * (error - prevError));
        prevError = error;
    }
    Console.printf("Error: %f\n", error);

}