#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/copy_q7/test_data.h"

BENCH_DECLARE_VAR();

void copy_riscv_copy_q7(void)
{
    q7_t q7_output[ARRAY_SIZE];

    generate_rand_q7(copy_q7_input, ARRAY_SIZE);

    BENCH_START(riscv_copy_q7);
    riscv_copy_q7(copy_q7_input, q7_output, ARRAY_SIZE);
    BENCH_END(riscv_copy_q7);

}