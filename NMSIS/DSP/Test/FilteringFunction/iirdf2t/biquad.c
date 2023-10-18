#include "ref.h"

void ref_biquad_cascade_df2T_f32(const riscv_biquad_cascade_df2T_instance_f32 *S,
                                 float32_t *pSrc, float32_t *pDst,
                                 uint32_t blockSize)
{
    float32_t *pIn = pSrc;                 /*  source pointer            */
    float32_t *pOut = pDst;                /*  destination pointer       */
    float32_t *pState = S->pState;         /*  State pointer             */
    const float32_t *pCoeffs = S->pCoeffs; /*  coefficient pointer       */
    float32_t acc;                         /*  accumulator               */
    float32_t b0, b1, b2, a1, a2;          /*  Filter coefficients       */
    float32_t Xn;                          /*  temporary input           */
    float32_t d1, d2;                      /*  state variables           */
    uint32_t sample, stage = S->numStages; /*  loop counters             */

    do {
        /* Reading the coefficients */
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        /*Reading the state values */
        d1 = pState[0];
        d2 = pState[1];

        sample = blockSize;

        while (sample > 0U) {
            /* Read the input */
            Xn = *pIn++;

            /* y[n] = b0 * x[n] + d1 */
            acc = (b0 * Xn) + d1;

            /* Store the result in the accumulator in the destination buffer. */
            *pOut++ = acc;

            /* Every time after the output is computed state should be updated.
             */
            /* d1 = b1 * x[n] + a1 * y[n] + d2 */
            d1 = (b1 * Xn + a1 * acc) + d2;

            /* d2 = b2 * x[n] + a2 * y[n] */
            d2 = (b2 * Xn) + (a2 * acc);

            /* decrement the loop counter */
            sample--;
        }

        /* Store the updated state variables back into the state array */
        *pState++ = d1;
        *pState++ = d2;

        /* The current stage input is given as the output to the next stage */
        pIn = pDst;

        /*Reset the output working pointer */
        pOut = pDst;

        /* decrement the loop counter */
        stage--;

    } while (stage > 0U);
}

