#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H
#include "arm_math.h"

/**
 * @brief Calculate the moving average of a signal
 * 
 * @param sig_src_arr Pointer to the source signal array
 * @param sig_dst_arr Pointer to the destination array for the filtered signal
 * @param signal_length Length of the signal array
 * @param filter_pts Number of points in the moving average filter window
 */
void moving_average(float32_t *sig_src_arr,
                    float32_t *sig_dst_arr,
                    uint32_t signal_length,
                    uint32_t filter_pts);

#endif // MOVING_AVERAGE_H