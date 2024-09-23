/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_radix2_init_q31.c
 * Description:  Radix-2 Decimation in Frequency Fixed-point CFFT & CIFFT Initialization function
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
#include "riscv_common_tables.h"

/**
  @addtogroup ComplexFFTDeprecated
  @{
 */

/**
  @brief         Initialization function for the Q31 CFFT/CIFFT.
  @deprecated    Do not use this function. It has been superseded by \ref riscv_cfft_q31 and will be removed in the future.
  @param[in,out] S              points to an instance of the Q31 CFFT/CIFFT structure
  @param[in]     fftLen         length of the FFT
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : <code>fftLen</code> is not a supported length

  @par           Details
                   The parameter <code>ifftFlag</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlag for calculation of CIFFT otherwise CFFT is calculated
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   The parameter <code>fftLen</code> Specifies length of CFFT/CIFFT process. Supported FFT Lengths are 16, 64, 256, 1024.
  @par
                   This Function also initializes Twiddle factor table pointer and Bit reversal table pointer.
*/

RISCV_DSP_ATTRIBUTE riscv_status riscv_cfft_radix2_init_q31(
  riscv_cfft_radix2_instance_q31 * S,
  uint16_t fftLen,
  uint8_t ifftFlag,
  uint8_t bitReverseFlag)
{
  /*  Initialise the default riscv status */
  riscv_status status = RISCV_MATH_ARGUMENT_ERROR;

  /*  Initialise the default riscv status */
  status = RISCV_MATH_SUCCESS;

  /*  Initialise the FFT length */
  S->fftLen = fftLen;

  /*  Initialise the Twiddle coefficient pointer */
  S->pTwiddle = (q31_t *) twiddleCoef_4096_q31;

  /*  Initialise the Flag for selection of CFFT or CIFFT */
  S->ifftFlag = ifftFlag;

  /*  Initialise the Flag for calculation Bit reversal or not */
  S->bitReverseFlag = bitReverseFlag;

  /*  Initializations of Instance structure depending on the FFT length */
  switch (S->fftLen)
  {
    /*  Initializations of structure parameters for 4096 point FFT */
  case 4096U:
    /*  Initialise the twiddle coef modifier value */
    S->twidCoefModifier = 1U;
    /*  Initialise the bit reversal table modifier */
    S->bitRevFactor = 1U;
    /*  Initialise the bit reversal table pointer */
    S->pBitRevTable = (uint16_t *) riscvBitRevTable;
    break;

    /*  Initializations of structure parameters for 2048 point FFT */
  case 2048U:
    /*  Initialise the twiddle coef modifier value */
    S->twidCoefModifier = 2U;
    /*  Initialise the bit reversal table modifier */
    S->bitRevFactor = 2U;
    /*  Initialise the bit reversal table pointer */
    S->pBitRevTable = (uint16_t *) & riscvBitRevTable[1];
    break;

    /*  Initializations of structure parameters for 1024 point FFT */
  case 1024U:
    /*  Initialise the twiddle coef modifier value */
    S->twidCoefModifier = 4U;
    /*  Initialise the bit reversal table modifier */
    S->bitRevFactor = 4U;
    /*  Initialise the bit reversal table pointer */
    S->pBitRevTable = (uint16_t *) & riscvBitRevTable[3];
    break;

    /*  Initializations of structure parameters for 512 point FFT */
  case 512U:
    /*  Initialise the twiddle coef modifier value */
    S->twidCoefModifier = 8U;
    /*  Initialise the bit reversal table modifier */
    S->bitRevFactor = 8U;
    /*  Initialise the bit reversal table pointer */
    S->pBitRevTable = (uint16_t *) & riscvBitRevTable[7];
    break;

  case 256U:
    /*  Initializations of structure parameters for 256 point FFT */
    S->twidCoefModifier = 16U;
    S->bitRevFactor = 16U;
    S->pBitRevTable = (uint16_t *) & riscvBitRevTable[15];
    break;

  case 128U:
    /*  Initializations of structure parameters for 128 point FFT */
    S->twidCoefModifier = 32U;
    S->bitRevFactor = 32U;
    S->pBitRevTable = (uint16_t *) & riscvBitRevTable[31];
    break;

  case 64U:
    /*  Initializations of structure parameters for 64 point FFT */
    S->twidCoefModifier = 64U;
    S->bitRevFactor = 64U;
    S->pBitRevTable = (uint16_t *) & riscvBitRevTable[63];
    break;

  case 32U:
    /*  Initializations of structure parameters for 32 point FFT */
    S->twidCoefModifier = 128U;
    S->bitRevFactor = 128U;
    S->pBitRevTable = (uint16_t *) & riscvBitRevTable[127];
    break;

  case 16U:
    /*  Initializations of structure parameters for 16 point FFT */
    S->twidCoefModifier = 256U;
    S->bitRevFactor = 256U;
    S->pBitRevTable = (uint16_t *) & riscvBitRevTable[255];
    break;


  default:
    /*  Reporting argument error if fftSize is not valid value */
    status = RISCV_MATH_ARGUMENT_ERROR;
    break;
  }

  return (status);
}

/**
  @} end of ComplexFFTDeprecated group
 */
