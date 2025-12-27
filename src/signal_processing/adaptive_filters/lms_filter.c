#include "lms_filter.h"



void LMSFilter_Init(LMSFilter_t *filter, 
                    uint32_t filterOrder,
                    float32_t mu,
                    uint32_t blockSize)
{
    arm_lms_init_f32(&(filter->lms_instance),
                     filterOrder,
                     filter->pCoeffs,
                     filter->pState,
                     mu,
                     blockSize);
}

void LMSFilter_Execute(LMSFilter_t *filter,
                       float32_t *pInput,
                       float32_t *pDesired,
                       float32_t *pOutput,
                       float32_t *error,
                       uint32_t blockSize)
{
    arm_lms_f32(&(filter->lms_instance),
                 pInput,
                 pDesired,
                 pOutput,
                error,
                 blockSize);
}