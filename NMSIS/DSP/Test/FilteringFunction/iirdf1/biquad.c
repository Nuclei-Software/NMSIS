#include "ref.h"

void ref_biquad_cascade_df1_f32(const riscv_biquad_casd_df1_inst_f32 *S,
                                float32_t *pSrc, float32_t *pDst,
                                uint32_t blockSize)
{
    float32_t *pIn = pSrc;                 /*  source pointer            */
    float32_t *pOut = pDst;                /*  destination pointer       */
    float32_t *pState = S->pState;         /*  pState pointer            */
    const float32_t *pCoeffs = S->pCoeffs; /*  coefficient pointer       */
    float32_t acc;                         /*  Simulates the accumulator */
    float32_t b0, b1, b2, a1, a2;          /*  Filter coefficients       */
    float32_t Xn1, Xn2, Yn1, Yn2;          /*  Filter pState variables   */
    float32_t Xn;                          /*  temporary input           */
    uint32_t sample, stage = S->numStages; /*  loop counters             */

    do {
        /* Reading the coefficients */
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        /* Reading the pState values */
        Xn1 = pState[0];
        Xn2 = pState[1];
        Yn1 = pState[2];
        Yn2 = pState[3];

        /*      The variables acc holds the output value that is computed:
         *    acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1]   + a2
         * * y[n-2]
         */

        sample = blockSize;

        while (sample > 0U) {
            /* Read the input */
            Xn = *pIn++;

            /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 *
             * y[n-2] */
            acc = (b0 * Xn) + (b1 * Xn1) + (b2 * Xn2) + (a1 * Yn1) + (a2 * Yn2);

            /* Store the result in the accumulator in the destination buffer. */
            *pOut++ = acc;

            /* Every time after the output is computed state should be updated.
             */
            /* The states should be updated as:    */
            /* Xn2 = Xn1    */
            /* Xn1 = Xn     */
            /* Yn2 = Yn1    */
            /* Yn1 = acc   */
            Xn2 = Xn1;
            Xn1 = Xn;
            Yn2 = Yn1;
            Yn1 = acc;

            /* decrement the loop counter */
            sample--;
        }

        /*  Store the updated state variables back into the pState array */
        *pState++ = Xn1;
        *pState++ = Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;

        /*  The first stage goes from the input buffer to the output buffer. */
        /*  Subsequent numStages  occur in-place in the output buffer */
        pIn = pDst;

        /* Reset the output pointer */
        pOut = pDst;

        /* decrement the loop counter */
        stage--;

    } while (stage > 0U);
}

