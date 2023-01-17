/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_cmplx_mult_q31.c
 * Description:  Floating-point matrix multiplication
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

#include "dsp/matrix_functions.h"

/**
  @ingroup groupMatrix
 */

/**
  @addtogroup CmplxMatrixMult
  @{
 */

/**
  @brief         Q31 Complex matrix multiplication.
  @param[in]     pSrcA      points to first input complex matrix structure
  @param[in]     pSrcB      points to second input complex matrix structure
  @param[out]    pDst       points to output complex matrix structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate
                   multiplication results but provides only a single guard bit. There is no saturation
                   on intermediate additions. Thus, if the accumulator overflows it wraps around and
                   distorts the result. The input signals should be scaled down to avoid intermediate
                   overflows. The input is thus scaled down by log2(numColsA) bits
                   to avoid overflows, as a total of numColsA additions are performed internally.
                   The 2.62 accumulator is right shifted by 31 bits and saturated to 1.31 format to yield the final result.
 */
riscv_status riscv_mat_cmplx_mult_q31(
  const riscv_matrix_instance_q31 * pSrcA,
  const riscv_matrix_instance_q31 * pSrcB,
        riscv_matrix_instance_q31 * pDst)
{
  q31_t *pIn1 = pSrcA->pData;                    /* Input data matrix pointer A */
  q31_t *pIn2 = pSrcB->pData;                    /* Input data matrix pointer B */
  q31_t *pInA = pSrcA->pData;                    /* Input data matrix pointer A */
  q31_t *pInB = pSrcB->pData;                    /* Input data matrix pointer B */
  q31_t *pOut = pDst->pData;                     /* Output data matrix pointer */
  q31_t *px;                                     /* Temporary output data matrix pointer */
  uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
  q63_t sumReal, sumImag;                        /* Accumulator */
  q31_t a1, b1, c1, d1;
  uint32_t col, i = 0U, j, row = numRowsA, colCnt; /* loop counters */
  riscv_status status;                             /* status of matrix multiplication */

#if defined (RISCV_MATH_LOOPUNROLL)
  q31_t a0, b0, c0, d0;
#endif

#ifdef RISCV_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrcA->numCols != pSrcB->numRows) ||
      (pSrcA->numRows != pDst->numRows)  ||
      (pSrcB->numCols != pDst->numCols)    )
  {
    /* Set status as RISCV_MATH_SIZE_MISMATCH */
    status = RISCV_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef RISCV_MATH_MATRIX_CHECK */
  {
#if defined(RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    uint32_t ii, jj, kk;
    size_t l;
    vint32m4_t va0m4, va1m4, v_inAR, v_inAI;
    vint64m8_t v_inAR2, v_inAI2, vres0m8, vres1m8;
    ptrdiff_t bstride = 8;       //  32bit / 8bit * 2 = 8
    colCnt = numColsB;

    for (jj = colCnt; jj > 0; jj--) {
      px = pOut;
      pInA = pIn1;
      for (ii = numRowsA; ii > 0; ii -= l) {
        l = vsetvl_e64m8(ii);
        pInB = pIn2;
        vres0m8 = vmv_v_x_i64m8(0, l);
        vres1m8 = vmv_v_v_i64m8(vres0m8, l);
        for (kk = 0; kk < numColsA; kk++) {
          vlsseg2e32_v_i32m4(&v_inAR, &v_inAI, pInA + 2 * kk, numColsA * bstride, l);
          v_inAR2 = vsub_vv_i64m8(vwmul_vx_i64m8(v_inAR, *(pInB + 0), l), vwmul_vx_i64m8(v_inAI, *(pInB + 1), l), l);
          v_inAI2 = vadd_vv_i64m8(vwmul_vx_i64m8(v_inAR, *(pInB + 1), l), vwmul_vx_i64m8(v_inAI, *(pInB + 0), l), l);
          vres0m8 = vmacc_vx_i64m8 (vres0m8, 1, v_inAR2, l);
          vres1m8 = vmacc_vx_i64m8 (vres1m8, 1, v_inAI2, l);
          pInB += numColsB * 2;
        }
        va0m4 = vnclip_wx_i32m4(vres0m8, 31, l);
        va1m4 = vnclip_wx_i32m4(vres1m8, 31, l);
        vssseg2e32_v_i32m4(px, numColsB * bstride, va0m4, va1m4, l);
        px += l * numColsB * 2;
        pInA += l * numColsA * 2;
      }
      pIn2 += 2;
      pOut += 2;
    }
  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;
#else
    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of the row being processed */
      px = pOut + 2 * i;

      /* For every row wise process, the column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, the pIn2 pointer is set
       ** to the starting address of the pSrcB data */
      pIn2 = pSrcB->pData;

      j = 0U;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sumReal = 0.0;
        sumImag = 0.0;

        /* Initiate pointer pIn1 to point to starting address of column being processed */
        pIn1 = pInA;

#if defined (RISCV_MATH_LOOPUNROLL)

        /* Apply loop unrolling and compute 4 MACs simultaneously. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
          q63_t in164, in264;
          in164 = read_q31x2_ia ((q31_t **) &pIn1);
          in264 = read_q31x2_ia ((q31_t **) &pIn2);
          pIn2 -= 2;
          pIn2 += 2 * numColsB;
          sumReal =  __RV_KMABB32(sumReal, in164, in264);
          sumImag =  __RV_KMABT32(sumImag, in264, in164);
          sumReal -= __RV_SMTT32(in164, in264);
          sumImag =  __RV_KMABT32(sumImag, in164, in264);

          in164 = read_q31x2_ia ((q31_t **) &pIn1);
          in264 = read_q31x2_ia ((q31_t **) &pIn2);
          pIn2 -= 2;
          pIn2 += 2 * numColsB;
          sumReal =  __RV_KMABB32(sumReal, in164, in264);
          sumImag =  __RV_KMABT32(sumImag, in264, in164);
          sumReal -= __RV_SMTT32(in164, in264);
          sumImag =  __RV_KMABT32(sumImag, in164, in264);

          in164 = read_q31x2_ia ((q31_t **) &pIn1);
          in264 = read_q31x2_ia ((q31_t **) &pIn2);
          pIn2 -= 2;
          pIn2 += 2 * numColsB;
          sumReal =  __RV_KMABB32(sumReal, in164, in264);
          sumImag =  __RV_KMABT32(sumImag, in264, in164);
          sumReal -= __RV_SMTT32(in164, in264);
          sumImag =  __RV_KMABT32(sumImag, in164, in264);

          in164 = read_q31x2_ia ((q31_t **) &pIn1);
          in264 = read_q31x2_ia ((q31_t **) &pIn2);
          pIn2 -= 2;
          pIn2 += 2 * numColsB;
          sumReal =  __RV_KMABB32(sumReal, in164, in264);
          sumImag =  __RV_KMABT32(sumImag, in264, in164);
          sumReal -= __RV_SMTT32(in164, in264);
          sumImag =  __RV_KMABT32(sumImag, in164, in264);
#else

          /* Reading real part of complex matrix A */
          a0 = *pIn1;

          /* Reading real part of complex matrix B */
          c0 = *pIn2;

          /* Reading imaginary part of complex matrix A */
          b0 = *(pIn1 + 1U);

          /* Reading imaginary part of complex matrix B */
          d0 = *(pIn2 + 1U);

          /* Multiply and Accumlates */
          sumReal += (q63_t) a0 * c0;
          sumImag += (q63_t) b0 * c0;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumlates */
          sumReal -= (q63_t) b0 * d0;
          sumImag += (q63_t) a0 * d0;

          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* read real and imag values from pSrcA and pSrcB buffer */
          a1 = *(pIn1     );
          c1 = *(pIn2     );
          b1 = *(pIn1 + 1U);
          d1 = *(pIn2 + 1U);

          /* Multiply and Accumlates */
          sumReal += (q63_t) a1 * c1;
          sumImag += (q63_t) b1 * c1;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumlates */
          sumReal -= (q63_t) b1 * d1;
          sumImag += (q63_t) a1 * d1;

          a0 = *(pIn1     );
          c0 = *(pIn2     );
          b0 = *(pIn1 + 1U);
          d0 = *(pIn2 + 1U);

          /* Multiply and Accumlates */
          sumReal += (q63_t) a0 * c0;
          sumImag += (q63_t) b0 * c0;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumlates */
          sumReal -= (q63_t) b0 * d0;
          sumImag += (q63_t) a0 * d0;

          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          a1 = *(pIn1     );
          c1 = *(pIn2     );
          b1 = *(pIn1 + 1U);
          d1 = *(pIn2 + 1U);

          /* Multiply and Accumlates */
          sumReal += (q63_t) a1 * c1;
          sumImag += (q63_t) b1 * c1;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumlates */
          sumReal -= (q63_t) b1 * d1;
          sumImag += (q63_t) a1 * d1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
          /* Decrement loop count */
          colCnt--;
        }

        /* If the columns of pSrcA is not a multiple of 4, compute any remaining MACs here.
         ** No loop unrolling is used. */
        colCnt = numColsA & 0x3U;

#else

        /* Initialize blkCnt with number of samples */
        colCnt = numColsA;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */
          a1 = *(pIn1     );
          c1 = *(pIn2     );
          b1 = *(pIn1 + 1U);
          d1 = *(pIn2 + 1U);

          /* Multiply and Accumlates */
          sumReal += (q63_t) a1 * c1;
          sumImag += (q63_t) b1 * c1;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumlates */
          sumReal -= (q63_t) b1 * d1;
          sumImag += (q63_t) a1 * d1;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Store result in destination buffer */
        *px++ = (q31_t) clip_q63_to_q31(sumReal >> 31);
        *px++ = (q31_t) clip_q63_to_q31(sumImag >> 31);

        /* Update pointer pIn2 to point to starting address of next column */
        j++;
        pIn2 = pSrcB->pData + 2U * j;

        /* Decrement column loop counter */
        col--;

      } while (col > 0U);

      /* Update pointer pInA to point to starting address of next row */
      i = i + numColsB;
      pInA = pInA + 2 * numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
#endif /* defined(RISCV_MATH_VECTOR) */
  }
  /* Return to application */
  return (status);
}

/**
  @} end of MatrixMult group
 */
