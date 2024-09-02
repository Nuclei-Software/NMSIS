#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/f64_to_f16/test_data.h"

BENCH_DECLARE_VAR();

void f64ToF16_riscv_f64_to_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED) && defined (F64)
    float16_t f16_output[ARRAY_SIZE];

    generate_rand_f64(f64_input, ARRAY_SIZE);

    BENCH_START(riscv_f64_to_f16);
    riscv_f64_to_f16(f64_input, f16_output, ARRAY_SIZE);
    BENCH_END(riscv_f64_to_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
