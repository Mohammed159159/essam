set(srcs
        "main.c"
        "sketch.cpp"
        "motors.cpp"
        "line_follower.cpp"
        )

set(requires "bluepad32" "bluepad32_arduino" "arduino" "btstack")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
