#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/f16_to_q15/test_data.h"

BENCH_DECLARE_VAR();

void f16ToQ15_riscv_f16_to_q15(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    q15_t q15_output[ARRAY_SIZE];

    generate_rand_f16(f16_input, ARRAY_SIZE);

    BENCH_START(riscv_f16_to_q15);
    riscv_f16_to_q15(f16_input, q15_output, ARRAY_SIZE);
    BENCH_END(riscv_f16_to_q15);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}

