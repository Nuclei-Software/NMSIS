#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

#define CFFT
//#define RFFT
//#define DCT4

int main()
{
    printf("Start TransformFunctions benchmark test:\n");

#ifdef CFFT
#if defined (RISCV_FLOAT16_SUPPORTED)
    cfft_riscv_cfft_f16();
    cfft_riscv_cfft_radix2_f16();
    cfft_riscv_cfft_radix4_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
    cfft_riscv_cfft_f32();
    cfft_riscv_cfft_q15();
    cfft_riscv_cfft_q31();
    cfft_riscv_cfft_radix2_f32();
    cfft_riscv_cfft_radix2_q15();
    cfft_riscv_cfft_radix2_q31();
    cfft_riscv_cfft_radix4_f32();
    cfft_riscv_cfft_radix4_q15();
    cfft_riscv_cfft_radix4_q31();
#endif

#ifdef RFFT
#if defined (RISCV_FLOAT16_SUPPORTED)
    rfft_riscv_rfft_fast_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */ 
    rfft_riscv_rfft_fast_f32();
    rfft_riscv_rfft_f32();
    rfft_riscv_rfft_q15();
    rfft_riscv_rfft_q31();
#endif

#ifdef DCT4
    dct4_riscv_dct4_f32();
    dct4_riscv_dct4_q15();
    dct4_riscv_dct4_q31();
#endif

    printf("test for TransformFunctions benchmark finished.\n");
}
