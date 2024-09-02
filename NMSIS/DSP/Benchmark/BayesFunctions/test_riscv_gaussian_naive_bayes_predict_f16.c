#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BayesFunctions/bayes_f16/test_data.h"

BENCH_DECLARE_VAR();

void bayes_riscv_gaussian_naive_bayes_predict_f16(void)
{
#if defined(RISCV_FLOAT16_SUPPORTED)
    /* Result of the classifier */
    float16_t bayes_f16_output[NB_OF_CLASSES];
    riscv_gaussian_naive_bayes_instance_f16 S_f16;

    S_f16.vectorDimension = VECTOR_DIMENSION;
    S_f16.numberOfClasses = NB_OF_CLASSES;

    for (int i = 0; i < NB_OF_CLASSES * VECTOR_DIMENSION; i++) {
        theta_f16[i] = (float16_t)theta_f32[i];
        sigma_f16[i] = (float16_t)sigma_f32[i];
    }

    for (int i = 0; i < NB_OF_CLASSES; i++) {
        classPriors_f16[i] = (float16_t)classPriors_f32[i];
    }

    S_f16.theta = theta_f16;
    S_f16.sigma = sigma_f16;
    S_f16.classPriors = classPriors_f16;
    S_f16.epsilon = (float16_t)epsilon_f32;

    bayes_f16_input[0] = (float16_t)1.5f;
    bayes_f16_input[1] = (float16_t)1.0f;

    BENCH_START(riscv_gaussian_naive_bayes_predict_f16);
    riscv_gaussian_naive_bayes_predict_f16(&S_f16, bayes_f16_input, bayes_f16_output, pBufferB_f16);
    BENCH_END(riscv_gaussian_naive_bayes_predict_f16);

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
}

