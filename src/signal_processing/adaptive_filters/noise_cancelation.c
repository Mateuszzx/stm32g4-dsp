#include "noise_cancelation.h"
#include "lms_filter.h"

#define ITERATIONS          200
#define FILTER_ORDER        21
#define MU                  0.01f


static float32_t lms_coeffs[FILTER_ORDER];
static float32_t lms_state[FILTER_ORDER + ITERATIONS - 1];
static LMSFilter_t lms_filter;

float32_t error[ITERATIONS];

void NoiseCancelation_Init(void)
{
    lms_filter.pCoeffs = lms_coeffs;
    lms_filter.pState = lms_state;

    LMSFilter_Init(&lms_filter,
                   (uint32_t)FILTER_ORDER,
                   (float32_t)MU,
                   (uint32_t) ITERATIONS);
}

void NoiseCancelation_Execute(float32_t *pInput,
                              float32_t *pReference,
                              float32_t *pOutput,
                              uint32_t blockSize)
{

    LMSFilter_Execute(&lms_filter,
                      pInput,
                      pReference,
                      pOutput,
                      error,
                      blockSize);
}