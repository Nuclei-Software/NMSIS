#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_add_f32/test_data.h"

BENCH_DECLARE_VAR();

void matAdd_riscv_mat_add_f32(void)
{
    riscv_matrix_instance_f32 f32_A;
    riscv_matrix_instance_f32 f32_B;
    riscv_matrix_instance_f32 f32_des;
    float32_t mat_add_f32_output[M * N];

    riscv_mat_init_f32(&f32_A, M, N, mat_add_f32_arrayA);
    riscv_mat_init_f32(&f32_B, M, N, mat_add_f32_arrayB);
    riscv_mat_init_f32(&f32_des, M, N, mat_add_f32_output);

    generate_rand_f32(mat_add_f32_arrayA, M * N);
    generate_rand_f32(mat_add_f32_arrayB, M * N);

    BENCH_START(riscv_mat_add_f32);
    riscv_status result = riscv_mat_add_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_add_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
