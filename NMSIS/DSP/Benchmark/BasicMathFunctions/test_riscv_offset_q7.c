#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/offset_q7/test_data.h"

BENCH_DECLARE_VAR();

void offset_riscv_offset_q7(void)
{
    q7_t offset_q7_output[ARRAY_SIZE_Q7];
    q7_t offset_q7;
    generate_rand_q7(offset_q7_input, ARRAY_SIZE_Q7);

    // q7_offset
    generate_rand_q7(&offset_q7, 1);
    BENCH_START(riscv_offset_q7);
    riscv_offset_q7(offset_q7_input, offset_q7, offset_q7_output, ARRAY_SIZE_Q7);
    BENCH_END(riscv_offset_q7);
}
