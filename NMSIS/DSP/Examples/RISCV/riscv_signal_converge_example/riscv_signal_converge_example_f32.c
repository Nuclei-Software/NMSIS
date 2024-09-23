/* ----------------------------------------------------------------------
* Copyright (C) 2010-2012 ARM Limited. All rights reserved.
* Copyright (c) 2019 Nuclei Limited. All rights reserved.
*
* $Date:         17. January 2013
* $Revision:     V1.4.0
*
* Project:       NMSIS DSP Library
* Title:         riscv_signal_converge_example_f32.c
*
* Description:   Example code demonstrating convergence of an adaptive
*                filter.
*
* Target Processor: RISC-V Cores
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------- */

/**
 * @addtogroup groupExamples
 * @{
 *
 * @defgroup SignalConvergence Signal Convergence Example
 *
 * \par Description:
 * \par
 * Demonstrates the ability of an adaptive filter to "learn" the transfer function of
 * a FIR lowpass filter using the Normalized LMS Filter, Finite Impulse
 * Response (FIR) Filter, and Basic Math Functions.
 *
 * \par Algorithm:
 * \par
 * The figure below illustrates the signal flow in this example. Uniformly distributed white
 * noise is passed through an FIR lowpass filter. The output of the FIR filter serves as the
 * reference input of the adaptive filter (normalized LMS filter). The white noise is input
 * to the adaptive filter. The adaptive filter learns the transfer function of the FIR filter.
 * The filter outputs two signals: (1) the output of the internal adaptive FIR filter, and
 * (2) the error signal which is the difference between the adaptive filter and the reference
 * output of the FIR filter. Over time as the adaptive filter learns the transfer function
 * of the FIR filter, the first output approaches the reference output of the FIR filter,
 * and the error signal approaches zero.
 * \par
 * The adaptive filter converges properly even if the input signal has a large dynamic
 * range (i.e., varies from small to large values). The coefficients of the adaptive filter
 * are initially zero, and then converge over 1536 samples. The internal function test_signal_converge()
 * implements the stopping condition. The function checks if all of the values of the error signal have a
 * magnitude below a threshold DELTA.
 *
 * \par Block Diagram:
 * \par
 * \image html SignalFlow.png
 *
 *
 * \par Variables Description:
 * \par
 * \li \c testInput_f32 points to the input data
 * \li \c firStateF32 points to FIR state buffer
 * \li \c lmsStateF32 points to Normalised Least mean square FIR filter state buffer
 * \li \c FIRCoeff_f32 points to coefficient buffer
 * \li \c lmsNormCoeff_f32 points to Normalised Least mean square FIR filter coefficient buffer
 * \li \c wire1, wir2, wire3 temporary buffers
 * \li \c errOutput, err_signal temporary error buffers
 *
 * \par NMSIS DSP Software Library Functions Used:
 * \par
 * - riscv_lms_norm_init_f32()
 * - riscv_fir_init_f32()
 * - riscv_fir_f32()
 * - riscv_lms_norm_f32()
 * - riscv_scale_f32()
 * - riscv_abs_f32()
 * - riscv_sub_f32()
 * - riscv_min_f32()
 * - riscv_copy_f32()
 *
 * <b> Refer  </b>
 * \link riscv_signal_converge_example_f32.c \endlink
 *
 * \example riscv_signal_converge_example_f32.c
 *
 * @} */


#include <stdio.h>
#include "riscv_math.h"
#include "math_helper.h"

/* ----------------------------------------------------------------------
** Global defines for the simulation
* ------------------------------------------------------------------- */

#define TEST_LENGTH_SAMPLES 1536
#define NUMTAPS               32
#define BLOCKSIZE             32
#define DELTA_ERROR         0.00009f
#define DELTA_COEFF         0.0001f
#define MU                  0.5f

#define NUMFRAMES (TEST_LENGTH_SAMPLES / BLOCKSIZE)

/* ----------------------------------------------------------------------
* Declare FIR state buffers and structure
* ------------------------------------------------------------------- */

float32_t firStateF32[NUMTAPS + BLOCKSIZE];
riscv_fir_instance_f32 LPF_instance;

/* ----------------------------------------------------------------------
* Declare LMSNorm state buffers and structure
* ------------------------------------------------------------------- */

float32_t lmsStateF32[NUMTAPS + BLOCKSIZE];
float32_t errOutput[TEST_LENGTH_SAMPLES];
riscv_lms_norm_instance_f32 lmsNorm_instance;


