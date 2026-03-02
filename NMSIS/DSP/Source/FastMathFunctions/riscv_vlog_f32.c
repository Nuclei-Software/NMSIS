/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_vlog_f32.c
 * Description:  Fast vectorized log
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


#include "dsp/fast_math_functions.h"
#include "riscv_common_tables.h"

#if defined(RISCV_MATH_VECTOR)
#include "riscv_vec_math.h"
#endif /* defined(RISCV_MATH_VECTOR) */

/**
  @ingroup groupFastMath
 */


/**
  @defgroup vlog Vector Log

  Compute the log values of a vector of samples.

 */

/**
  @addtogroup vlog
  @{
 */


RISCV_DSP_ATTRIBUTE void riscv_vlog_f32(
  const float32_t * pSrc,
        float32_t * pDst,
        uint32_t blockSize)
{
   uint32_t blkCnt;

#if defined(RISCV_MATH_VECTOR)
   size_t l;
   blkCnt = blockSize;
   vfloat32m8_t vx, vy;
   for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
     vx = __riscv_vle32_v_f32m8(pSrc, l);
     pSrc += l;
     vy = log_ps_m8(vx, l);
     __riscv_vse32_v_f32m8(pDst, vy, l);
     pDst += l;
   }
#else
   blkCnt = blockSize;

   while (blkCnt > 0U)
   {
      /* C = log(A) */
  
      /* Calculate log and store result in destination buffer. */
      *pDst++ = logf(*pSrc++);
  
      /* Decrement loop counter */
      blkCnt--;
   }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of vlog group
 */
