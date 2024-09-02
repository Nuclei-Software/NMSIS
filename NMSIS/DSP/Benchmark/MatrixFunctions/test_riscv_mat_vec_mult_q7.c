#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_vec_mult_q7/test_data.h"

BENCH_DECLARE_VAR();

void matVecMult_riscv_mat_vec_mult_q7(void)
{
    q7_t q7_output[M];
    riscv_matrix_instance_q7 q7_A;

    riscv_mat_init_q7(&q7_A, M, N, mat_q7_array);

    generate_rand_q7(mat_q7_array, M * N);
    generate_rand_q7(mat_q7_vec, N);

    BENCH_START(riscv_mat_vec_mult_q7);
    riscv_mat_vec_mult_q7(&q7_A, mat_q7_vec, q7_output);
    BENCH_END(riscv_mat_vec_mult_q7);

}