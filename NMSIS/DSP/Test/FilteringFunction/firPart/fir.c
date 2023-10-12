#include "ref.h"

void ref_fir_f32(const riscv_fir_instance_f32 *S, float32_t *pSrc,
                 float32_t *pDst, uint32_t blockSize)
{
    float32_t *pState = S->pState;         /* State pointer */
    const float32_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    float32_t *pStateCurnt; /* Points to the current sample of the state */
    uint32_t numTaps =
        S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t i;     /* Loop counters */
    float32_t acc;

    /* S->pState points to state array which contains previous frame (numTaps -
     * 1) samples */
    /* pStateCurnt points to the location where the new input data should be
     * written */
    pStateCurnt = &(S->pState[(numTaps - 1U)]);

    while (blockSize > 0U) {
        /* Copy one sample at a time into state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Set the accumulator to zero */
        acc = 0.0f;

        for (i = 0; i < numTaps; i++) {
            /* Perform the multiply-accumulates */
            acc += pState[i] * pCoeffs[i];
        }

        /* The result is store in the destination buffer. */
        *pDst++ = acc;

        /* Advance state pointer by 1 for the next sample */
        pState++;

        blockSize--;
    }

    /* Processing is complete.
    ** Now copy the last numTaps - 1 samples to the starting of the state
    *buffer.
    ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    /* Copy data */
    for (i = 0; i < numTaps - 1; i++) {
        pStateCurnt[i] = pState[i];
    }
}