#if defined (RISCV_FLOAT16_SUPPORTED)
void ref_biquad_cascade_df2T_f16(
  const riscv_biquad_cascade_df2T_instance_f16 * S,
        float16_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
  const float16_t *pIn = pSrc;                         /* Source pointer */
        float16_t *pOut = pDst;                        /* Destination pointer */
        float16_t *pState = S->pState;                 /* State pointer */
  const float16_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
        float16_t acc1;                                /* Accumulator */
        float16_t b0, b1, b2, a1, a2;                  /* Filter coefficients */
        float16_t Xn1;                                 /* Temporary input */
        float16_t d1, d2;                              /* State variables */
        uint32_t sample, stage = S->numStages;         /* Loop counters */

  do
  {
     /* Reading the coefficients */
     b0 = pCoeffs[0];
     b1 = pCoeffs[1];
     b2 = pCoeffs[2];
     a1 = pCoeffs[3];
     a2 = pCoeffs[4];

     /* Reading the state values */
     d1 = pState[0];
     d2 = pState[1];

     pCoeffs += 5U;

      /* Initialize blkCnt with number of samples */
      sample = blockSize;

      while (sample > 0U) {
        Xn1 = *pIn++;

        acc1 = b0 * Xn1 + d1;

        d1 = b1 * Xn1 + d2;
        d1 += a1 * acc1;

        d2 = b2 * Xn1;
        d2 += a2 * acc1;

        *pOut++ = acc1;

        /* decrement loop counter */
        sample--;
      }

      /* Store the updated state variables back into the state array */
      pState[0] = d1;
      pState[1] = d2;

      pState += 2U;

      /* The current stage output is given as the input to the next stage */
      pIn = pDst;

      /* Reset the output working pointer */
      pOut = pDst;

      /* decrement loop counter */
      stage--;

   } while (stage > 0U);
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

void ref_biquad_cascade_stereo_df2T_f32(
    const riscv_biquad_cascade_stereo_df2T_instance_f32 *S, float32_t *pSrc,
    float32_t *pDst, uint32_t blockSize)
{
    float32_t *pIn = pSrc;                 /*  source pointer            */
    float32_t *pOut = pDst;                /*  destination pointer       */
    float32_t *pState = S->pState;         /*  State pointer             */
    const float32_t *pCoeffs = S->pCoeffs; /*  coefficient pointer       */
    float32_t acc1a, acc1b;                /*  accumulator               */
    float32_t b0, b1, b2, a1, a2;          /*  Filter coefficients       */
    float32_t Xn1a, Xn1b;                  /*  temporary input           */
    float32_t d1a, d2a, d1b, d2b;          /*  state variables           */
    uint32_t sample, stage = S->numStages; /*  loop counters             */

    do {
        /* Reading the coefficients */
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        /*Reading the state values */
        d1a = pState[0];
        d2a = pState[1];
        d1b = pState[2];
        d2b = pState[3];

        sample = blockSize;

        while (sample > 0U) {
            /* Read the input */
            Xn1a = *pIn++; // Channel a
            Xn1b = *pIn++; // Channel b

            /* y[n] = b0 * x[n] + d1 */
            acc1a = (b0 * Xn1a) + d1a;
            acc1b = (b0 * Xn1b) + d1b;

            /* Store the result in the accumulator in the destination buffer. */
            *pOut++ = acc1a;
            *pOut++ = acc1b;

            /* Every time after the output is computed state should be updated.
             */
            /* d1 = b1 * x[n] + a1 * y[n] + d2 */
            d1a = ((b1 * Xn1a) + (a1 * acc1a)) + d2a;
            d1b = ((b1 * Xn1b) + (a1 * acc1b)) + d2b;

            /* d2 = b2 * x[n] + a2 * y[n] */
            d2a = (b2 * Xn1a) + (a2 * acc1a);
            d2b = (b2 * Xn1b) + (a2 * acc1b);

            /* decrement the loop counter */
            sample--;
        }

        /* Store the updated state variables back into the state array */
        *pState++ = d1a;
        *pState++ = d2a;
        *pState++ = d1b;
        *pState++ = d2b;

        /* The current stage input is given as the output to the next stage */
        pIn = pDst;

        /*Reset the output working pointer */
        pOut = pDst;

        /* decrement the loop counter */
        stage--;

    } while (stage > 0U);
}

#if defined (RISCV_FLOAT16_SUPPORTED)
void ref_biquad_cascade_stereo_df2T_f16(
  const riscv_biquad_cascade_stereo_df2T_instance_f16 * S,
        float16_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
  const float16_t *pIn = pSrc;                         /* Source pointer */
        float16_t *pOut = pDst;                        /* Destination pointer */
        float16_t *pState = S->pState;                 /* State pointer */
  const float16_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
        float16_t acc1a, acc1b;                        /* Accumulator */
        float16_t b0, b1, b2, a1, a2;                  /* Filter coefficients */
        float16_t Xn1a, Xn1b;                          /* Temporary input */
        float16_t d1a, d2a, d1b, d2b;                  /* State variables */
        uint32_t sample, stage = S->numStages;         /* Loop counters */

    do
    {
        /* Reading the coefficients */
        b0 = pCoeffs[0];
        b1 = pCoeffs[1];
        b2 = pCoeffs[2];
        a1 = pCoeffs[3];
        a2 = pCoeffs[4];

        /* Reading the state values */
        d1a = pState[0];
        d2a = pState[1];
        d1b = pState[2];
        d2b = pState[3];

        pCoeffs += 5U;

        /* Initialize blkCnt with number of samples */
        sample = blockSize;

        while (sample > 0U) {
          /* Read the input */
          Xn1a = *pIn++; /* Channel a */
          Xn1b = *pIn++; /* Channel b */

          /* y[n] = b0 * x[n] + d1 */
          acc1a = (b0 * Xn1a) + d1a;
          acc1b = (b0 * Xn1b) + d1b;

          /* Store the result in the accumulator in the destination buffer. */
          *pOut++ = acc1a;
          *pOut++ = acc1b;

          /* Every time after the output is computed state should be updated. */
          /* d1 = b1 * x[n] + a1 * y[n] + d2 */
          d1a = ((b1 * Xn1a) + (a1 * acc1a)) + d2a;
          d1b = ((b1 * Xn1b) + (a1 * acc1b)) + d2b;

          /* d2 = b2 * x[n] + a2 * y[n] */
          d2a = (b2 * Xn1a) + (a2 * acc1a);
          d2b = (b2 * Xn1b) + (a2 * acc1b);

          /* decrement loop counter */
          sample--;
        }

        /* Store the updated state variables back into the state array */
        pState[0] = d1a;
        pState[1] = d2a;

        pState[2] = d1b;
        pState[3] = d2b;

        pState += 4U;

        /* The current stage output is given as the input to the next stage */
        pIn = pDst;

        /* Reset the output working pointer */
        pOut = pDst;

        /* Decrement the loop counter */
        stage--;

    } while (stage > 0U);
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

void ref_biquad_cascade_df2T_f64(const riscv_biquad_cascade_df2T_instance_f64 *S,
                                 float64_t *pSrc, float64_t *pDst,
                                 uint32_t blockSize)
{
    float64_t *pIn = pSrc;                 /*  source pointer            */
    float64_t *pOut = pDst;                /*  destination pointer       */
    float64_t *pState = S->pState;         /*  State pointer             */
    const float64_t *pCoeffs = S->pCoeffs; /*  coefficient pointer       */
    float64_t acc;                         /*  accumulator               */
    float64_t b0, b1, b2, a1, a2;          /*  Filter coefficients       */
    float64_t Xn;                          /*  temporary input           */
    float64_t d1, d2;                      /*  state variables           */
    uint32_t sample, stage = S->numStages; /*  loop counters             */

    do {
        /* Reading the coefficients */
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        /*Reading the state values */
        d1 = pState[0];
        d2 = pState[1];

        sample = blockSize;

        while (sample > 0U) {
            /* Read the input */
            Xn = *pIn++;

            /* y[n] = b0 * x[n] + d1 */
            acc = (b0 * Xn) + d1;

            /* Store the result in the accumulator in the destination buffer. */
            *pOut++ = acc;

            /* Every time after the output is computed state should be updated.
             */
            /* d1 = b1 * x[n] + a1 * y[n] + d2 */
            d1 = (b1 * Xn + a1 * acc) + d2;

            /* d2 = b2 * x[n] + a2 * y[n] */
            d2 = (b2 * Xn) + (a2 * acc);

            /* decrement the loop counter */
            sample--;
        }

        /* Store the updated state variables back into the state array */
        *pState++ = d1;
        *pState++ = d2;

        /* The current stage input is given as the output to the next stage */
        pIn = pDst;

        /*Reset the output working pointer */
        pOut = pDst;

        /* decrement the loop counter */
        stage--;

    } while (stage > 0U);
}
