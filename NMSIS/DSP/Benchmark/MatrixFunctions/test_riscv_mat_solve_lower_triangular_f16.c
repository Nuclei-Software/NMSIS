#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_solve_lower_triangular_f16/test_data.h"

BENCH_DECLARE_VAR();

void matSolveLower_riscv_mat_solve_lower_triangular_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output[M * N];
    riscv_matrix_instance_f16 f16_A;
    riscv_matrix_instance_f16 f16_B;
    riscv_matrix_instance_f16 f16_des;

    riscv_mat_init_f16(&f16_A, M, N, mat_lt);
    riscv_mat_init_f16(&f16_B, M, N, mat_sl_f16_input);
    riscv_mat_init_f16(&f16_des, M, N, f16_output);

    generate_rand_f16(mat_sl_f16_input, M * N);
    for (int i=0; i < M; i++) {
        for (int j = M - 1; j > i; j--) {
            mat_lt[i * M + j] = 0;
        }
        for (int j = 0; j <= i; j++)
            mat_lt[i * M + j] = (float16_t)rand();
    }

    BENCH_START(riscv_mat_solve_lower_triangular_f16);
    riscv_status result = riscv_mat_solve_lower_triangular_f16(&f16_A, &f16_B, &f16_des);
    BENCH_END(riscv_mat_solve_lower_triangular_f16);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}

