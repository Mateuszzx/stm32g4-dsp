#include "moving_avarage.h"



void moving_average(float32_t *sig_src_arr,
                    float32_t *sig_dst_arr,
                    uint32_t signal_length,
                    uint32_t filter_pts)
{
    float32_t sum;
    uint32_t i;
    int32_t j;
    uint32_t half_filter = filter_pts / 2;

    for (i = half_filter; i < (signal_length - half_filter); i++) {
        sum = 0.0f;
        for (j = 0; j < filter_pts; j++) {
            if (i >= j) {
                sum += sig_src_arr[i - j];
            }
        }
        sig_dst_arr[i] = sum / filter_pts;
    }  
}


