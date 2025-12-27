#include "moving_average.h"



void moving_average(float32_t *sig_src_arr,
                    float32_t *sig_dst_arr,
                    uint32_t signal_length,
                    uint32_t filter_pts)
{
    uint32_t i;
    int32_t j;
    int32_t half_filter = filter_pts / 2;
    // Simple moving average implementation
    for (i = 0; i < signal_length; i++) {
        if (i < half_filter || i >= (signal_length - half_filter)) {
            sig_dst_arr[i] = sig_src_arr[i];
            continue;
        }
        for (j = (-1 *half_filter); j < half_filter; j++) {
            sig_dst_arr[i] += sig_src_arr[i + j];
        }
        sig_dst_arr[i] /=  filter_pts;
    }  
}


