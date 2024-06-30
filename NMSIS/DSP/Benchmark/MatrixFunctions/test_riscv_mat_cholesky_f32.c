#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_cholesky_f32/test_data.h"

BENCH_DECLARE_VAR();

void matCholesky_riscv_mat_cholesky_f32(void)
{
    float32_t f32_output[M * M];

    float32_t tmp = (float32_t)(rand() % Q31_MAX) / Q31_MAX;
    riscv_matrix_instance_f32 f32_A;
    riscv_matrix_instance_f32 f32_des;
    riscv_matrix_instance_f32 f32_posi;
    riscv_matrix_instance_f32 f32_dot;
    riscv_matrix_instance_f32 f32_tmp;
    riscv_mat_init_f32(&f32_A, M, M, f32_input_array);
    riscv_mat_init_f32(&f32_des, M, M, f32_output);
    riscv_mat_init_f32(&f32_posi, M, M, f32_posi_array);
    riscv_mat_init_f32(&f32_dot, M, M, f32_dot_array);
    riscv_mat_init_f32(&f32_tmp, M, M, f32_tmp_array);

    generate_rand_f32(f32_input_array, M * M);
    generate_rand_f32(f32_output, M * M);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (i == j) {
                f32_tmp_array[i * M + j] = tmp;
            }
        }
    }
    generate_posi_def_symme_f32(&f32_A, &f32_tmp, &f32_dot, &f32_posi);
    BENCH_START(riscv_mat_cholesky_f32);
    riscv_status result = riscv_mat_cholesky_f32(&f32_posi, &f32_des);
    BENCH_END(riscv_mat_cholesky_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
