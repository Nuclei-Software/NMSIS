#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_trans_f16/test_data.h"

BENCH_DECLARE_VAR();

void matTrans_riscv_mat_trans_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output[N * M];
    riscv_matrix_instance_f16 f16_A;
    riscv_matrix_instance_f16 f16_des;
    riscv_mat_init_f16(&f16_A, M, N, mat_trans_f16_array);
    riscv_mat_init_f16(&f16_des, N, M, f16_output);

    generate_rand_f16(mat_trans_f16_array, M * N);

    BENCH_START(riscv_mat_trans_f16);
    riscv_status result = riscv_mat_trans_f16(&f16_A, &f16_des);
    BENCH_END(riscv_mat_trans_f16);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}

