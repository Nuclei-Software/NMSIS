#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_cmplx_mult_f32/test_data.h"

BENCH_DECLARE_VAR();

void matCmplxMult_riscv_mat_cmplx_mult_f32(void)
{
    float32_t f32_output[2 * M * N];
    riscv_matrix_instance_f32 f32_A;
    riscv_matrix_instance_f32 f32_B;
    riscv_matrix_instance_f32 f32_des;
    riscv_mat_init_f32(&f32_A, M, K / 2, (float32_t *)mat_cmplx_mult_f32_arrayA);
    riscv_mat_init_f32(&f32_B, K / 2, N, (float32_t *)mat_cmplx_mult_f32_arrayB);
    riscv_mat_init_f32(&f32_des, M, N, f32_output);

    generate_rand_f32(mat_cmplx_mult_f32_arrayA, M * K);
    generate_rand_f32(mat_cmplx_mult_f32_arrayB, K * N);

    BENCH_START(riscv_mat_cmplx_mult_f32);
    riscv_status result = riscv_mat_cmplx_mult_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_cmplx_mult_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
