#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "TestData/TransformFunctions/cfft_q31/test_data.h"

BENCH_DECLARE_VAR();

void cfft_riscv_cfft_q31(void)
{
    generate_rand_q31(cfft_testinput_q31_50hz_200Hz, CFFTSIZE * 2);

    BENCH_START(riscv_cfft_q31);
    riscv_cfft_q31(&riscv_cfft_sR_q31_len512, cfft_testinput_q31_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_q31);
}