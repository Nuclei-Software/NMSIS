#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/levinson_durbin_f32/test_data.h"

BENCH_DECLARE_VAR();

void levinsonDurbin_riscv_levinson_durbin_f32(void)
{
    float32_t err;
    generate_rand_f32(phi, COEFSIZE + 1);

    BENCH_START(riscv_levinson_durbin_f32);
    riscv_levinson_durbin_f32(phi, autoRegreCoef, &err, COEFSIZE);
    BENCH_END(riscv_levinson_durbin_f32);

    return;
}