#if defined (RISCV_FLOAT16_SUPPORTED)
void ref_biquad_cascade_df1_f16(
  const riscv_biquad_casd_df1_inst_f16 * S,
        float16_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
  const float16_t *pIn = pSrc;                         /* Source pointer */
        float16_t *pOut = pDst;                        /* Destination pointer */
        float16_t *pState = S->pState;                 /* pState pointer */
  const float16_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
        float16_t acc;                                 /* Accumulator */
        float16_t b0, b1, b2, a1, a2;                  /* Filter coefficients */
        float16_t Xn1, Xn2, Yn1, Yn2;                  /* Filter pState variables */
        float16_t Xn;                                  /* Temporary input */
        uint32_t sample, stage = S->numStages;         /* Loop counters */
  do
  {
    /* Reading the coefficients */
    b0 = *pCoeffs++;
    b1 = *pCoeffs++;
    b2 = *pCoeffs++;
    a1 = *pCoeffs++;
    a2 = *pCoeffs++;

    /* Reading the pState values */
    Xn1 = pState[0];
    Xn2 = pState[1];
    Yn1 = pState[2];
    Yn2 = pState[3];

    /* Initialize blkCnt with number of samples */
    sample = blockSize;

    while (sample > 0U)
    {
      /* Read the input */
      Xn = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      acc = (b0 * Xn) + (b1 * Xn1) + (b2 * Xn2) + (a1 * Yn1) + (a2 * Yn2);

      /* Store output in destination buffer. */
      *pOut++ = acc;

      Xn2 = Xn1;
      Xn1 = Xn;
      Yn2 = Yn1;
      Yn1 = acc;

      /* decrement loop counter */
      sample--;
    }

    /* Store the updated state variables back into the pState array */
    *pState++ = Xn1;
    *pState++ = Xn2;
    *pState++ = Yn1;
    *pState++ = Yn2;

    pIn = pDst;

    /* Reset output pointer */
    pOut = pDst;

    /* decrement loop counter */
    stage--;

  } while (stage > 0U);
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

void ref_biquad_cas_df1_32x64_q31(const riscv_biquad_cas_df1_32x64_ins_q31 *S,
                                  q31_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    q31_t *pIn = pSrc;  /*  input pointer initialization  */
    q31_t *pOut = pDst; /*  output pointer initialization */
    q63_t *pState =
        S->pState; /*  state pointer initialization  			*/
    const q31_t *pCoeffs =
        S->pCoeffs;           /*  coeff pointer initialization  			*/
    q63_t acc;                /*  accumulator                   			*/
    q31_t Xn1, Xn2;           /*  Input Filter state variables  			*/
    q63_t Yn1, Yn2;           /*  Output Filter state variables 			*/
    q31_t b0, b1, b2, a1, a2; /*  Filter coefficients */
    q31_t Xn;                 /*  temporary input               			*/
    int32_t shift = (int32_t)S->postShift +
                    1; /*  Shift to be applied to the output 	*/
    uint32_t sample, stage = S->numStages; /*  loop counters */
    q31_t acc_l, acc_h; /*  temporary output               		*/
    uint32_t uShift = ((uint32_t)S->postShift + 1U);
    uint32_t lShift = 32U - uShift; /*  Shift to be applied to the output
                                     */

    do {
        /* Reading the coefficients */
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        /* Reading the state values */
        Xn1 = pState[0];
        Xn2 = pState[1];
        Yn1 = pState[2];
        Yn2 = pState[3];

        sample = blockSize;

        while (sample > 0U) {
            /* Read the input */
            Xn = *pIn++;

            /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 *
             * y[n-2] */
            acc = (q63_t)Xn * b0 + (q63_t)Xn1 * b1 + (q63_t)Xn2 * b2;
            /* acc +=  a1 * y[n-1] */
            acc += mult32x64(Yn1, a1);
            /* acc +=  a2 * y[n-2] */
            acc += mult32x64(Yn2, a2);

            /* Every time after the output is computed state should be updated.
             */
            Xn2 = Xn1;
            Xn1 = Xn;
            Yn2 = Yn1;

            /* The result is converted to 1.63, Yn1 variable is reused  */
            Yn1 = acc << shift;

            /* Calc lower part of acc */
            acc_l = acc & 0xffffffff;

            /* Calc upper part of acc */
            acc_h = (acc >> 32) & 0xffffffff;

            /* Apply shift for lower part of acc and upper part of acc */
            acc_h = (uint32_t)acc_l >> lShift | acc_h << uShift;

            /* Store the output in the destination buffer in 1.31 format. */
            *pOut++ = acc_h;

            /* decrement the loop counter */
            sample--;
        }

        /*  The first stage output is given as input to the second stage. */
        pIn = pDst;

        /* Reset to destination buffer working pointer */
        pOut = pDst;

        /*  Store the updated state variables back into the pState array */
        *pState++ = (q63_t)Xn1;
        *pState++ = (q63_t)Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;

    } while (--stage);
}

void ref_biquad_cascade_df1_q31(const riscv_biquad_casd_df1_inst_q31 *S,
                                q31_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    q63_t acc; /*  accumulator                   */
    uint32_t uShift = ((uint32_t)S->postShift + 1U);
    uint32_t lShift = 32U - uShift;    /*  Shift to be applied to the output */
    q31_t *pIn = pSrc;                 /*  input pointer initialization  */
    q31_t *pOut = pDst;                /*  output pointer initialization */
    q31_t *pState = S->pState;         /*  pState pointer initialization */
    const q31_t *pCoeffs = S->pCoeffs; /*  coeff pointer initialization  */
    q31_t Xn1, Xn2, Yn1, Yn2;          /*  Filter state variables        */
    q31_t b0, b1, b2, a1, a2;          /*  Filter coefficients           */
    q31_t Xn;                          /*  temporary input               */
    uint32_t sample, stage = S->numStages; /*  loop counters                 */

    do {
        /* Reading the coefficients */
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        /* Reading the state values */
        Xn1 = pState[0];
        Xn2 = pState[1];
        Yn1 = pState[2];
        Yn2 = pState[3];

        /*      The variables acc holds the output value that is computed:
         *    acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 *
         * y[n-2]
         */

        sample = blockSize;

        while (sample > 0U) {
            /* Read the input */
            Xn = *pIn++;

            /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 *
             * y[n-2] */
            /* acc =  b0 * x[n] */
            acc = (q63_t)b0 * Xn;

            /* acc +=  b1 * x[n-1] */
            acc += (q63_t)b1 * Xn1;
            /* acc +=  b[2] * x[n-2] */
            acc += (q63_t)b2 * Xn2;
            /* acc +=  a1 * y[n-1] */
            acc += (q63_t)a1 * Yn1;
            /* acc +=  a2 * y[n-2] */
            acc += (q63_t)a2 * Yn2;

            /* The result is converted to 1.31  */
            acc = acc >> lShift;

            /* Every time after the output is computed state should be updated.
             */
            /* The states should be updated as:  */
            /* Xn2 = Xn1    */
            /* Xn1 = Xn     */
            /* Yn2 = Yn1    */
            /* Yn1 = acc    */
            Xn2 = Xn1;
            Xn1 = Xn;
            Yn2 = Yn1;
            Yn1 = (q31_t)acc;

            /* Store the output in the destination buffer. */
            *pOut++ = (q31_t)acc;

            /* decrement the loop counter */
            sample--;
        }

        /*  The first stage goes from the input buffer to the output buffer. */
        /*  Subsequent stages occur in-place in the output buffer */
        pIn = pDst;

        /* Reset to destination pointer */
        pOut = pDst;

        /*  Store the updated state variables back into the pState array */
        *pState++ = Xn1;
        *pState++ = Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;

    } while (--stage);
}

void ref_biquad_cascade_df1_fast_q31(const riscv_biquad_casd_df1_inst_q31 *S,
                                     q31_t *pSrc, q31_t *pDst,
                                     uint32_t blockSize)
{
    q31_t acc = 0;                     /*  accumulator                   */
    q31_t Xn1, Xn2, Yn1, Yn2;          /*  Filter state variables        */
    q31_t b0, b1, b2, a1, a2;          /*  Filter coefficients           */
    q31_t *pIn = pSrc;                 /*  input pointer initialization  */
    q31_t *pOut = pDst;                /*  output pointer initialization */
    q31_t *pState = S->pState;         /*  pState pointer initialization */
    const q31_t *pCoeffs = S->pCoeffs; /*  coeff pointer initialization  */
    q31_t Xn;                          /*  temporary input               */
    int32_t shift =
        (int32_t)S->postShift + 1; /*  Shift to be applied to the output */
    uint32_t sample, stage = S->numStages; /*  loop counters */

    do {
        /* Reading the coefficients */
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        /* Reading the state values */
        Xn1 = pState[0];
        Xn2 = pState[1];
        Yn1 = pState[2];
        Yn2 = pState[3];

        sample = blockSize;

        while (sample > 0U) {
            /* Read the input */
            Xn = *pIn++;

            /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 *
             * y[n-2] */
            mult_32x32_keep32_R(acc, b0, Xn);
            multAcc_32x32_keep32_R(acc, b1, Xn1);
            multAcc_32x32_keep32_R(acc, b2, Xn2);
            multAcc_32x32_keep32_R(acc, a1, Yn1);
            multAcc_32x32_keep32_R(acc, a2, Yn2);

            /* The result is converted to 1.31  */
            acc <<= shift;

            /* Every time after the output is computed state should be updated.
             */
            Xn2 = Xn1;
            Xn1 = Xn;
            Yn2 = Yn1;
            Yn1 = acc;

            /* Store the output in the destination buffer. */
            *pOut++ = acc;

            /* decrement the loop counter */
            sample--;
        }

        /*  The first stage goes from the input buffer to the output buffer. */
        /*  Subsequent stages occur in-place in the output buffer */
        pIn = pDst;

        /* Reset to destination pointer */
        pOut = pDst;

        /*  Store the updated state variables back into the pState array */
        *pState++ = Xn1;
        *pState++ = Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;

    } while (--stage);
}

void ref_biquad_cascade_df1_fast_q15(const riscv_biquad_casd_df1_inst_q15 *S,
                                     q15_t *pSrc, q15_t *pDst,
                                     uint32_t blockSize)
{
  const q15_t *pIn = pSrc;                             /* Source pointer */
        q15_t *pOut = pDst;                            /* Destination pointer */
        q15_t *pState = S->pState;                     /* State pointer */
  const q15_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t acc;                                     /* Accumulator */
        q31_t in;                                      /* Temporary variable to hold input value */
        q31_t out;                                     /* Temporary variable to hold output value */
        q31_t b0;                                      /* Temporary variable to hold bo value */
        q31_t b1, a1;                                  /* Filter coefficients */
        q31_t state_in, state_out;                     /* Filter state variables */
        int32_t shift = (int32_t) (15 - S->postShift); /* Post shift */
        uint32_t sample, stage = S->numStages;         /* Loop counters */

  do
  {
    /* Read the b0 and 0 coefficients using SIMD  */
    b0 = read_q15x2_ia ((q15_t **)&pCoeffs);

    /* Read the b1 and b2 coefficients using SIMD */
    b1 = read_q15x2_ia ((q15_t **)&pCoeffs);

    /* Read the a1 and a2 coefficients using SIMD */
    a1 = read_q15x2_ia ((q15_t **)&pCoeffs);

    /* Read the input state values from the state buffer:  x[n-1], x[n-2] */
    state_in = read_q15x2_ia (&pState);

    /* Read the output state values from the state buffer:  y[n-1], y[n-2] */
    state_out = read_q15x2_da (&pState);

    /* Initialize blkCnt with number of samples */
    sample = blockSize;

    while (sample > 0U)
    {
      /* Read the input */
      in = *pIn++;

      /* out =  b0 * x[n] + 0 * 0 */
      out = __SMUAD(b0, in);

      /* acc =  b1 * x[n-1], acc +=  b2 * x[n-2] + out */
      acc = __SMLAD(b1, state_in, out);
      /* acc +=  a1 * y[n-1] + acc +=  a2 * y[n-2] */
      acc = __SMLAD(a1, state_out, acc);

      /* The result is converted from 3.29 to 1.31 and then saturation is applied */
      out = __SSAT((acc >> shift), 16);

      /* Store the output in the destination buffer. */
      *pOut++ = (q15_t) out;

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as:  */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */
      /* x[n-N], x[n-N-1] are packed together to make state_in of type q31 */
      /* y[n-N], y[n-N-1] are packed together to make state_out of type q31 */
      state_in = __PKHBT(in, state_in, 16);
      state_out = __PKHBT(out, state_out, 16);

      /* decrement loop counter */
      sample--;
    }

    /* The first stage goes from the input buffer to the output buffer. */
    /* Subsequent (numStages - 1) occur in-place in the output buffer */
    pIn = pDst;

    /* Reset the output pointer */
    pOut = pDst;

    /* Store the updated state variables back into the state array */
    write_q15x2_ia(&pState, state_in);
    write_q15x2_ia(&pState, state_out);

    /* Decrement loop counter */
    stage--;

  } while (stage > 0U);
}

void ref_biquad_cascade_df1_q15(const riscv_biquad_casd_df1_inst_q15 *S,
                                q15_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
  const q15_t *pIn = pSrc;                             /* Source pointer */
        q15_t *pOut = pDst;                            /* Destination pointer */
        q15_t b0, b1, b2, a1, a2;                      /* Filter coefficients */
        q15_t Xn1, Xn2, Yn1, Yn2;                      /* Filter state variables */
        q15_t Xn;                                      /* temporary input */
        q63_t acc;                                     /* Accumulator */
        int32_t shift = (15 - (int32_t) S->postShift); /* Post shift */
        q15_t *pState = S->pState;                     /* State pointer */
  const q15_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        uint32_t sample, stage = (uint32_t) S->numStages;     /* Stage loop counter */

  do
  {
    /* Reading the coefficients */
    b0 = *pCoeffs++;
    pCoeffs++;  // skip the 0 coefficient
    b1 = *pCoeffs++;
    b2 = *pCoeffs++;
    a1 = *pCoeffs++;
    a2 = *pCoeffs++;

    /* Reading the state values */
    Xn1 = pState[0];
    Xn2 = pState[1];
    Yn1 = pState[2];
    Yn2 = pState[3];

    /* The variables acc holds the output value that is computed:
     *    acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
     */

    sample = blockSize;

    while (sample > 0U)
    {
      /* Read the input */
      Xn = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      /* acc =  b0 * x[n] */
      acc = (q31_t) b0 *Xn;

      /* acc +=  b1 * x[n-1] */
      acc += (q31_t) b1 *Xn1;
      /* acc +=  b[2] * x[n-2] */
      acc += (q31_t) b2 *Xn2;
      /* acc +=  a1 * y[n-1] */
      acc += (q31_t) a1 *Yn1;
      /* acc +=  a2 * y[n-2] */
      acc += (q31_t) a2 *Yn2;

      /* The result is converted to 1.31  */
      acc = __SSAT((acc >> shift), 16);

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as:  */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */
      Xn2 = Xn1;
      Xn1 = Xn;
      Yn2 = Yn1;
      Yn1 = (q15_t) acc;

      /* Store the output in the destination buffer. */
      *pOut++ = (q15_t) acc;

      /* decrement the loop counter */
      sample--;
    }

    /*  The first stage goes from the input buffer to the output buffer. */
    /*  Subsequent stages occur in-place in the output buffer */
    pIn = pDst;

    /* Reset to destination pointer */
    pOut = pDst;

    /*  Store the updated state variables back into the pState array */
    *pState++ = Xn1;
    *pState++ = Xn2;
    *pState++ = Yn1;
    *pState++ = Yn2;

  } while (--stage);
}
