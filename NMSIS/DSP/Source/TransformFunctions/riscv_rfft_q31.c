/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_rfft_q31.c
 * Description:  FFT & RIFFT Q31 process function
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

#include "dsp/transform_functions.h"

/* ----------------------------------------------------------------------
 * Internal functions prototypes
 * -------------------------------------------------------------------- */

RISCV_DSP_ATTRIBUTE void riscv_split_rfft_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier);

RISCV_DSP_ATTRIBUTE void riscv_split_rifft_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier);

/**
  @addtogroup RealFFTQ31
  @{
 */

/**
  @brief         Processing function for the Q31 RFFT/RIFFT.
  @param[in]     S     points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     pSrc  points to input buffer (Source buffer is modified by this function)
  @param[out]    pDst  points to output buffer

  @par           Input an output formats
                   Internally input is downscaled by 2 for every stage to avoid saturations inside CFFT/CIFFT process.
                   Hence the output format is different for different RFFT sizes.
                   The input and output formats for different RFFT sizes and number of bits to upscale are mentioned in the tables below for RFFT and RIFFT:
  @par             Input and Output formats for RFFT Q31

| RFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ---------: | ------------: | -------------: | ------------------------: |
| 32         | 1.31          | 6.26           | 5                         |
| 64         | 1.31          | 7.25           | 6                         |
| 128        | 1.31          | 8.24           | 7                         |
| 256        | 1.31          | 9.23           | 8                         |
| 512        | 1.31          | 10.22          | 9                         |
| 1024       | 1.31          | 11.21          | 10                        |
| 2048       | 1.31          | 12.20          | 11                        |
| 4096       | 1.31          | 13.19          | 12                        |
| 8192       | 1.31          | 14.18          | 13                        |

  @par             Input and Output formats for RIFFT Q31

| RIFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ----------: | ------------: | -------------: | ------------------------: |
| 32          | 1.31          | 6.26           | 0                         |
| 64          | 1.31          | 7.25           | 0                         |
| 128         | 1.31          | 8.24           | 0                         |
| 256         | 1.31          | 9.23           | 0                         |
| 512         | 1.31          | 10.22          | 0                         |
| 1024        | 1.31          | 11.21          | 0                         |
| 2048        | 1.31          | 12.20          | 0                         |
| 4096        | 1.31          | 13.19          | 0                         |
| 8192        | 1.31          | 14.18          | 0                         |

  @par
                   If the input buffer is of length N (fftLenReal), the output buffer must have length 2N
                   since it is containing the conjugate part.
                   The input buffer is modified by this function.
  @par
                   For the RIFFT, the source buffer must have length N+2 since the Nyquist frequency value
                   is needed but conjugate part is ignored.
                   It is not using the packing trick of the float version.

 */

RISCV_DSP_ATTRIBUTE void riscv_rfft_q31(
  const riscv_rfft_instance_q31 * S,
        q31_t * pSrc,
        q31_t * pDst)
{
  const riscv_cfft_instance_q31 *S_CFFT = S->pCfft;
        uint32_t L2 = S->fftLenReal >> 1U;

  /* Calculation of RIFFT of input */
  if (S->ifftFlagR == 1U)
  {
     /*  Real IFFT core process */
     riscv_split_rifft_q31 (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);

     /* Complex IFFT process */
     riscv_cfft_q31 (S_CFFT, pDst, S->ifftFlagR, S->bitReverseFlagR);

     riscv_shift_q31(pDst, 1, pDst, S->fftLenReal);
  }
  else
  {
     /* Calculation of RFFT of input */

     /* Complex FFT process */
     riscv_cfft_q31 (S_CFFT, pSrc, S->ifftFlagR, S->bitReverseFlagR);

     /*  Real FFT core process */
     riscv_split_rfft_q31 (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);
  }

}

/**
  @} end of RealFFTQ31 group
 */

/**
  @brief         Core Real FFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
 */

