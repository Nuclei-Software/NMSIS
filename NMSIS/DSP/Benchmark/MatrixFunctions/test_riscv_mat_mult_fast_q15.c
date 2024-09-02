#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_mult_fast_q15/test_data.h"

BENCH_DECLARE_VAR();

void matMultFast_riscv_mat_mult_fast_q15(void)
{
    q15_t q15_output[M * N];
    q15_t q15_output_back[K * N];

    riscv_matrix_instance_q15 q15_A;
    riscv_matrix_instance_q15 q15_B;
    riscv_matrix_instance_q15 q15_des;

    riscv_mat_init_q15(&q15_A, M, K, (q15_t *)mat_mult_q15_arrayA);
    riscv_mat_init_q15(&q15_B, K, N, (q15_t *)mat_mult_q15_arrayB);
    riscv_mat_init_q15(&q15_des, M, N, q15_output);

    generate_rand_q15(mat_mult_q15_arrayA, M * K);
    generate_rand_q15(mat_mult_q15_arrayB, K * N);

    BENCH_START(riscv_mat_mult_fast_q15);
    riscv_status result = riscv_mat_mult_fast_q15(&q15_A, &q15_B, &q15_des, q15_output_back);
    BENCH_END(riscv_mat_mult_fast_q15);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);
}
