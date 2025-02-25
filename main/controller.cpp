#include "controller.h"

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void print(String str) {
    static unsigned long print_start_time = 0;
    if (millis() - print_start_time > 500) {
        Console.println(str.c_str());
        print_start_time = millis();
    }
}

void connectController() {
    Console.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Console.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    // Setup the Bluepad32 callbacks, and the default behavior for scanning or not.
    // By default, if the "startScanning" parameter is not passed, it will do the "start scanning".
    // Notice that "Start scanning" will try to auto-connect to devices that are compatible with Bluepad32.
    // E.g: if a Gamepad, keyboard or mouse are detected, it will try to auto connect to them.
    bool startScanning = true;
    BP32.setup(&onConnectedController, &onDisconnectedController, startScanning);

    // Notice that scanning can be stopped / started at any time by calling:
    // BP32.enableNewBluetoothConnections(enabled);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();

    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);

    // Enables the BLE Service in Bluepad32.
    // This service allows clients, like a mobile app, to setup and see the state of Bluepad32.
    // By default, it is disabled.
    BP32.enableBLEService(false);
}

void control_robot() {
    // This call fetches all the controllers' data.
    // Call this function in your main loop.
    bool dataUpdated = BP32.update();

    if (dataUpdated)
        processControllers();
}

void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Console.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Console.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Console.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Console.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Console.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
    Console.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
    );
}

void processGamepad(ControllerPtr ctl) {
    const int threshold = 50;
    diff = 0;
    motorSpeed = 0;

    if (ctl->axisX() > threshold) {
        diff = map(ctl->axisX(), threshold, 512, minDiff, 255);
    }
    if (ctl->axisX() < -threshold) {
        diff = map(ctl->axisX(), -threshold, -511, -minDiff, -255);
    }
    if (abs(diff) < minDiff) {
        diff = 0;
    }

    if (ctl->throttle()) {
        motorSpeed = map(ctl->throttle(), threshold, 1023, minSpeed, 255);
        isFwd = true;
    } else if (ctl->brake()) {
        motorSpeed = map(ctl->brake(), threshold, 1023, minSpeed, 255);
        isFwd = false;
    }
    if (motorSpeed < minSpeed) {
        motorSpeed = 0;
    }

    move(motorSpeed, diff, isFwd);

    if (abs(ctl->axisRX()) > threshold) {
        if (ctl->axisRX() > threshold) {
            motorSpeed = map(ctl->axisRX(), threshold, 511, minSpeed, 255);
        }
        if (ctl->axisRX() < -threshold) {
            motorSpeed = map(ctl->axisRX(), -threshold, -512, -minSpeed, -255);
        }
        if (abs(motorSpeed) < minSpeed) {
            motorSpeed = 0;
        }

        if (motorSpeed > 0)
            right(motorSpeed);
        else if (motorSpeed < 0)
            left(-motorSpeed);

    }

    // Console.printf("axisRx: %d | Motor speed: %d\n", ctl->axisRX(), motorSpeed);

    bool currentYState = ctl->y();
    static bool prevYState = false;

    if (currentYState && !prevYState)
        is_line_follower_enabled = !is_line_follower_enabled;

    prevYState = currentYState;

    bool currentAState = ctl->a();
    static bool prevAState = false;

    if (currentAState && !prevAState)
        is_maze_enabled = !is_maze_enabled;

    prevAState = currentAState;

    bool currentBState = ctl->b();
    static bool prevBState = false;

    if (currentBState && !prevBState)
        baseSpeed -= 10;

    prevBState = currentBState;

    int currentDpadState = ctl->dpad();
    static int prevDpadState = -1;
    if (currentDpadState == DPAD_UP && prevDpadState != DPAD_UP)
        kp_maze += 0.1;
    if (currentDpadState == DPAD_DOWN && prevDpadState != DPAD_DOWN)
        kp_maze -= 0.1;
    if (currentDpadState == DPAD_RIGHT && prevDpadState != DPAD_RIGHT)
        kd_maze += 0.1;
    if (currentDpadState == DPAD_LEFT && prevDpadState != DPAD_LEFT)
        kd_maze -= 0.1;
    prevDpadState = currentDpadState;

    bool currentR1State = ctl->r1();
    static bool prevR1State = false;

    if (currentR1State && !prevR1State)
        sharpError += 0.1;

    prevR1State = currentR1State;

    bool currentL1State = ctl->l1();
    static bool prevL1State = false;

    if (currentL1State && !prevL1State)
        sharpError -= 0.1;

    prevL1State = currentL1State;

    // if (ctl->a() || ctl->y()) {
    //     // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
    //     // It is possible to set it by calling:
    //     // Some controllers have two motors: "strong motor", "weak motor".
    //     // It is possible to control them independently.
    //     ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
    //                         0x40 /* strongMagnitude */);
    // }

    // Another way to query controller data is by getting the buttons() function.
    // See how the different "dump*" functions dump the Controller info.
    // dumpGamepad(ctl);

    // See ArduinoController.h for all the available functions.
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else {
                Console.printf("Unsupported controller\n");
            }
        }
    }
}
