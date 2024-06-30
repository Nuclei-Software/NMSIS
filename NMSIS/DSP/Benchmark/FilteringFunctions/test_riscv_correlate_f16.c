#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ConvolutionFunctions/correlate_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void correlate_riscv_correlate_f16(void)
{
    float16_t correlate_f16_output[2 * max(ARRAYA_SIZE_F16, ARRAYB_SIZE_F16)];

    generate_rand_f16(correlate_input_f16_A, ARRAYA_SIZE_F16);
    generate_rand_f16(correlate_input_f16_B, ARRAYB_SIZE_F16);

    BENCH_START(riscv_correlate_f16);
    riscv_correlate_f16(correlate_input_f16_A, ARRAYA_SIZE_F16, correlate_input_f16_B, ARRAYB_SIZE_F16,
                      correlate_f16_output);
    BENCH_END(riscv_correlate_f16);

    return;
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
