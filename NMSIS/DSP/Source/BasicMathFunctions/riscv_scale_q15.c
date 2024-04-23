/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_scale_q15.c
 * Description:  Multiplies a Q15 vector by a scalar
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */

/**
  @addtogroup BasicScale
  @{
 */

/**
  @brief         Multiplies a Q15 vector by a scalar.
  @param[in]     pSrc       points to the input vector
  @param[in]     scaleFract fractional portion of the scale value
  @param[in]     shift      number of bits to shift the result by
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The input data <code>*pSrc</code> and <code>scaleFract</code> are in 1.15 format.
                   These are multiplied to yield a 2.30 intermediate result and this is shifted with saturation to 1.15 format.
 */

void riscv_scale_q15(
  const q15_t *pSrc,
        q15_t scaleFract,
        int8_t shift,
        q15_t *pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */
  int8_t kShift = 15 - shift;                    /* Shift to apply after scaling */

#if defined (RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vint16m4_t vx;

  for (; (l = __riscv_vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l) {
    vx = __riscv_vle16_v_i16m4(pSrc, l);
    pSrc += l;
    __riscv_vse16_v_i16m4(pDst, __riscv_vnclip_wx_i16m4(__riscv_vwmul_vx_i32m8(vx, scaleFract, l), kShift, __RISCV_VXRM_RNU, l), l);
    pDst += l;
  }
#else

#if defined (RISCV_MATH_LOOPUNROLL)
#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
  q31_t out32, tmpScale32;
  q63_t out64;
  tmpScale32 = (q31_t)scaleFract | ((q31_t)scaleFract << 16);
#endif /* __RISCV_XLEN == 64 */
  q31_t inA1, inA2;
  q31_t out1, out2, out3, out4;                  /* Temporary output variables */
  q15_t in1, in2, in3, in4;                      /* Temporary input variables */
#endif /* defined (RISCV_MATH_DSP) */
#endif /* defined (RISCV_MATH_LOOPUNROLL) */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = A * scale */

#if defined (RISCV_MATH_DSP)

#if __RISCV_XLEN == 64
    out32 = read_q15x2_ia((q15_t **)&pSrc);
    out64 = __RV_SMUL16(out32, tmpScale32);
    out64 = __RV_SRA32(out64, kShift);
    out1 = (q31_t)(out64 >> 32);
    out2 = (q31_t)out64;

    out32 = read_q15x2_ia((q15_t **)&pSrc);
    out64 = __RV_SMUL16(out32, tmpScale32);
    out64 = __RV_SRA32(out64, kShift);
    out3 = (q31_t)(out64 >> 32);
    out4 = (q31_t)out64;
#else
    /* read 2 times 2 samples at a time from source */
    inA1 = read_q15x2_ia ((q15_t **) &pSrc);
    inA2 = read_q15x2_ia ((q15_t **) &pSrc);

    /* Scale inputs and store result in temporary variables
     * in single cycle by packing the outputs */
    out1 = (q31_t)((q15_t) (inA1 >> 16) * scaleFract);
    out2 = (q31_t)((q15_t) (inA1      ) * scaleFract);
    out3 = (q31_t)((q15_t) (inA2 >> 16) * scaleFract);
    out4 = (q31_t)((q15_t) (inA2      ) * scaleFract);

    /* apply shifting */
    out1 = out1 >> kShift;
    out2 = out2 >> kShift;
    out3 = out3 >> kShift;
    out4 = out4 >> kShift;
#endif /* __RISCV_XLEN == 64 */

    /* saturate the output */
    in1 = (q15_t) (__SSAT(out1, 16));
    in2 = (q15_t) (__SSAT(out2, 16));
    in3 = (q15_t) (__SSAT(out3, 16));
    in4 = (q15_t) (__SSAT(out4, 16));

    /* store result to destination */
    write_q15x2_ia(&pDst, __RV_PKBB16(in1, in2));
    write_q15x2_ia(&pDst, __RV_PKBB16(in3, in4));
#else
    *pDst++ = (q15_t)(__SSAT(((q31_t)*pSrc++ * scaleFract) >> kShift, 16));
    *pDst++ = (q15_t)(__SSAT(((q31_t)*pSrc++ * scaleFract) >> kShift, 16));
    *pDst++ = (q15_t)(__SSAT(((q31_t)*pSrc++ * scaleFract) >> kShift, 16));
    *pDst++ = (q15_t)(__SSAT(((q31_t)*pSrc++ * scaleFract) >> kShift, 16));
#endif /* defined (RISCV_MATH_DSP) */


    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize & 0x3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = A * scale */

    /* Scale input and store result in destination buffer. */
    *pDst++ = (q15_t) (__SSAT(((q31_t) *pSrc++ * scaleFract) >> kShift, 16));

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of BasicScale group
 */
