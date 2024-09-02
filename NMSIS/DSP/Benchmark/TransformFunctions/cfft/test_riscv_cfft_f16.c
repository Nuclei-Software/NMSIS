#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "TestData/TransformFunctions/cfft_f16/test_data.h"

BENCH_DECLARE_VAR();

void cfft_riscv_cfft_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    generate_rand_f16(cfft_testinput_f16_50hz_200Hz, CFFTSIZE * 2);

    BENCH_START(riscv_cfft_f16);
    riscv_cfft_f16(&riscv_cfft_sR_f16_len512, cfft_testinput_f16_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
