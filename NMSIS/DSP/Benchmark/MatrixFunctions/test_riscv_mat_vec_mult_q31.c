#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_vec_mult_q31/test_data.h"

BENCH_DECLARE_VAR();

void matVecMult_riscv_mat_vec_mult_q31(void)
{
    q31_t q31_output[M];
    riscv_matrix_instance_q31 q31_A;

    riscv_mat_init_q31(&q31_A, M, N, mat_q31_array);

    generate_rand_q31(mat_q31_array, M * N);
    generate_rand_q31(mat_q31_vec, N);

    BENCH_START(riscv_mat_vec_mult_q31);
    riscv_mat_vec_mult_q31(&q31_A, mat_q31_vec, q31_output);
    BENCH_END(riscv_mat_vec_mult_q31);
}