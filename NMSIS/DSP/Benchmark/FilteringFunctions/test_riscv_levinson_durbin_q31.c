#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/levinson_durbin_q31/test_data.h"

BENCH_DECLARE_VAR();

void levinsonDurbin_riscv_levinson_durbin_q31(void)
{
    q31_t err;
    generate_rand_q31(phi, COEFSIZE);

    BENCH_START(riscv_levinson_durbin_q31);
    riscv_levinson_durbin_q31(phi, autoRegreCoef, &err, COEFSIZE);
    BENCH_END(riscv_levinson_durbin_q31);

    return;
}
