#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BayesFunctions/bayes_f32/test_data.h"

BENCH_DECLARE_VAR();

void bayes_riscv_gaussian_naive_bayes_predict_f32(void)
{
    /* Result of the classifier */
    float32_t bayes_f32_output[NB_OF_CLASSES];
    riscv_gaussian_naive_bayes_instance_f32 S_f32;

    S_f32.vectorDimension = VECTOR_DIMENSION;
    S_f32.numberOfClasses = NB_OF_CLASSES;
    S_f32.theta = theta_f32;
    S_f32.sigma = sigma_f32;
    S_f32.classPriors = classPriors_f32;
    S_f32.epsilon = epsilon_f32;

    bayes_f32_input[0] = 1.5f;
    bayes_f32_input[1] = 1.0f;

    BENCH_START(riscv_gaussian_naive_bayes_predict_f32);
    riscv_gaussian_naive_bayes_predict_f32(&S_f32, bayes_f32_input, bayes_f32_output, pBufferB_f32);
    BENCH_END(riscv_gaussian_naive_bayes_predict_f32);

    return;
}
