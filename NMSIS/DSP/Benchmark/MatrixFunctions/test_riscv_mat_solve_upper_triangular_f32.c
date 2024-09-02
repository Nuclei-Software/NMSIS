#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_solve_upper_triangular_f32/test_data.h"

BENCH_DECLARE_VAR();

void matSolveUpper_riscv_mat_solve_upper_triangular_f32(void)
{
    float32_t f32_output[M * N];
    riscv_matrix_instance_f32 f32_A;
    riscv_matrix_instance_f32 f32_B;
    riscv_matrix_instance_f32 f32_des;

    riscv_mat_init_f32(&f32_A, M, N, mat_ut);
    riscv_mat_init_f32(&f32_B, M, N, mat_su_f32_input);
    riscv_mat_init_f32(&f32_des, M, N, f32_output);

    generate_rand_f32(mat_su_f32_input, M * N);
    for (int i = 0; i < M * N; i++) {
        mat_ut[i] = (float32_t)rand();
    }
    for (int i = 0; i < M; i++) {
        for(int j = 0; j < i; j++) {
            mat_ut[i * M + j] = 0;
        }
    }

    BENCH_START(riscv_mat_solve_upper_triangular_f32);
    riscv_status result = riscv_mat_solve_upper_triangular_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_solve_upper_triangular_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
