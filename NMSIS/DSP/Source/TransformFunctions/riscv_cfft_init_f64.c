/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_init_f64.c
 * Description:  Initialization function for cfft f64 instance
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

#define FFTINIT(EXT,SIZE)                                           \
  S->bitRevLength = riscv_cfft_sR_##EXT##_len##SIZE.bitRevLength;        \
  S->pBitRevTable = riscv_cfft_sR_##EXT##_len##SIZE.pBitRevTable;         \
  S->pTwiddle = riscv_cfft_sR_##EXT##_len##SIZE.pTwiddle;

/**
 * @defgroup ComplexFFTF64 Complex FFT F64
 */

/**
  @ingroup groupTransforms
 */

/**
  @addtogroup ComplexFFT
  @{
 */

/**
  @addtogroup ComplexFFTF64
  @{
 */


#include "dsp/transform_functions.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"


#define CFFTINIT_F64(LEN)                                          \
riscv_status riscv_cfft_init_##LEN##_f64(riscv_cfft_instance_f64 * S)\
{                                                              \
    /*  Initialise the default riscv status */                   \
        riscv_status status = RISCV_MATH_SUCCESS;                  \
                                                               \
        /*  Initialise the FFT length */                       \
        S->fftLen = LEN;                                       \
                                                               \
        /*  Initialise the Twiddle coefficient pointer */      \
        S->pTwiddle = NULL;                                    \
                                                               \
        FFTINIT(f64,LEN);                                   \
                                                               \
        return (status);                                       \
}

/**
  @brief         Initialization function for the cfft f64 function with 4096 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(4096)

/**
  @brief         Initialization function for the cfft f64 function with 2048 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(2048)


/**
  @brief         Initialization function for the cfft f64 function with 1024 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(1024)

/**
  @brief         Initialization function for the cfft f64 function with 512 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(512)

/**
  @brief         Initialization function for the cfft f64 function with 256 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(256)

/**
  @brief         Initialization function for the cfft f64 function with 128 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(128)

/**
  @brief         Initialization function for the cfft f64 function with 64 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(64)

/**
  @brief         Initialization function for the cfft f64 function with 32 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(32)

/**
  @brief         Initialization function for the cfft f64 function with 16 samples
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h
 */
CFFTINIT_F64(16)

/**
  @brief         Generic initialization function for the cfft f64 function
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @param[in]     fftLen         fft length (number of complex samples)
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using
                variables declared in riscv_const_structs.h

  @par          This function should be used only if you don't know the FFT sizes that
                you'll need at build time. The use of this function will prevent the
                linker from removing the FFT tables that are not needed and the library
                code size will be bigger than needed.

  @par          If you use NMSIS-DSP as a static library, and if you know the FFT sizes
                that you need at build time, then it is better to use the initialization
                functions defined for each FFT size.

 */
RISCV_DSP_ATTRIBUTE riscv_status riscv_cfft_init_f64(
  riscv_cfft_instance_f64 * S,
  uint16_t fftLen)
{
        /*  Initialise the default riscv status */
        riscv_status status = RISCV_MATH_SUCCESS;

        /*  Initializations of Instance structure depending on the FFT length */
        switch (fftLen) {
            /*  Initializations of structure parameters for 4096 point FFT */
        case 4096U:
            /*  Initialise the bit reversal table modifier */
            status = riscv_cfft_init_4096_f64(S);
            break;

            /*  Initializations of structure parameters for 2048 point FFT */
        case 2048U:
            /*  Initialise the bit reversal table modifier */
            status = riscv_cfft_init_2048_f64(S);

            break;

            /*  Initializations of structure parameters for 1024 point FFT */
        case 1024U:
            /*  Initialise the bit reversal table modifier */
            status = riscv_cfft_init_1024_f64(S);

            break;

            /*  Initializations of structure parameters for 512 point FFT */
        case 512U:
            /*  Initialise the bit reversal table modifier */
            status = riscv_cfft_init_512_f64(S);
            break;

        case 256U:
            status = riscv_cfft_init_256_f64(S);
            break;

        case 128U:
            status = riscv_cfft_init_128_f64(S);
            break;

        case 64U:
            status = riscv_cfft_init_64_f64(S);
            break;

        case 32U:
            status = riscv_cfft_init_32_f64(S);
            break;

        case 16U:
            /*  Initializations of structure parameters for 16 point FFT */
            status = riscv_cfft_init_16_f64(S);
            break;

        default:
            /*  Reporting argument error if fftSize is not valid value */
            status = RISCV_MATH_ARGUMENT_ERROR;
            break;
        }


        return (status);
}
/**
  @} end of ComplexFFTF64 group
 */

/**
  @} end of ComplexFFT group
 */
