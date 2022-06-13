#include "ref.h"

void ref_mse_f32(float32_t *pSrcA, const float32_t *pSrcB,
                   uint32_t blockSize, float32_t *result)
{
    uint32_t blkCnt; /* Loop counter */
    float32_t inA, inB;
    float32_t sum = 0.0f; /* Temporary return variable */
    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;

    while (blkCnt > 0U) {
        inA = *pSrcA++;
        inB = *pSrcB++;
        inA = inA - inB;
        sum += inA * inA;
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store result in destination buffer */
    *result = sum / blockSize;
}

void ref_mse_q31(q31_t *pSrcA, const q31_t *pSrcB, uint32_t blockSize,
                   q31_t *pResult)
{
    uint32_t blkCnt; /* Loop counter */
    q63_t sum = 0;   /* Temporary result storage */
    q31_t inA32, inB32; /* Temporary variable to store packed input value */
    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;

    while (blkCnt > 0U) {
        inA32 = *pSrcA++ >> 1;
        inB32 = *pSrcB++ >> 1;
        inA32 = __QSUB(inA32, inB32);
        sum += ((q63_t)inA32 * inA32) >> 14U;
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store result in q31 format */
    *pResult = (q31_t)((sum / blockSize) >> 15);
}

void ref_mse_q15(q15_t *pSrcA, const q15_t *pSrcB, uint32_t blockSize,
                   q15_t *pResult)
{
    uint32_t blkCnt; /* Loop counter */
    q63_t sum = 0;   /* Temporary result storage */
    q15_t inA, inB;  /* Temporary variable to store input value */
    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;

    while (blkCnt > 0U) {
        inA = *pSrcA++ >> 1;
        inB = *pSrcB++ >> 1;
        inA = (q15_t)__SSAT(((q31_t)inA - (q31_t)inB), 16);
        sum += (q63_t)((q31_t)inA * inA);
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store result in q15 format */
    *pResult = (q15_t)__SSAT((q31_t)(sum / blockSize) >> 13, 16);
}

void ref_mse_q7(q7_t *pSrcA, const q7_t *pSrcB, uint32_t blockSize,
                  q7_t *pResult)
{
    uint32_t blkCnt; /* Loop counter */
    q31_t sum = 0;   /* Temporary result storage */
    q7_t inA, inB;   /* Temporary variable to store input value */
    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;

    while (blkCnt > 0U) {
        inA = *pSrcA++ >> 1;
        inB = *pSrcB++ >> 1;
        inA = (q7_t)__SSAT((q15_t)inA - (q15_t)inB, 8);
        sum += ((q15_t)inA * inA);
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store result in q7 format */
    *pResult = (q7_t)__SSAT((q15_t)(sum / blockSize) >> 5, 8);
}