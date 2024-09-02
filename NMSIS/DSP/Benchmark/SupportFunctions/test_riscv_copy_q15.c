#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/copy_q15/test_data.h"

BENCH_DECLARE_VAR();

void copy_riscv_copy_q15(void)
{
    q15_t q15_output[ARRAY_SIZE];

    generate_rand_q15(copy_q15_input, ARRAY_SIZE);

    BENCH_START(riscv_copy_q15);
    riscv_copy_q15(copy_q15_input, q15_output, ARRAY_SIZE);
    BENCH_END(riscv_copy_q15);
}