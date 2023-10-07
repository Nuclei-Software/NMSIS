/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates. All rights reserved.
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

/* ----------------------------------------------------------------------
 * Project:      NMSIS NN Library
 * Title:        riscv_nn_mult_q7.c
 * Description:  Q7 vector multiplication with variable output shifts
 *
 * $Date:        20. July 2021
 * $Revision:    V.1.1.2
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup NNBasicMath
 * @{
 */

/**
 * @brief           Q7 vector multiplication with variable output shifts
 * @param[in]       *pSrcA        pointer to the first input vector
 * @param[in]       *pSrcB        pointer to the second input vector
 * @param[out]      *pDst         pointer to the output vector
 * @param[in]       out_shift     amount of right-shift for output
 * @param[in]       blockSize     number of samples in each vector
 *
 * <b>Scaling and Overflow Behavior:</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the allowable Q7 range [0x80 0x7F] will be saturated.
 */

void riscv_nn_mult_q7(q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, const uint16_t out_shift, uint32_t blockSize)
{
	uint32_t blkCnt;

#if defined (RISCV_MATH_VECTOR)
    blkCnt = blockSize & (~RVV_OPT_THRESHOLD);                              /* Loop counter */
    size_t l;

    vint8m4_t pA_v8m4, pB_v8m4;
    vint16m8_t tempi16m8;

    for (; (l = __riscv_vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l) {
        pA_v8m4 = __riscv_vle8_v_i8m4(pSrcA, l);
        pB_v8m4 = __riscv_vle8_v_i8m4(pSrcB, l);
        pSrcA += l;
        pSrcB += l;
        tempi16m8 = __riscv_vadd_vx_i16m8(__riscv_vwmul_vv_i16m8(pA_v8m4, pB_v8m4, l), NN_ROUND(out_shift), l);
        __riscv_vse8_v_i8m4(pDst, __riscv_vnclip_wx_i8m4(__riscv_vsra_vx_i16m8(tempi16m8, out_shift, l), 0, __RISCV_VXRM_RNU, l), l);
        pDst += l;

    }
    blkCnt = blockSize & RVV_OPT_THRESHOLD;

#elif defined(RISCV_MATH_DSP)

    /* Run the below code for RISC-V Core with DSP enabled */
    q7_t out1, out2, out3, out4;                   /* Temporary variables to store the product */
    q63_t mul1;
    q31_t inA1, inB1;
    /* loop Unrolling */
    blkCnt = blockSize >> 2U;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
    ** a second loop below computes the remaining 1 to 3 samples. */
    while (blkCnt > 0U)
    {
        /* C = A * B */
        /* Multiply the inputs and store the results in temporary variables */
        // out1 = (q7_t) __SSAT(((q15_t) ((q15_t) (*pSrcA++) * (*pSrcB++) + NN_ROUND(out_shift)) >> out_shift), 8);
        // out2 = (q7_t) __SSAT(((q15_t) ((q15_t) (*pSrcA++) * (*pSrcB++) + NN_ROUND(out_shift)) >> out_shift), 8);
        // out3 = (q7_t) __SSAT(((q15_t) ((q15_t) (*pSrcA++) * (*pSrcB++) + NN_ROUND(out_shift)) >> out_shift), 8);
        // out4 = (q7_t) __SSAT(((q15_t) ((q15_t) (*pSrcA++) * (*pSrcB++) + NN_ROUND(out_shift)) >> out_shift), 8);

        inA1 = riscv_nn_read_q7x4_ia((const q7_t **)&pSrcA);
        inB1 = riscv_nn_read_q7x4_ia((const q7_t **)&pSrcB);

        mul1 = __RV_SMUL8(inA1, inB1);

        out1 = (q7_t) __SSAT((q15_t) ((mul1 & 0xffff) + NN_ROUND(out_shift)) >> out_shift, 8);
        out2 = (q7_t) __SSAT((q15_t) (((mul1 & 0x00000000ffff0000UL) >> 16) + NN_ROUND(out_shift)) >> out_shift, 8);
        out3 = (q7_t) __SSAT((q15_t) (((mul1 & 0x0000ffff00000000UL) >> 32) + NN_ROUND(out_shift)) >> out_shift, 8);
        out4 = (q7_t) __SSAT((q15_t) (((mul1 & 0xffff000000000000UL) >> 48) + NN_ROUND(out_shift)) >> out_shift, 8);

        /* Store the results of 4 inputs in the destination buffer in single cycle by packing */
        *__SIMD32(pDst)++ = PACK_S8x4_32x1(out1, out2, out3, out4);

        /* Decrement the blockSize loop counter */
        blkCnt--;
    }

    /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
    ** No loop unrolling is used. */
    blkCnt = blockSize & 0x3U;

#else
    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;

#endif
    while (blkCnt > 0U)
    {
        /* C = A * B */
        /* Multiply the inputs and store the result in the destination buffer */
        *pDst++ = (q7_t)__SSAT(((q15_t)((q15_t)(*pSrcA++) * (*pSrcB++) + NN_ROUND(out_shift)) >> out_shift), 8);

        /* Decrement the blockSize loop counter */
        blkCnt--;
    }
}

/**
 * @} end of NNBasicMath group
 */
