#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_vec_mult_f16/test_data.h"

BENCH_DECLARE_VAR();

void matVecMult_riscv_mat_vec_mult_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output[M];
    riscv_matrix_instance_f16 f16_A;

    riscv_mat_init_f16(&f16_A, M, N, mat_f16_array);

    generate_rand_f16(mat_f16_array, M * N);
    generate_rand_f16(mat_f16_vec, N);

    BENCH_START(riscv_mat_vec_mult_f16);
    riscv_mat_vec_mult_f16(&f16_A, mat_f16_vec, f16_output);
    BENCH_END(riscv_mat_vec_mult_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
