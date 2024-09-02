#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_vec_mult_q15/test_data.h"

BENCH_DECLARE_VAR();

void matVecMult_riscv_mat_vec_mult_q15(void)
{
    q15_t q15_output[M];
    riscv_matrix_instance_q15 q15_A;

    riscv_mat_init_q15(&q15_A, M, N, mat_q15_array);

    generate_rand_q15(mat_q15_array, M * N);
    generate_rand_q15(mat_q15_vec, N);

    BENCH_START(riscv_mat_vec_mult_q15);
    riscv_mat_vec_mult_q15(&q15_A, mat_q15_vec, q15_output);
    BENCH_END(riscv_mat_vec_mult_q15);
}