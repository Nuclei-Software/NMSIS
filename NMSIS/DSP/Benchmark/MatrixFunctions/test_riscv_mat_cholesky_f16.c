#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_cholesky_f16/test_data.h"

BENCH_DECLARE_VAR();

void matCholesky_riscv_mat_cholesky_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output[M * M];

    float16_t tmp = (float16_t)(rand() % Q31_MAX) / Q31_MAX;
    riscv_matrix_instance_f16 f16_A;
    riscv_matrix_instance_f16 f16_des;
    riscv_matrix_instance_f16 f16_posi;
    riscv_matrix_instance_f16 f16_dot;
    riscv_matrix_instance_f16 f16_tmp;
    riscv_mat_init_f16(&f16_A, M, M, f16_input_array);
    riscv_mat_init_f16(&f16_des, M, M, f16_output);
    riscv_mat_init_f16(&f16_posi, M, M, f16_posi_array);
    riscv_mat_init_f16(&f16_dot, M, M, f16_dot_array);
    riscv_mat_init_f16(&f16_tmp, M, M, f16_tmp_array);

    generate_rand_f16(f16_input_array, M * M);
    generate_rand_f16(f16_output, M * M);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (i == j) {
                f16_tmp_array[i * M + j] = tmp;
            }
        }
    }

    generate_posi_def_symme_f16(&f16_A, &f16_tmp, &f16_dot, &f16_posi);
    BENCH_START(riscv_mat_cholesky_f16);
    riscv_status result = riscv_mat_cholesky_f16(&f16_posi, &f16_des);
    BENCH_END(riscv_mat_cholesky_f16);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}

