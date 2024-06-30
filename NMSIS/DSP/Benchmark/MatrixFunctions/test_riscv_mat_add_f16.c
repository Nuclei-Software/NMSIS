#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_add_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void matAdd_riscv_mat_add_f16(void)
{
    riscv_matrix_instance_f16 f16_A;
    riscv_matrix_instance_f16 f16_B;
    riscv_matrix_instance_f16 f16_des;
    float16_t mat_add_f16_output[M * N];

    riscv_mat_init_f16(&f16_A, M, N, mat_add_f16_arrayA);
    riscv_mat_init_f16(&f16_B, M, N, mat_add_f16_arrayB);
    riscv_mat_init_f16(&f16_des, M, N, mat_add_f16_output);

    generate_rand_f16(mat_add_f16_arrayA, M * N);
    generate_rand_f16(mat_add_f16_arrayB, M * N);

    BENCH_START(riscv_mat_add_f16);
    riscv_status result = riscv_mat_add_f16(&f16_A, &f16_B, &f16_des);
    BENCH_END(riscv_mat_add_f16);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
