#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_inverse_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void matInverse_riscv_mat_inverse_f16(void)
{
    float16_t f16_output[M * M];
    riscv_matrix_instance_f16 f16_A;
    riscv_matrix_instance_f16 f16_des;
    riscv_mat_init_f16(&f16_A, M, M, mat_inverse_f16_input);
    riscv_mat_init_f16(&f16_des, M, M, f16_output);

    generate_rand_f16(mat_inverse_f16_input, M * M);

    BENCH_START(riscv_mat_inverse_f16);
    riscv_status result = riscv_mat_inverse_f16(&f16_A, &f16_des);
    BENCH_END(riscv_mat_inverse_f16);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */