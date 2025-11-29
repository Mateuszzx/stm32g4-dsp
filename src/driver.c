#include "driver.h"

static float a;

void driver_init(void) {
    // Initialization code for the driver
    a = 0.0f;
}

void driver_loop(void) {
    // Main loop code for the driver
    a += 1.5f;
}