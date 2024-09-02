#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/offset_q15/test_data.h"

BENCH_DECLARE_VAR();

void offset_riscv_offset_q15(void)
{
    q15_t offset_q15_output[ARRAY_SIZE_Q15];
    q15_t offset_q15;

    generate_rand_q15(offset_q15_input, ARRAY_SIZE_Q15);

    // q15_offset
    generate_rand_q15(&offset_q15, 1);
    BENCH_START(riscv_offset_q15);
    riscv_offset_q15(offset_q15_input, offset_q15, offset_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_offset_q15);
}
