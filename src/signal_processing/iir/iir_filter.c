#include "iir_filter.h"


void IIRFilter_Init(IIRFilter_t *filter,
                    const float32_t *pCoeffs,
                    float32_t *pState,
                    uint32_t numStages)
{
    arm_biquad_cascade_df1_init_f32(&(filter->biquad_instance),
                                    numStages,
                                    pCoeffs,
                                    pState);
                                    
    filter->pCoeffs = (float32_t *)pCoeffs;
    filter->pState = pState;
    filter->numStages = numStages;
}

void IIRFilter_Execute(IIRFilter_t *filter,
                       float32_t *pSrc,
                       float32_t *pDst,
                       uint32_t blockSize)
{
    arm_biquad_cascade_df1_f32(&(filter->biquad_instance),
                               pSrc,
                               pDst,
                               blockSize);
}

