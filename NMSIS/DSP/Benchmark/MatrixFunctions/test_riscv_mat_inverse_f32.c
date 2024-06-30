#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_inverse_f32/test_data.h"

BENCH_DECLARE_VAR();

void matInverse_riscv_mat_inverse_f32(void)
{
    float32_t f32_output[M * M];
    riscv_matrix_instance_f32 f32_A;
    riscv_matrix_instance_f32 f32_des;
    riscv_mat_init_f32(&f32_A, M, M, mat_inverse_f32_input);
    riscv_mat_init_f32(&f32_des, M, M, f32_output);

    generate_rand_f32(mat_inverse_f32_input, M * M);

    BENCH_START(riscv_mat_inverse_f32);
    riscv_status result = riscv_mat_inverse_f32(&f32_A, &f32_des);
    BENCH_END(riscv_mat_inverse_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}