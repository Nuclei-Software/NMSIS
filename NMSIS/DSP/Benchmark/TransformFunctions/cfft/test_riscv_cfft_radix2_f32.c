#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "../TestData/TransformFunctions/cfft_radix2_f32/test_data.h"

#if defined(RISCV_MATH_VECTOR)
extern uint16_t bitrevIndexGrp [FFT_DOT];
#endif

static uint32_t reverseBits(uint32_t index,uint8_t totalLayer) {
    uint32_t rev = 0;
    for (int i = 0; i < totalLayer && index > 0; ++i) {
        rev |= (index & 1) << (totalLayer - 1 - i);
        index >>= 1;
    }
    return rev;
}

static void init_bitrev(int fftSize)
{
#if defined(RISCV_MATH_VECTOR)
	for(uint32_t i = 0;i < fftSize;i++)
	{
        //bit reverse index
        bitrevIndexGrp[i] = reverseBits(i, (int)log2((double)fftSize));
        #ifdef DEBUG_PRINT
        printf("rev[%d]:%d\r\n",i,bitrevIndexGrp[i]);
        #endif
        //index for index load
        bitrevIndexGrp[i] = 4 * 2 * bitrevIndexGrp[i];
	}
#endif
}


BENCH_DECLARE_VAR();

void cfft_riscv_cfft_radix2_f32(void)
{
    generate_rand_f32(cfft_testinput_f32_50hz_200Hz, CFFTSIZE * 2);

    riscv_cfft_radix2_instance_f32 f32_S;
    riscv_status result = riscv_cfft_radix2_init_f32(&f32_S, CFFTSIZE, ifftFlag, doBitReverse);
    init_bitrev(CFFTSIZE); /* generate bit reverse index group */

    BENCH_START(riscv_cfft_radix2_f32);
    riscv_cfft_radix2_f32(&f32_S, cfft_testinput_f32_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
