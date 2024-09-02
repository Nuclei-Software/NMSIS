#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/copy_q31/test_data.h"

BENCH_DECLARE_VAR();

void copy_riscv_copy_q31(void)
{
    q31_t q31_output[ARRAY_SIZE];

    generate_rand_q31(copy_q31_input, ARRAY_SIZE);

    BENCH_START(riscv_copy_q31);
    riscv_copy_q31(copy_q31_input, q31_output, ARRAY_SIZE);
    BENCH_END(riscv_copy_q31);

}