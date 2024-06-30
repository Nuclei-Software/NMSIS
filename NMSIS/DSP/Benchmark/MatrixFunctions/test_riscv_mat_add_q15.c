#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_add_q15/test_data.h"

BENCH_DECLARE_VAR();

void matAdd_riscv_mat_add_q15(void)
{
    riscv_matrix_instance_q15 q15_A;
    riscv_matrix_instance_q15 q15_B;
    riscv_matrix_instance_q15 q15_des;
    q15_t mat_add_q15_output[M * N];

    riscv_mat_init_q15(&q15_A, M, N, (q15_t *)mat_add_q15_arrayA);
    riscv_mat_init_q15(&q15_B, M, N, (q15_t *)mat_add_q15_arrayB);
    riscv_mat_init_q15(&q15_des, M, N, mat_add_q15_output);

    generate_rand_q15(mat_add_q15_arrayA, M * N);
    generate_rand_q15(mat_add_q15_arrayB, M * N);

    BENCH_START(riscv_mat_add_q15);
    riscv_status result = riscv_mat_add_q15(&q15_A, &q15_B, &q15_des);
    BENCH_END(riscv_mat_add_q15);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}