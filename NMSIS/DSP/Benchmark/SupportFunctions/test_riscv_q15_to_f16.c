#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/q15_to_f16/test_data.h"

BENCH_DECLARE_VAR();

#if defined (RISCV_FLOAT16_SUPPORTED)

void q15ToF16_riscv_q15_to_f16(void)
{
    float16_t f16_output[ARRAY_SIZE];

    generate_rand_q15(q15_input, ARRAY_SIZE);

    BENCH_START(riscv_q15_to_f16);
    riscv_q15_to_f16(q15_input, f16_output, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */