/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_rfft_fast_init_f16.c
 * Description:  Split Radix Decimation in Frequency CFFT Floating point processing function
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2023 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/transform_functions_f16.h"
#include "riscv_common_tables_f16.h"
#include "riscv_const_structs_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

/**
  @ingroup RealFFT
 */

/**
  @addtogroup RealFFTF16
  @{
 */


/**
  @brief         Initialization function for the 32pt floating-point real FFT.
  @param[in,out] S  points to an riscv_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected
 */

riscv_status riscv_rfft_fast_init_32_f16( riscv_rfft_fast_instance_f16 * S ) {

  riscv_status status;

  if( !S ) return RISCV_MATH_ARGUMENT_ERROR;

  status=riscv_cfft_init_16_f16(&(S->Sint));
  if (status != RISCV_MATH_SUCCESS)
  {
    return(status);
  }

  S->fftLenRFFT = 32U;
  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_32;

  return RISCV_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 64pt floating-point real FFT.
  @param[in,out] S  points to an riscv_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected
 */

riscv_status riscv_rfft_fast_init_64_f16( riscv_rfft_fast_instance_f16 * S ) {

  riscv_status status;

  if( !S ) return RISCV_MATH_ARGUMENT_ERROR;

  status=riscv_cfft_init_32_f16(&(S->Sint));
  if (status != RISCV_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 64U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_64;

  return RISCV_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 128pt floating-point real FFT.
  @param[in,out] S  points to an riscv_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected
 */

riscv_status riscv_rfft_fast_init_128_f16( riscv_rfft_fast_instance_f16 * S ) {

  riscv_status status;

  if( !S ) return RISCV_MATH_ARGUMENT_ERROR;

  status=riscv_cfft_init_64_f16(&(S->Sint));
  if (status != RISCV_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 128;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_128;

  return RISCV_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 256pt floating-point real FFT.
  @param[in,out] S  points to an riscv_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected
*/

riscv_status riscv_rfft_fast_init_256_f16( riscv_rfft_fast_instance_f16 * S ) {

  riscv_status status;

  if( !S ) return RISCV_MATH_ARGUMENT_ERROR;

  status=riscv_cfft_init_128_f16(&(S->Sint));
  if (status != RISCV_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 256U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_256;

  return RISCV_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 512pt floating-point real FFT.
  @param[in,out] S  points to an riscv_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected
 */

riscv_status riscv_rfft_fast_init_512_f16( riscv_rfft_fast_instance_f16 * S ) {

  riscv_status status;

  if( !S ) return RISCV_MATH_ARGUMENT_ERROR;

  status=riscv_cfft_init_256_f16(&(S->Sint));
  if (status != RISCV_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 512U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_512;

  return RISCV_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 1024pt floating-point real FFT.
  @param[in,out] S  points to an riscv_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected
 */

riscv_status riscv_rfft_fast_init_1024_f16( riscv_rfft_fast_instance_f16 * S ) {

  riscv_status status;

  if( !S ) return RISCV_MATH_ARGUMENT_ERROR;

  status=riscv_cfft_init_512_f16(&(S->Sint));
  if (status != RISCV_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 1024U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_1024;

  return RISCV_MATH_SUCCESS;
}

/**
  @brief         Initialization function for the 2048pt floating-point real FFT.
  @param[in,out] S  points to an riscv_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected
 */
riscv_status riscv_rfft_fast_init_2048_f16( riscv_rfft_fast_instance_f16 * S ) {

  riscv_status status;

  if( !S ) return RISCV_MATH_ARGUMENT_ERROR;

  status=riscv_cfft_init_1024_f16(&(S->Sint));
  if (status != RISCV_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 2048U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_2048;

  return RISCV_MATH_SUCCESS;
}

/**
* @brief         Initialization function for the 4096pt floating-point real FFT.
* @param[in,out] S  points to an riscv_rfft_fast_instance_f16 structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected
 */

riscv_status riscv_rfft_fast_init_4096_f16( riscv_rfft_fast_instance_f16 * S ) {

  riscv_status status;

  if( !S ) return RISCV_MATH_ARGUMENT_ERROR;

  status=riscv_cfft_init_2048_f16(&(S->Sint));
  if (status != RISCV_MATH_SUCCESS)
  {
    return(status);
  }
  S->fftLenRFFT = 4096U;

  S->pTwiddleRFFT    = (float16_t *) twiddleCoefF16_rfft_4096;

  return RISCV_MATH_SUCCESS;
}

/**
  @brief         Generic initialization function for the floating-point real FFT.
  @param[in,out] S       points to an riscv_rfft_fast_instance_f16 structure
  @param[in]     fftLen  length of the Real Sequence
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : <code>fftLen</code> is not a supported length

  @par           Description
                   The parameter <code>fftLen</code> specifies the length of RFFT/CIFFT process.
                   Supported FFT Lengths are 32, 64, 128, 256, 512, 1024, 2048, 4096.
  @par
                   This Function also initializes Twiddle factor table pointer and Bit reversal table pointer.

  @par
                This function should be used only if you don't know the FFT sizes that
                you'll need at build time. The use of this function will prevent the
                linker from removing the FFT tables that are not needed and the library
                code size will be bigger than needed.

  @par
                If you use NMSIS-DSP as a static library, and if you know the FFT sizes
                that you need at build time, then it is better to use the initialization
                functions defined for each FFT size.

 */

riscv_status riscv_rfft_fast_init_f16(
  riscv_rfft_fast_instance_f16 * S,
  uint16_t fftLen)
{
  riscv_status status;


  switch (fftLen)
  {
  case 4096U:
    status = riscv_rfft_fast_init_4096_f16(S);
    break;
  case 2048U:
    status = riscv_rfft_fast_init_2048_f16(S);
    break;
  case 1024U:
    status = riscv_rfft_fast_init_1024_f16(S);
    break;
  case 512U:
    status = riscv_rfft_fast_init_512_f16(S);
    break;
  case 256U:
    status = riscv_rfft_fast_init_256_f16(S);
    break;
  case 128U:
    status = riscv_rfft_fast_init_128_f16(S);
    break;
  case 64U:
    status = riscv_rfft_fast_init_64_f16(S);
    break;
  case 32U:
    status = riscv_rfft_fast_init_32_f16(S);
    break;
  default:
    return(RISCV_MATH_ARGUMENT_ERROR);
    break;
  }

  return(status);

}

/**
  @} end of RealFFTF16 group
 */

#endif /*  #if defined(RISCV_FLOAT16_SUPPORTED) */
