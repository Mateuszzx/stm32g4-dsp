#ifndef DISPLAY_TASK_H
#define DISPLAY_TASK_H

#include <stdio.h>
#include "uart_driver.h"


/**
 * @brief Display Task
 * 
 * @param params Argument passed to the task (AppContext pointer)
 */
void DisplayTask(void *params);


#endif // DISPLAY_TASK_H