#if defined (RISCV_FLOAT16_SUPPORTED)
void ref_fir_f16(
  const riscv_fir_instance_f16 * S,
        float16_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
        float16_t *pState = S->pState;                 /* State pointer */
  const float16_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
        float16_t *pStateCurnt;                        /* Points to the current sample of the state */
        float16_t *px;                                 /* Temporary pointer for state buffer */
  const float16_t *pb;                                 /* Temporary pointer for coefficient buffer */
        float16_t acc0;                                /* Accumulator */
        uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
        uint32_t i, tapCnt, blkCnt;                    /* Loop counters */

  pStateCurnt = &(S->pState[(numTaps - 1U)]);

  /* Initialize blkCnt with number of taps */
  blkCnt = blockSize;

  while (blkCnt > 0U)
  {
    /* Copy one sample at a time into state buffer */
    *pStateCurnt++ = *pSrc++;

    /* Set the accumulator to zero */
    acc0 = 0.0f;

    /* Initialize state pointer */
    px = pState;

    /* Initialize Coefficient pointer */
    pb = pCoeffs;

    i = numTaps;

    /* Perform the multiply-accumulates */
    while (i > 0U)
    {
      acc0 += (float16_t)*px++ * (float16_t)*pb++;

      i--;
    }

    /* Store result in destination buffer. */
    *pDst++ = acc0;

    /* Advance state pointer by 1 for the next sample */
    pState = pState + 1U;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Points to the start of the state buffer */
  pStateCurnt = S->pState;

  /* Initialize tapCnt with number of taps */
  tapCnt = (numTaps - 1U);

  /* Copy remaining data */
  while (tapCnt > 0U)
  {
    *pStateCurnt++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

void ref_fir_q31(const riscv_fir_instance_q31 *S, q31_t *pSrc, q31_t *pDst,
                 uint32_t blockSize)
{
    q31_t *pState = S->pState;         /* State pointer */
    const q31_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q31_t *pStateCurnt; /* Points to the current sample of the state */
    uint32_t numTaps =
        S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t i;     /* Loop counter */
    q63_t acc;

    /* S->pState points to state array which contains previous frame (numTaps -
     * 1) samples */
    /* pStateCurnt points to the location where the new input data should be
     * written */
    pStateCurnt = &(S->pState[(numTaps - 1U)]);

    while (blockSize > 0U) {
        /* Copy one sample at a time into state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Set the accumulator to zero */
        acc = 0.0f;

        for (i = 0; i < numTaps; i++) {
            /* Perform the multiply-accumulates */
            acc += (q63_t)pState[i] * pCoeffs[i];
        }

        /* The result is store in the destination buffer. */
        *pDst++ = (q31_t)(acc >> 31);

        /* Advance state pointer by 1 for the next sample */
        pState++;

        blockSize--;
    }

    /* Processing is complete.
    ** Now copy the last numTaps - 1 samples to the starting of the state
    *buffer.
    ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    /* Copy data */
    for (i = 0; i < numTaps - 1; i++) {
        pStateCurnt[i] = pState[i];
    }
}

void ref_fir_fast_q31(const riscv_fir_instance_q31 *S, q31_t *pSrc, q31_t *pDst,
                      uint32_t blockSize)
{
    q31_t *pState = S->pState;         /* State pointer */
    const q31_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q31_t *pStateCurnt; /* Points to the current sample of the state */
    uint32_t numTaps =
        S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t i;     /* Loop counter */
    q31_t acc;

    /* S->pState points to state array which contains previous frame (numTaps -
     * 1) samples */
    /* pStateCurnt points to the location where the new input data should be
     * written */
    pStateCurnt = &(S->pState[(numTaps - 1U)]);

    while (blockSize > 0U) {
        /* Copy one sample at a time into state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Set the accumulator to zero */
        acc = 0.0f;

        for (i = 0; i < numTaps; i++) {
            /* Perform the multiply-accumulates */
            acc = (q31_t)(((((q63_t)acc) << 32) +
                           ((q63_t)pState[i] * pCoeffs[i]) + 0x80000000LL) >>
                          32);
        }

        /* The result is store in the destination buffer. */
        *pDst++ = (q31_t)(acc << 1);

        /* Advance state pointer by 1 for the next sample */
        pState++;

        blockSize--;
    }

    /* Processing is complete.
    ** Now copy the last numTaps - 1 samples to the starting of the state
    *buffer.
    ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    /* Copy data */
    for (i = 0; i < numTaps - 1; i++) {
        pStateCurnt[i] = pState[i];
    }
}

void ref_fir_q15(const riscv_fir_instance_q15 *S, q15_t *pSrc, q15_t *pDst,
                 uint32_t blockSize)
{
    q15_t *pState = S->pState;         /* State pointer */
    const q15_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q15_t *pStateCurnt; /* Points to the current sample of the state */
    uint32_t numTaps =
        S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t i;     /* Loop counter */
    q63_t acc;

    /* S->pState points to state array which contains previous frame (numTaps -
     * 1) samples */
    /* pStateCurnt points to the location where the new input data should be
     * written */
    pStateCurnt = &(S->pState[(numTaps - 1U)]);

    while (blockSize > 0U) {
        /* Copy one sample at a time into state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Set the accumulator to zero */
        acc = 0.0f;

        for (i = 0; i < numTaps; i++) {
            /* Perform the multiply-accumulates */
            acc += (q31_t)pState[i] * pCoeffs[i];
        }

        /* The result is store in the destination buffer. */
        *pDst++ = ref_sat_q15(acc >> 15);

        /* Advance state pointer by 1 for the next sample */
        pState++;

        blockSize--;
    }

    /* Processing is complete.
    ** Now copy the last numTaps - 1 samples to the starting of the state
    *buffer.
    ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    /* Copy data */
    for (i = 0; i < numTaps; i++) {
        pStateCurnt[i] = pState[i];
    }
}

void ref_fir_fast_q15(const riscv_fir_instance_q15 *S, q15_t *pSrc, q15_t *pDst,
                      uint32_t blockSize)
{
    q15_t *pState = S->pState;         /* State pointer */
    const q15_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q15_t *pStateCurnt; /* Points to the current sample of the state */
    uint32_t numTaps =
        S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t i;     /* Loop counter */
    q31_t acc;

    /* S->pState points to state array which contains previous frame (numTaps -
     * 1) samples */
    /* pStateCurnt points to the location where the new input data should be
     * written */
    pStateCurnt = &(S->pState[(numTaps - 1U)]);

    while (blockSize > 0U) {
        /* Copy one sample at a time into state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Set the accumulator to zero */
        acc = 0.0f;

        for (i = 0; i < numTaps; i++) {
            /* Perform the multiply-accumulates */
            acc += (q31_t)pState[i] * pCoeffs[i];
        }

        /* The result is store in the destination buffer. */
        *pDst++ = ref_sat_q15(acc >> 15);

        /* Advance state pointer by 1 for the next sample */
        pState++;

        blockSize--;
    }

    /* Processing is complete.
    ** Now copy the last numTaps - 1 samples to the starting of the state
    *buffer.
    ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    /* Copy data */
    for (i = 0; i < numTaps - 1; i++) {
        pStateCurnt[i] = pState[i];
    }
}

void ref_fir_q7(const riscv_fir_instance_q7 *S, q7_t *pSrc, q7_t *pDst,
                uint32_t blockSize)
{
    q7_t *pState = S->pState;         /* State pointer */
    const q7_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q7_t *pStateCurnt; /* Points to the current sample of the state */
    uint32_t numTaps =
        S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t i;     /* Loop counter */
    q31_t acc;

    /* S->pState points to state array which contains previous frame (numTaps -
     * 1) samples */
    /* pStateCurnt points to the location where the new input data should be
     * written */
    pStateCurnt = &(S->pState[(numTaps - 1U)]);

    while (blockSize > 0U) {
        /* Copy one sample at a time into state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Set the accumulator to zero */
        acc = 0.0f;

        for (i = 0; i < numTaps; i++) {
            /* Perform the multiply-accumulates */
            acc += (q31_t)pState[i] * pCoeffs[i];
        }

        /* The result is store in the destination buffer. */
        *pDst++ = ref_sat_q7(acc >> 7);

        /* Advance state pointer by 1 for the next sample */
        pState++;

        blockSize--;
    }

    /* Processing is complete.
    ** Now copy the last numTaps - 1 samples to the starting of the state
    *buffer.
    ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    /* Copy data */
    for (i = 0; i < numTaps - 1; i++) {
        pStateCurnt[i] = pState[i];
    }
}
