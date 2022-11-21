#include "ref.h"

void ref_copy_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i];
    }
}

void ref_copy_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i];
    }
}

void ref_copy_q15(q15_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i];
    }
}

void ref_copy_q7(q7_t *pSrc, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i];
    }
}

void ref_fill_f32(float32_t value, float32_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = value;
    }
}

void ref_fill_q31(q31_t value, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = value;
    }
}

void ref_fill_q15(q15_t value, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = value;
    }
}

void ref_fill_q7(q7_t value, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = value;
    }
}

void ref_q31_to_q15(q31_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] >> 16;
    }
}

void ref_q31_to_q7(q31_t *pSrc, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] >> 24;
    }
}

void ref_q15_to_q31(q15_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ((q31_t)pSrc[i]) << 16;
    }
}

void ref_q15_to_q7(q15_t *pSrc, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] >> 8;
    }
}

void ref_q7_to_q31(q7_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ((q31_t)pSrc[i]) << 24;
    }
}

void ref_q7_to_q15(q7_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ((q15_t)pSrc[i]) << 8;
    }
}

void ref_q63_to_float(q63_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ((float32_t)pSrc[i]) / 9223372036854775808.0f;
    }
}

void ref_q31_to_float(q31_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ((float32_t)pSrc[i]) / 2147483648.0f;
    }
}

void ref_q15_to_float(q15_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ((float32_t)pSrc[i]) / 32768.0f;
    }
}

void ref_q7_to_float(q7_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ((float32_t)pSrc[i]) / 128.0f;
    }
}

void ref_float_to_q31(float32_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    float32_t in;

    for (i = 0; i < blockSize; i++) {
        in = pSrc[i];
        in *= 2147483648.0f;              // scale up
        in += in > 0.0f ? 0.5f : -0.5f;   // round
        pDst[i] = ref_sat_q31((q63_t)in); // cast and saturate
    }
}

void ref_float_to_q15(float32_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    float32_t in;

    for (i = 0; i < blockSize; i++) {
        in = pSrc[i];
        in *= 32768.0f;
        in += in > 0.0f ? 0.5f : -0.5f;
        pDst[i] = ref_sat_q15((q31_t)in);
    }
}

void ref_float_to_q7(float32_t *pSrc, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    float32_t in;

    for (i = 0; i < blockSize; i++) {
        in = pSrc[i];
        in *= 128.0f;
        in += in > 0.0f ? 0.5f : -0.5f;
        pDst[i] = ref_sat_q7((q15_t)in);
    }
}