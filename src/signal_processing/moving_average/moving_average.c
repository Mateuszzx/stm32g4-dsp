#include "moving_average.h"
#include "arm_math.h"


void moving_average(float32_t *sig_src_arr,
                    float32_t *sig_dst_arr,
                    uint32_t signal_length,
                    uint32_t filter_pts)
{
    uint32_t i;
    uint32_t half_filter = filter_pts / 2;
    // Simple moving average implementation
    for (i = 0; i < signal_length; i++) {
        if (i < half_filter || i >= (signal_length - half_filter)) {
            sig_dst_arr[i] = sig_src_arr[i];
            continue;
        }
        arm_mean_f32(&sig_src_arr[i - half_filter],
                      filter_pts,
                      &sig_dst_arr[i]);
    }
}

