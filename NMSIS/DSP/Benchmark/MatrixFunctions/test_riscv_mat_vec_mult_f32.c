#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_vec_mult_f32/test_data.h"

BENCH_DECLARE_VAR();

void matVecMult_riscv_mat_vec_mult_f32(void)
{
    float32_t f32_output[M];
    riscv_matrix_instance_f32 f32_A;
    riscv_mat_init_f32(&f32_A, M, N, mat_f32_array);

    generate_rand_f32(mat_f32_array, M * N);
    generate_rand_f32(mat_f32_vec, N);

    BENCH_START(riscv_mat_vec_mult_f32);
    riscv_mat_vec_mult_f32(&f32_A, mat_f32_vec, f32_output);
    BENCH_END(riscv_mat_vec_mult_f32);

    return;
}