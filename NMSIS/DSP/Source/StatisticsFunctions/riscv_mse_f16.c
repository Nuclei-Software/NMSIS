/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mse_f16.c
 * Description:  Half floating point mean square error
 *
 * $Date:        05 April 2022
 * $Revision:    V1.10.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/statistics_functions_f16.h"

/**
  @ingroup groupStats
 */

/**
  @addtogroup MSE
  @{
 */

/**
  @brief         Mean square error between two half floating point vectors.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     blockSize   number of samples in input vector
  @param[out]    result      mean square error
 */






#if defined(RISCV_FLOAT16_SUPPORTED)




RISCV_DSP_ATTRIBUTE void riscv_mse_f16(
    const float16_t * pSrcA,
    const float16_t * pSrcB,
    uint32_t    blockSize,
    float16_t * result)

{

  _Float16 inA, inB;
  _Float16 sum = 0.0f16;                          /* Temporary return variable */
#if defined(RISCV_MATH_VECTOR)
    size_t blkCnt = blockSize;
    size_t l;
    const float16_t *pInA = pSrcA;
    const float16_t *pInB = pSrcB;
    vfloat16m8_t v_inA, v_inB, v_subVal;
    vfloat16m8_t vsum;

    l = __riscv_vsetvlmax_e16m8();
    vsum = __riscv_vfmv_v_f_f16m8(0.0f, l);

    for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
        v_inA = __riscv_vle16_v_f16m8(pInA, l);
        pInA += l;
        v_inB = __riscv_vle16_v_f16m8(pInB, l);
        pInB += l;
        v_subVal = __riscv_vfsub_vv_f16m8(v_inA, v_inB, l);
        vsum = __riscv_vfmacc_vv_f16m8(vsum, v_subVal, v_subVal, l);
    }
    l = __riscv_vsetvl_e16m8(1);
    vfloat16m1_t temp00m1 = __riscv_vfmv_v_f_f16m1(0.0f, l);
    l = __riscv_vsetvlmax_e16m8();
    temp00m1 = __riscv_vfredusum_vs_f16m8_f16m1(vsum, temp00m1, l);
    sum += __riscv_vfmv_f_s_f16m1_f16(temp00m1);

#else
#if defined (RISCV_MATH_LOOPUNROLL)

  uint32_t blkCnt;                               /* Loop counter */

  blkCnt = (blockSize) >> 3;


  while (blkCnt > 0U)
  {
    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    /* Decrement loop counter */
    blkCnt--;
  }


  /* Loop unrolling: Compute remaining outputs */
  blkCnt = (blockSize) & 7;
#else
  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;
#endif
  while (blkCnt > 0U)
  {
    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = (_Float16)inA - (_Float16)inB;
    sum += (_Float16)inA * (_Float16)inA;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */

  /* Store result in destination buffer */
  *result = (_Float16)sum / (_Float16)blockSize;
}


#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
/**
  @} end of MSE group
 */
