#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/f64_to_float/test_data.h"

BENCH_DECLARE_VAR();

void f64ToF32_riscv_f64_to_float(void)
{
#ifdef F64
    float32_t f32_output[ARRAY_SIZE];

    generate_rand_f64(f64_input, ARRAY_SIZE);

    BENCH_START(riscv_f64_to_float);
    riscv_f64_to_float(f64_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_f64_to_float);
#endif
}