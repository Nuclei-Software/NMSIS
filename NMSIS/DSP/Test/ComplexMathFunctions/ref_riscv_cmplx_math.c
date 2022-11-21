#include "ref.h"

void ref_cmplx_mag_f32(float32_t *pSrc, float32_t *pDst, uint32_t numSamples)
{
    uint32_t i;

    for (i = 0; i < numSamples * 2; i += 2) {
        *pDst++ = sqrtf(pSrc[i] * pSrc[i] + pSrc[i + 1] * pSrc[i + 1]);
    }
}

void ref_cmplx_mag_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    q31_t acc0, acc1, out;

    for (i = 0; i < numSamples * 2; i += 2) {
        acc0 = (q31_t)(((q63_t)pSrc[i] * pSrc[i]) >> 33);
        acc1 = (q31_t)(((q63_t)pSrc[i + 1] * pSrc[i + 1]) >> 33);
        out = acc0 + acc1;
        *pDst++ = (q31_t)(sqrtf((float)out / 2147483648.0f) * 2147483648.0f);
    }
}

void ref_cmplx_mag_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    q31_t acc0, acc1;
    q15_t out;

    for (i = 0; i < numSamples * 2; i += 2) {
        acc0 = pSrc[i] * pSrc[i];
        acc1 = pSrc[i + 1] * pSrc[i + 1];
        out = (q15_t)(((q63_t)acc0 + acc1) >> 17);
        *pDst++ = (q15_t)(sqrtf((float)out / 32768.0f) * 32768.0f);
    }
}

void ref_cmplx_mag_squared_f32(float32_t *pSrc, float32_t *pDst,
                               uint32_t numSamples)
{
    uint32_t i;

    for (i = 0; i < numSamples * 2; i += 2) {
        *pDst++ = pSrc[i] * pSrc[i] + pSrc[i + 1] * pSrc[i + 1];
    }
}

void ref_cmplx_mag_squared_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    q31_t acc0, acc1;

    for (i = 0; i < numSamples * 2; i += 2) {
        acc0 = (q31_t)(((q63_t)pSrc[i] * pSrc[i]) >> 33);
        acc1 = (q31_t)(((q63_t)pSrc[i + 1] * pSrc[i + 1]) >> 33);
        *pDst++ = acc0 + acc1;
    }
}

void ref_cmplx_mag_squared_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    q31_t acc0, acc1;

    for (i = 0; i < numSamples * 2; i += 2) {
        acc0 = pSrc[i] * pSrc[i];
        acc1 = pSrc[i + 1] * pSrc[i + 1];
        *pDst++ = (q15_t)(((q63_t)acc0 + acc1) >> 17);
    }
}

void ref_cmplx_conj_f32(float32_t *pSrc, float32_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    for (i = 0; i < numSamples * 2; i += 2) {
        pDst[i] = pSrc[i];
        pDst[i + 1] = -pSrc[i + 1];
    }
}

void ref_cmplx_conj_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    for (i = 0; i < numSamples * 2; i += 2) {
        pDst[i] = pSrc[i];
        pDst[i + 1] = -pSrc[i + 1];
    }
}

void ref_cmplx_conj_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    for (i = 0; i < numSamples * 2; i += 2) {
        pDst[i] = pSrc[i];
        pDst[i + 1] = -pSrc[i + 1];
    }
}

void ref_cmplx_dot_prod_f32(float32_t *pSrcA, float32_t *pSrcB,
                            uint32_t numSamples, float32_t *realResult,
                            float32_t *imagResult)
{
    uint32_t blkCnt = numSamples;
    float32_t real_sum = 0.0f, imag_sum = 0.0f;    /* Temporary result variables */
    float32_t a0, b0, c0, d0;

    while (blkCnt > 0U)
    {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += a0 * c0;
        imag_sum += a0 * d0;
        real_sum -= b0 * d0;
        imag_sum += b0 * c0;
        /* Decrement loop counter */
        blkCnt--;
    }

  /* Store real and imaginary result in destination buffer. */
  *realResult = real_sum;
  *imagResult = imag_sum;
}

void ref_cmplx_dot_prod_q31(q31_t *pSrcA, q31_t *pSrcB, uint32_t numSamples,
                            q63_t *realResult, q63_t *imagResult)
{
    q63_t sumr, sumi;
    uint32_t i;

    sumr = 0;
    sumi = 0;

    for (i = 0; i < numSamples * 2; i += 2) {
        // shifting down 14 here to provide guard bits
        sumr += (((q63_t)pSrcA[i] * pSrcB[i]) >> 14) -
                (((q63_t)pSrcA[i + 1] * pSrcB[i + 1]) >> 14);
        sumi += (((q63_t)pSrcA[i] * pSrcB[i + 1]) >> 14) +
                (((q63_t)pSrcA[i + 1] * pSrcB[i]) >> 14);
    }

    *realResult = sumr;
    *imagResult = sumi;
}

