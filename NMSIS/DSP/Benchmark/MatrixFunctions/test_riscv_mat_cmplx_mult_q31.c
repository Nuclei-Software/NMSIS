#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_cmplx_mult_q31/test_data.h"

BENCH_DECLARE_VAR();

void matCmplxMult_riscv_mat_cmplx_mult_q31(void)
{
    q31_t q31_output[M * N];

    riscv_matrix_instance_q31 q31_A;
    riscv_matrix_instance_q31 q31_B;
    riscv_matrix_instance_q31 q31_des;

    riscv_mat_init_q31(&q31_A, M, K / 2, (q31_t *)mat_cmplx_mult_q31_arrayA);
    riscv_mat_init_q31(&q31_B, K / 2, N / 2, (q31_t *)mat_cmplx_mult_q31_arrayB);
    riscv_mat_init_q31(&q31_des, M, N, q31_output);

    generate_rand_q31(mat_cmplx_mult_q31_arrayA, M * K);
    generate_rand_q31(mat_cmplx_mult_q31_arrayB, K * N);

    BENCH_START(riscv_mat_cmplx_mult_q31);
    riscv_status result = riscv_mat_cmplx_mult_q31(&q31_A, &q31_B, &q31_des);
    BENCH_END(riscv_mat_cmplx_mult_q31);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}