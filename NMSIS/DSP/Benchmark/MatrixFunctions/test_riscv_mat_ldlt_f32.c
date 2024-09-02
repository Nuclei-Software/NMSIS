#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_ldlt_f32/test_data.h"

BENCH_DECLARE_VAR();

void matLdlt_riscv_mat_ldlt_f32(void)
{
    uint16_t pp[M];
    float32_t f32_output[M * M];

    riscv_matrix_instance_f32 f32_B;
    riscv_matrix_instance_f32 f32_posi;
    riscv_matrix_instance_f32 f32_des;

    riscv_mat_init_f32(&f32_B, M, M, f32_e_array);
    riscv_mat_init_f32(&f32_posi, M, M, mat_ldlt_f32_input);

    for(int i = 0; i < M; i++){
        for(int j = M - 1; j > i; j--){
            if (i == j) break;
            f32_e_array[i * M + j] = f32_e_array[j * M + i];
        }
    }
    riscv_mat_init_f32(&f32_des, M, M, (float32_t *)f32_output);
    BENCH_START(riscv_mat_ldlt_f32);
    riscv_status result = riscv_mat_ldlt_f32(&f32_posi, &f32_B, &f32_des, pp);
    BENCH_END(riscv_mat_ldlt_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}