void ref_cmplx_dot_prod_q15(q15_t *pSrcA, q15_t *pSrcB, uint32_t numSamples,
                            q31_t *realResult, q31_t *imagResult)
{
    q63_t sumr, sumi;
    uint32_t i;

    sumr = 0;
    sumi = 0;

    for (i = 0; i < numSamples * 2; i += 2) {
        sumr += (q31_t)pSrcA[i] * pSrcB[i] - (q31_t)pSrcA[i + 1] * pSrcB[i + 1];
        sumi += (q31_t)pSrcA[i] * pSrcB[i + 1] + (q31_t)pSrcA[i + 1] * pSrcB[i];
    }

    // shifting down 6 at the end here because there are already 32 guard bits
    // available, this method is more accurate
    *realResult = (q31_t)(sumr >> 6);
    *imagResult = (q31_t)(sumi >> 6);
}

void ref_cmplx_mult_cmplx_f32(float32_t *pSrcA, float32_t *pSrcB,
                              float32_t *pDst, uint32_t numSamples)
{
    uint32_t i;

    for (i = 0; i < numSamples * 2; i += 2) {
        pDst[i] = pSrcA[i] * pSrcB[i] - pSrcA[i + 1] * pSrcB[i + 1];
        pDst[i + 1] = pSrcA[i] * pSrcB[i + 1] + pSrcA[i + 1] * pSrcB[i];
    }
}

void ref_cmplx_mult_cmplx_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst,
                              uint32_t numSamples)
{
    uint32_t i;
    q31_t mul1, mul2, mul3, mul4;

    for (i = 0; i < numSamples * 2; i += 2) {
        mul1 = ((q63_t)pSrcA[i] * pSrcB[i]) >> 33;
        mul2 = ((q63_t)pSrcA[i + 1] * pSrcB[i + 1]) >> 33;
        mul3 = ((q63_t)pSrcA[i] * pSrcB[i + 1]) >> 33;
        mul4 = ((q63_t)pSrcA[i + 1] * pSrcB[i]) >> 33;
        pDst[i] = mul1 - mul2;
        pDst[i + 1] = mul3 + mul4;
    }
}

void ref_cmplx_mult_cmplx_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst,
                              uint32_t numSamples)
{
    uint32_t i;
    q31_t mul1, mul2, mul3, mul4;

    for (i = 0; i < numSamples * 2; i += 2) {
        mul1 = ((q31_t)pSrcA[i] * pSrcB[i]) >> 17;
        mul2 = ((q31_t)pSrcA[i + 1] * pSrcB[i + 1]) >> 17;
        mul3 = ((q31_t)pSrcA[i] * pSrcB[i + 1]) >> 17;
        mul4 = ((q31_t)pSrcA[i + 1] * pSrcB[i]) >> 17;
        pDst[i] = mul1 - mul2;
        pDst[i + 1] = mul3 + mul4;
    }
}

void ref_cmplx_mult_real_f32(float32_t *pSrcCmplx, float32_t *pSrcReal,
                             float32_t *pCmplxDst, uint32_t numSamples)
{
    uint32_t i;

    for (i = 0; i < numSamples; i++) {
        pCmplxDst[2 * i + 0] = pSrcCmplx[2 * i + 0] * pSrcReal[i];
        pCmplxDst[2 * i + 1] = pSrcCmplx[2 * i + 1] * pSrcReal[i];
    }
}

void ref_cmplx_mult_real_q31(q31_t *pSrcCmplx, q31_t *pSrcReal,
                             q31_t *pCmplxDst, uint32_t numSamples)
{
    uint32_t i;
    q31_t tempR, tempI;

    for (i = 0; i < numSamples; i++) {
        tempR = ((q63_t)pSrcCmplx[2 * i + 0] * pSrcReal[i]) >> 32;
        tempI = ((q63_t)pSrcCmplx[2 * i + 1] * pSrcReal[i]) >> 32;
        pCmplxDst[2 * i + 0] = ref_sat_n(tempR, 31) << 1;
        pCmplxDst[2 * i + 1] = ref_sat_n(tempI, 31) << 1;
    }
}

void ref_cmplx_mult_real_q15(q15_t *pSrcCmplx, q15_t *pSrcReal,
                             q15_t *pCmplxDst, uint32_t numSamples)
{
    uint32_t i;
    q31_t tempR, tempI;

    for (i = 0; i < numSamples; i++) {
        tempR = ((q31_t)pSrcCmplx[2 * i + 0] * pSrcReal[i]) >> 15;
        tempI = ((q31_t)pSrcCmplx[2 * i + 1] * pSrcReal[i]) >> 15;
        pCmplxDst[2 * i + 0] = ref_sat_q15(tempR);
        pCmplxDst[2 * i + 1] = ref_sat_q15(tempI);
    }
}