RISCV_DSP_ATTRIBUTE void riscv_split_rfft_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier)
{
        uint32_t i;                                    /* Loop Counter */
        q31_t outR, outI;                              /* Temporary variables for output */
  const q31_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
        q31_t CoefA1, CoefA2, CoefB1;                  /* Temporary variables for twiddle coefficients */
        q31_t *pOut1 = &pDst[2], *pOut2 = &pDst[4 * fftLen - 1];
        q31_t *pIn1 =  &pSrc[2], *pIn2 =  &pSrc[2 * fftLen - 1];

  /* Init coefficient pointers */
  pCoefA = &pATable[modifier * 2];
  pCoefB = &pBTable[modifier * 2];

  i = fftLen - 1U;

  while (i > 0U)
  {
     /*
       outR = (  pSrc[2 * i]             * pATable[2 * i]
               - pSrc[2 * i + 1]         * pATable[2 * i + 1]
               + pSrc[2 * n - 2 * i]     * pBTable[2 * i]
               + pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);

       outI = (  pIn[2 * i + 1]         * pATable[2 * i]
               + pIn[2 * i]             * pATable[2 * i + 1]
               + pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
               - pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
      */

     CoefA1 = *pCoefA++;
     CoefA2 = *pCoefA;

     /* outR = (pSrc[2 * i] * pATable[2 * i] */
     mult_32x32_keep32_R (outR, *pIn1, CoefA1);

     /* outI = pIn[2 * i] * pATable[2 * i + 1] */
     mult_32x32_keep32_R (outI, *pIn1++, CoefA2);

     /* - pSrc[2 * i + 1] * pATable[2 * i + 1] */
     multSub_32x32_keep32_R (outR, *pIn1, CoefA2);

     /* (pIn[2 * i + 1] * pATable[2 * i] */
     multAcc_32x32_keep32_R (outI, *pIn1++, CoefA1);

     /* pSrc[2 * n - 2 * i] * pBTable[2 * i]  */
     multSub_32x32_keep32_R (outR, *pIn2, CoefA2);
     CoefB1 = *pCoefB;

     /* pIn[2 * n - 2 * i] * pBTable[2 * i + 1] */
     multSub_32x32_keep32_R (outI, *pIn2--, CoefB1);

     /* pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1] */
     multAcc_32x32_keep32_R (outR, *pIn2, CoefB1);

     /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
     multSub_32x32_keep32_R (outI, *pIn2--, CoefA2);

     /* write output */
     *pOut1++ = outR;
     *pOut1++ = outI;

     /* write complex conjugate output */
     *pOut2-- = -outI;
     *pOut2-- = outR;

     /* update coefficient pointer */
     pCoefB = pCoefB + (2 * modifier);
     pCoefA = pCoefA + (2 * modifier - 1);

     /* Decrement loop count */
     i--;
  }

  pDst[2 * fftLen]     = (pSrc[0] - pSrc[1]) >> 1U;
  pDst[2 * fftLen + 1] = 0;

  pDst[0] = (pSrc[0] + pSrc[1]) >> 1U;
  pDst[1] = 0;
}

/**
  @brief         Core Real IFFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
 */

RISCV_DSP_ATTRIBUTE void riscv_split_rifft_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier)
{       
        q31_t outR, outI;                              /* Temporary variables for output */
  const q31_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
        q31_t CoefA1, CoefA2, CoefB1;                  /* Temporary variables for twiddle coefficients */
        q31_t *pIn1 = &pSrc[0], *pIn2 = &pSrc[2 * fftLen + 1];

  pCoefA = &pATable[0];
  pCoefB = &pBTable[0];

  while (fftLen > 0U)
  {
     /*
       outR = (  pIn[2 * i]             * pATable[2 * i]
               + pIn[2 * i + 1]         * pATable[2 * i + 1]
               + pIn[2 * n - 2 * i]     * pBTable[2 * i]
               - pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);

       outI = (  pIn[2 * i + 1]         * pATable[2 * i]
               - pIn[2 * i]             * pATable[2 * i + 1]
               - pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
               - pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
      */

     CoefA1 = *pCoefA++;
     CoefA2 = *pCoefA;

     /* outR = (pIn[2 * i] * pATable[2 * i] */
     mult_32x32_keep32_R (outR, *pIn1, CoefA1);

     /* - pIn[2 * i] * pATable[2 * i + 1] */
     mult_32x32_keep32_R (outI, *pIn1++, -CoefA2);

     /* pIn[2 * i + 1] * pATable[2 * i + 1] */
     multAcc_32x32_keep32_R (outR, *pIn1, CoefA2);

     /* pIn[2 * i + 1] * pATable[2 * i] */
     multAcc_32x32_keep32_R (outI, *pIn1++, CoefA1);

     /* pIn[2 * n - 2 * i] * pBTable[2 * i] */
     multAcc_32x32_keep32_R (outR, *pIn2, CoefA2);
     CoefB1 = *pCoefB;

     /* pIn[2 * n - 2 * i] * pBTable[2 * i + 1] */
     multSub_32x32_keep32_R (outI, *pIn2--, CoefB1);

     /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1] */
     multAcc_32x32_keep32_R (outR, *pIn2, CoefB1);

     /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
     multAcc_32x32_keep32_R (outI, *pIn2--, CoefA2);

     /* write output */
     *pDst++ = outR;
     *pDst++ = outI;

     /* update coefficient pointer */
     pCoefB = pCoefB + (modifier * 2);
     pCoefA = pCoefA + (modifier * 2 - 1);

     /* Decrement loop count */
     fftLen--;
  }

}

