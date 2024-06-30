#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/f16_to_f64/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void f16ToF64_riscv_f16_to_f64(void)
{
    float64_t f64_output[ARRAY_SIZE];

    generate_rand_f16(f16_input, ARRAY_SIZE);

    BENCH_START(riscv_f16_to_f64.);
    riscv_f16_to_f64(f16_input, f64_output, ARRAY_SIZE);
    BENCH_END(riscv_f16_to_f64.);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */