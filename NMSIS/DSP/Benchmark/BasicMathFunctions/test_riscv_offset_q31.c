#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/offset_q31/test_data.h"

BENCH_DECLARE_VAR();

void offset_riscv_offset_q31(void)
{
    q31_t offset_q31_output[ARRAY_SIZE_Q31];
    q31_t offset_q31;

    generate_rand_q31(offset_q31_input, ARRAY_SIZE_Q31);

    // q31_offset
    generate_rand_q31(&offset_q31, 1);
    BENCH_START(riscv_offset_q31);
    riscv_offset_q31(offset_q31_input, offset_q31, offset_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_offset_q31);
}
