#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SVMFunctions/svm_rbf_predict_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void svm_riscv_svm_rbf_predict_f16(void)
{
    int32_t result;
    riscv_svm_rbf_instance_f16 params;

    for (int i = 0; i < NB_SUPPORT_VECTORS; i++) {
        dualCoefficients[i] = (float16_t)(rand() / RAND_MAX * 2 - 1);
    }
    generate_rand_f16(f16_input, VECTOR_DIMENSION);
    generate_rand_f16(supportVectors, NB_SUPPORT_VECTORS * VECTOR_DIMENSION);

    riscv_svm_rbf_init_f16(&params,
        NB_SUPPORT_VECTORS,
        VECTOR_DIMENSION,
        intercept,        /* Intercept */
        dualCoefficients,
        supportVectors,
        classes,
        gamma_f16          /* Gamma */
    );

    BENCH_START(riscv_svm_rbf_predict_f16);
    riscv_svm_rbf_predict_f16(&params, f16_input, &result);
    BENCH_END(riscv_svm_rbf_predict_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
