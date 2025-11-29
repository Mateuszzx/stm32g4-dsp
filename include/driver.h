#ifndef DRIVER_H
#define DRIVER_H


#include <stdint.h>
#include <stdbool.h>

#include "arm_math.h"

void driver_init(void);

void driver_loop(void);

#endif // DRIVER_H