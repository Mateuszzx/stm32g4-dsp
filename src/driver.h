#ifndef DRIVER_H
#define DRIVER_H


#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os2.h"
#include "arm_math.h"

#include "waveform_generator/sine_generator.h"

void driver_init(void);

void driver_loop(void);

#endif // DRIVER_H