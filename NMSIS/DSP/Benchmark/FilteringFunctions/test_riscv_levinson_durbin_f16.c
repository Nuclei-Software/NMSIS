#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/levinson_durbin_f16/test_data.h"

BENCH_DECLARE_VAR();

void levinsonDurbin_riscv_levinson_durbin_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t err;
    generate_rand_f16(phi, COEFSIZE);

    BENCH_START(riscv_levinson_durbin_f16);
    riscv_levinson_durbin_f16(phi, autoRegreCoef, &err, COEFSIZE);
    BENCH_END(riscv_levinson_durbin_f16);

#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
}