/* ----------------------------------------------------------------------
* Function Declarations for Signal Convergence Example
* ------------------------------------------------------------------- */

riscv_status test_signal_converge_example( void );


/* ----------------------------------------------------------------------
* Internal functions
* ------------------------------------------------------------------- */
riscv_status test_signal_converge(float32_t* err_signal,
                        uint32_t blockSize);

void getinput(float32_t* input,
     uint32_t fr_cnt,
          uint32_t blockSize);

/* ----------------------------------------------------------------------
* External Declarations for FIR F32 module Test
* ------------------------------------------------------------------- */
extern float32_t testInput_f32[TEST_LENGTH_SAMPLES];
extern float32_t lmsNormCoeff_f32[32];
extern const float32_t FIRCoeff_f32[32];
extern riscv_lms_norm_instance_f32 lmsNorm_instance;

/* ----------------------------------------------------------------------
* Declare I/O buffers
* ------------------------------------------------------------------- */

float32_t wire1[BLOCKSIZE];
float32_t wire2[BLOCKSIZE];
float32_t wire3[BLOCKSIZE];
float32_t err_signal[BLOCKSIZE];

/* ----------------------------------------------------------------------
* Signal converge test
* ------------------------------------------------------------------- */

int32_t main(void)
{
/*  Enable Vector  */
#if (defined(__riscv_vector))
  __RV_CSR_SET(CSR_MSTATUS, 0x200);
#endif

  uint32_t i;
  riscv_status status;
  uint32_t index;
  float32_t minValue;

  /* Initialize the LMSNorm data structure */
  riscv_lms_norm_init_f32(&lmsNorm_instance, NUMTAPS, lmsNormCoeff_f32, lmsStateF32, MU, BLOCKSIZE);

  /* Initialize the FIR data structure */
  riscv_fir_init_f32(&LPF_instance, NUMTAPS, (float32_t *)FIRCoeff_f32, firStateF32, BLOCKSIZE);

  /* ----------------------------------------------------------------------
  * Loop over the frames of data and execute each of the processing
  * functions in the system.
  * ------------------------------------------------------------------- */

  for(i=0; i < NUMFRAMES; i++)
  {
    /* Read the input data - uniformly distributed random noise - into wire1 */
    riscv_copy_f32(testInput_f32 + (i * BLOCKSIZE), wire1, BLOCKSIZE);

    /* Execute the FIR processing function.  Input wire1 and output wire2 */
    riscv_fir_f32(&LPF_instance, wire1, wire2, BLOCKSIZE);

    /* Execute the LMS Norm processing function*/

    riscv_lms_norm_f32(&lmsNorm_instance, /* LMSNorm instance */
         wire1,                         /* Input signal */
         wire2,                         /* Reference Signal */
         wire3,                         /* Converged Signal */
         err_signal,                    /* Error Signal, this will become small as the signal converges */
         BLOCKSIZE);                    /* BlockSize */

    /* apply overall gain */
    riscv_scale_f32(wire3, 5, wire3, BLOCKSIZE);   /* in-place buffer */
  }

  status = RISCV_MATH_SUCCESS;

  /* -------------------------------------------------------------------------------
  * Test whether the error signal has reached towards 0.
  * ----------------------------------------------------------------------------- */

  riscv_abs_f32(err_signal, err_signal, BLOCKSIZE);
  riscv_min_f32(err_signal, BLOCKSIZE, &minValue, &index);

  if (minValue > DELTA_ERROR)
  {
  	printf("first failed\n");
    status = RISCV_MATH_TEST_FAILURE;
  }

  /* ----------------------------------------------------------------------
  * Test whether the filter coefficients have converged.
  * ------------------------------------------------------------------- */

  riscv_sub_f32((float32_t *)FIRCoeff_f32, lmsNormCoeff_f32, lmsNormCoeff_f32, NUMTAPS);

  riscv_abs_f32(lmsNormCoeff_f32, lmsNormCoeff_f32, NUMTAPS);
  riscv_min_f32(lmsNormCoeff_f32, NUMTAPS, &minValue, &index);

  if (minValue > DELTA_COEFF)
  {
    printf("second failed\n");
    status = RISCV_MATH_TEST_FAILURE;
  }

  /* ----------------------------------------------------------------------
  * Loop here if the signals did not pass the convergence check.
  * This denotes a test failure
  * ------------------------------------------------------------------- */

  if ( status != RISCV_MATH_SUCCESS)
  {
  	printf("failed\n");
    return 1;
  }
  printf("passed\n");
  return 0;

}

 /** \endlink */
