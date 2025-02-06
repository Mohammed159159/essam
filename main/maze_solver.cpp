#include "maze_solver.h"

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