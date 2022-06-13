#include "ref.h"

void ref_min_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult,
                 uint32_t *pIndex)
{
    uint32_t i, ind = 0;
    float32_t min = FLT_MAX;

    for (i = 0; i < blockSize; i++) {
        if (min > pSrc[i]) {
            min = pSrc[i];
            ind = i;
        }
    }
    *pResult = min;
    *pIndex = ind;
}

void ref_min_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult,
                 uint32_t *pIndex)
{
    uint32_t i, ind = 0;
    q31_t min = INT_MAX;

    for (i = 0; i < blockSize; i++) {
        if (min > pSrc[i]) {
            min = pSrc[i];
            ind = i;
        }
    }
    *pResult = min;
    *pIndex = ind;
}

void ref_min_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult,
                 uint32_t *pIndex)
{
    uint32_t i, ind = 0;
    q15_t min = SHRT_MAX;

    for (i = 0; i < blockSize; i++) {
        if (min > pSrc[i]) {
            min = pSrc[i];
            ind = i;
        }
    }
    *pResult = min;
    *pIndex = ind;
}

void ref_min_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult, uint32_t *pIndex)
{
    uint32_t i, ind = 0;
    q7_t min = SCHAR_MAX;

    for (i = 0; i < blockSize; i++) {
        if (min > pSrc[i]) {
            min = pSrc[i];
            ind = i;
        }
    }
    *pResult = min;
    *pIndex = ind;
}

void ref_min_no_idx_f32(float32_t *pSrc, uint32_t blockSize,
                        float32_t *pResult)
{
    float32_t minValue = F32_MAX;
    float32_t newVal;

    while (blockSize > 0U) {
        newVal = *pSrc++;
        /* compare for the minimum value */
        if (minValue > newVal) {
            /* Update the minimum value and it's index */
            minValue = newVal;
        }
        blockSize--;
    }
    *pResult = minValue;
}

void ref_min_no_idx_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult)
{
    q31_t minVal1, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;    /* loop counter */
    /* Load first input value that act as reference value for comparision */
    out = *pSrc++;
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal1 = *pSrc++;
        /* compare for the minimum value */
        if (out > minVal1) {
            /* Update the minimum value */
            out = minVal1;
        }
        /* Decrement the loop counter */
        blkCnt--;
    }
    /* Store the minimum value into destination pointer */
    *pResult = out;
}

void ref_min_no_idx_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult)
{
    q15_t minVal1, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;    /* loop counter */
    /* Load first input value that act as reference value for comparision */
    out = *pSrc++;
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal1 = *pSrc++;
        /* compare for the minimum value */
        if (out > minVal1) {
            /* Update the minimum value */
            out = minVal1;
        }
        /* Decrement the loop counter */
        blkCnt--;
    }
    /* Store the minimum value into destination pointer */
    *pResult = out;
}

void ref_min_no_idx_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult)
{
    q7_t minVal1, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;   /* loop counter */
    /* Load first input value that act as reference value for comparision */
    out = *pSrc++;
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal1 = *pSrc++;
        /* compare for the minimum value */
        if (out > minVal1) {
            /* Update the minimum value */
            out = minVal1;
        }
        /* Decrement the loop counter */
        blkCnt--;
    }
    /* Store the minimum value into destination pointer */
    *pResult = out;
}
