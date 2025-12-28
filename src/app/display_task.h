/**
 * @file display_task.h
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Display Task Header
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
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