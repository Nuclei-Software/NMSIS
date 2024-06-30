#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SVMFunctions/svm_rbf_predict_f32/test_data.h"

BENCH_DECLARE_VAR();

void svm_riscv_svm_rbf_predict_f32(void)
{
    int32_t result;
    riscv_svm_rbf_instance_f32 params;

    for (int i = 0; i < NB_SUPPORT_VECTORS; i++) {
        dualCoefficients[i] = (float32_t)rand() / RAND_MAX * 2 - 1;
    }
    generate_rand_f32(f32_input, VECTOR_DIMENSION);
    generate_rand_f32(supportVectors, NB_SUPPORT_VECTORS * VECTOR_DIMENSION);

    riscv_svm_rbf_init_f32(&params,
        NB_SUPPORT_VECTORS,
        VECTOR_DIMENSION,
        intercept,        /* Intercept */
        dualCoefficients,
        supportVectors,
        classes,
        gamma_f32          /* Gamma */
    );

    BENCH_START(riscv_svm_rbf_predict_f32);
    riscv_svm_rbf_predict_f32(&params, f32_input, &result);
    BENCH_END(riscv_svm_rbf_predict_f32);

    return;
}
