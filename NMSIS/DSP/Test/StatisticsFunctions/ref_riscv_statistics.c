#include "ref.h"

void ref_absmax_f32(
  const float32_t * pSrc,
        uint32_t blockSize,
        float32_t * pResult,
        uint32_t * pIndex)
{
  float32_t maxVal, out;                         /* Temporary variables to store the output value. */
  uint32_t blkCnt, outIndex;                     /* Loop counter */

  /* Initialise index value to zero. */
  outIndex = 0U;

  /* Load first input value that act as reference value for comparision */
  out = fabsf(*pSrc++);

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);


  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = fabsf(*pSrc++);

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

void ref_absmax_q7(
  const q7_t * pSrc,
        uint32_t blockSize,
        q7_t * pResult,
        uint32_t * pIndex)
{
  q7_t maxVal, out;                              /* Temporary variables to store the output value. */
  uint32_t blkCnt, outIndex;                     /* Loop counter */

  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
    pSrc++;

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

void ref_absmax_q15(
  const q15_t * pSrc,
        uint32_t blockSize,
        q15_t * pResult,
        uint32_t * pIndex)
{
  q15_t maxVal, out;                             /* Temporary variables to store the output value. */
  uint32_t blkCnt, outIndex;                     /* Loop counter */

  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t) 0x8000) ? 0x7fff : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t) 0x8000) ? 0x7fff : -*pSrc);
    pSrc++;

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

void ref_absmax_q31(
  const q31_t * pSrc,
        uint32_t blockSize,
        q31_t * pResult,
        uint32_t * pIndex)
{
  q31_t maxVal, out;                             /* Temporary variables to store the output value. */
  uint32_t blkCnt, outIndex;                     /* Loop counter */

  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
    pSrc++;

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

void ref_absmax_no_idx_f32(
  const float32_t * pSrc,
        uint32_t blockSize,
        float32_t * pResult)
{
  float32_t maxVal, out;               /* Temporary variables to store the output value. */
  uint32_t blkCnt;                     /* Loop counter */

  /* Load first input value that act as reference value for comparision */
  out = fabsf(*pSrc++);

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = fabsf(*pSrc++);

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
}

void ref_absmax_no_idx_q7(
  const q7_t * pSrc,
        uint32_t blockSize,
        q7_t * pResult)
{
  q7_t maxVal, out;                    /* Temporary variables to store the output value. */
  uint32_t blkCnt;                     /* Loop counter */

  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
    pSrc++;

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
}

void ref_absmax_no_idx_q15(
  const q15_t * pSrc,
        uint32_t blockSize,
        q15_t * pResult)
{
  q15_t maxVal, out;                   /* Temporary variables to store the output value. */
  uint32_t blkCnt;                     /* Loop counter */

  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t) 0x8000) ? 0x7fff : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t) 0x8000) ? 0x7fff : -*pSrc);
    pSrc++;

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
}

void ref_absmax_no_idx_q31(
  const q31_t * pSrc,
        uint32_t blockSize,
        q31_t * pResult)
{
  q31_t maxVal, out;                   /* Temporary variables to store the output value. */
  uint32_t blkCnt;                     /* Loop counter */

  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
    pSrc++;

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
}

void ref_absmin_f32(
  const float32_t * pSrc,
        uint32_t blockSize,
        float32_t * pResult,
        uint32_t * pIndex)
{
       float32_t minVal, out;                         /* Temporary variables to store the output value. */
       uint32_t blkCnt, outIndex;                     /* Loop counter */

  /* Initialise index value to zero. */
  outIndex = 0U;

  /* Load first input value that act as reference value for comparision */
  out = fabsf(*pSrc++);

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = fabsf(*pSrc++);

    /* compare for the minimum value */
    if (out > minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the minimum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

void ref_absmin_q7(
  const q7_t * pSrc,
        uint32_t blockSize,
        q7_t * pResult,
        uint32_t * pIndex)
{
        q7_t minVal, out;                              /* Temporary variables to store the output value. */
        uint32_t blkCnt, outIndex;                     /* Loop counter */

  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
    pSrc++;

    /* compare for the minimum value */
    if (out > minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the minimum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

void ref_absmin_q15(
  const q15_t * pSrc,
        uint32_t blockSize,
        q15_t * pResult,
        uint32_t * pIndex)
{
       q15_t minVal, out;                             /* Temporary variables to store the output value. */
        uint32_t blkCnt, outIndex;                     /* Loop counter */


  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t) 0x8000) ? 0x7fff : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t) 0x8000) ? 0x7fff : -*pSrc);
    pSrc++;

    /* compare for the minimum value */
    if (out > minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the minimum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

void ref_absmin_q31(
  const q31_t * pSrc,
        uint32_t blockSize,
        q31_t * pResult,
        uint32_t * pIndex)
{
        q31_t minVal, out;                             /* Temporary variables to store the output value. */
        uint32_t blkCnt, outIndex;                     /* Loop counter */

  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
    pSrc++;

    /* compare for the minimum value */
    if (out > minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the minimum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

void ref_absmin_no_idx_f32(const float32_t *pSrc, uint32_t blockSize,
                             float32_t *pResult)
{
    float32_t minVal, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;       /* Loop counter */
    /* Load first input value that act as reference value for comparision */
    out = fabsf(*pSrc++);
    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal = fabsf(*pSrc++);

        /* compare for the minimum value */
        if (out > minVal) {
            /* Update the minimum value and it's index */
            out = minVal;
        }
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store the minimum value and it's index into destination pointers */
    *pResult = out;
}

void ref_absmin_no_idx_q7(const q7_t *pSrc, uint32_t blockSize, q7_t *pResult)
{
    q7_t minVal, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;  /* Loop counter */
    /* Load first input value that act as reference value for comparision */
    out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t)0x80) ? (q7_t)0x7f : -*pSrc);
    pSrc++;
    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t)0x80) ? (q7_t)0x7f : -*pSrc);
        pSrc++;
        /* compare for the minimum value */
        if (out > minVal) {
            /* Update the minimum value and it's index */
            out = minVal;
        }
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store the minimum value and it's index into destination pointers */
    *pResult = out;
}

void ref_absmin_no_idx_q15(const q15_t *pSrc, uint32_t blockSize,
                             q15_t *pResult)
{
    q15_t minVal, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;   /* Loop counter */
    /* Load first input value that act as reference value for comparision */
    out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t)0x8000) ? 0x7fff : -*pSrc);
    pSrc++;
    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t)0x8000) ? 0x7fff : -*pSrc);
        pSrc++;

        /* compare for the minimum value */
        if (out > minVal) {
            /* Update the minimum value and it's index */
            out = minVal;
        }
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store the minimum value and it's index into destination pointers */
    *pResult = out;
}

void ref_absmin_no_idx_q31(const q31_t *pSrc, uint32_t blockSize,
                             q31_t *pResult)
{
    q31_t minVal, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;   /* Loop counter */
    /* Load first input value that act as reference value for comparision */
    out = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
    pSrc++;
    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
        pSrc++;
        /* compare for the minimum value */
        if (out > minVal) {
            /* Update the minimum value and it's index */
            out = minVal;
        }
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store the minimum value and it's index into destination pointers */
    *pResult = out;
}

void ref_max_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult,
                 uint32_t *pIndex)
{
    uint32_t i, ind = 0;
    float32_t max = -FLT_MAX;

    for (i = 0; i < blockSize; i++) {
        if (max < pSrc[i]) {
            max = pSrc[i];
            ind = i;
        }
    }
    *pResult = max;
    *pIndex = ind;
}

void ref_max_no_idx_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    uint32_t i = 0;
    float32_t max = -FLT_MAX;

    for (i = 0; i < blockSize; i++) {
        if (max < pSrc[i]) {
            max = pSrc[i];
        }
    }
    *pResult = max;
}

void ref_max_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult,
                 uint32_t *pIndex)
{
    uint32_t i, ind = 0;
    q31_t max = INT_MIN;

    for (i = 0; i < blockSize; i++) {
        if (max < pSrc[i]) {
            max = pSrc[i];
            ind = i;
        }
    }
    *pResult = max;
    *pIndex = ind;
}

void ref_max_no_idx_q31(q31_t *pSrc, uint32_t blockSize,
                 q31_t *pResult)
{
    q31_t maxVal1, out;
    uint32_t blkCnt;
    out = *pSrc++;
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        maxVal1 = *pSrc++;
        if (out < maxVal1) {
            out = maxVal1;
        }
        blkCnt--;
    }
    *pResult = out;
}

void ref_max_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult,
                 uint32_t *pIndex)
{
    uint32_t i, ind = 0;
    q15_t max = SHRT_MIN;

    for (i = 0; i < blockSize; i++) {
        if (max < pSrc[i]) {
            max = pSrc[i];
            ind = i;
        }
    }
    *pResult = max;
    *pIndex = ind;
}

void ref_max_no_idx_q15(q15_t *pSrc, uint32_t blockSize,
                 q15_t *pResult)
{
    q15_t maxVal1, out;
    uint32_t blkCnt;
    out = *pSrc++;
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        maxVal1 = *pSrc++;
        if (out < maxVal1) {
            out = maxVal1;
        }
        blkCnt--;
    }
    *pResult = out;
}

void ref_max_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult, uint32_t *pIndex)
{
    uint32_t i, ind = 0;
    q7_t max = SCHAR_MIN;

    for (i = 0; i < blockSize; i++) {
        if (max < pSrc[i]) {
            max = pSrc[i];
            ind = i;
        }
    }
    *pResult = max;
    *pIndex = ind;
}

void ref_max_no_idx_q7(q7_t *pSrc, uint32_t blockSize,
                 q7_t *pResult)
{
    q7_t maxVal1, out;
    uint32_t blkCnt;
    out = *pSrc++;
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        maxVal1 = *pSrc++;
        if (out < maxVal1) {
            out = maxVal1;
        }
        blkCnt--;
    }
    *pResult = out;
}

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

void ref_mean_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    uint32_t i;
    float32_t sum = 0;

    for (i = 0; i < blockSize; i++) {
        sum += pSrc[i];
    }
    *pResult = sum / (float32_t)blockSize;
}

void ref_mean_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult)
{
    uint32_t i;
    q63_t sum = 0;

    for (i = 0; i < blockSize; i++) {
        sum += pSrc[i];
    }
    *pResult = (q31_t)(sum / (int32_t)blockSize);
}

void ref_mean_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult)
{
    uint32_t i;
    q31_t sum = 0;

    for (i = 0; i < blockSize; i++) {
        sum += pSrc[i];
    }
    *pResult = (q15_t)(sum / (int32_t)blockSize);
}

void ref_mean_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult)
{
    uint32_t i;
    q31_t sum = 0;

    for (i = 0; i < blockSize; i++) {
        sum += pSrc[i];
    }
    *pResult = (q7_t)(sum / (int32_t)blockSize);
}

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

void ref_power_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    uint32_t i;
    float32_t sumsq = 0;

    for (i = 0; i < blockSize; i++) {
        sumsq += pSrc[i] * pSrc[i];
    }
    *pResult = sumsq;
}

void ref_power_q31(q31_t *pSrc, uint32_t blockSize, q63_t *pResult)
{
    uint32_t i;
    q63_t sumsq = 0;

    for (i = 0; i < blockSize; i++) {
        sumsq += ((q63_t)pSrc[i] * pSrc[i]) >> 14;
    }
    *pResult = sumsq;
}

void ref_power_q15(q15_t *pSrc, uint32_t blockSize, q63_t *pResult)
{
    uint32_t i;
    q63_t sumsq = 0;

    for (i = 0; i < blockSize; i++) {
        sumsq += (q63_t)pSrc[i] * pSrc[i];
    }
    *pResult = sumsq;
}

void ref_power_q7(q7_t *pSrc, uint32_t blockSize, q31_t *pResult)
{
    uint32_t i;
    q31_t sumsq = 0;

    for (i = 0; i < blockSize; i++) {
        sumsq += (q31_t)pSrc[i] * pSrc[i];
    }
    *pResult = sumsq;
}

void ref_rms_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    uint32_t i;
    float32_t sumsq = 0;

    for (i = 0; i < blockSize; i++) {
        sumsq += pSrc[i] * pSrc[i];
    }
    *pResult = sqrtf(sumsq / (float32_t)blockSize);
}

void ref_rms_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult)
{
  uint32_t blkCnt;         /* Loop counter */
  uint64_t sum = 0;        /* Temporary result storage (can get never negative. changed type from q63 to uint64 */
  q31_t in;                /* Temporary variable to store input value */

  blkCnt = blockSize;
  while (blkCnt > 0U)
  {
    /* C = A[0] * A[0] + A[1] * A[1] + ... + A[blockSize-1] * A[blockSize-1] */

    in = *pSrc++;
    /* Compute sum of squares and store result in a temporary variable. */
    sum += ((q63_t)in * in);

    /* Decrement loop counter */
    blkCnt--;
  }
  /* Convert data in 2.62 to 1.31 by 31 right shifts and saturate */
  /* Compute Rms and store result in destination vector */
  riscv_sqrt_q31(clip_q63_to_q31((sum / (q63_t)blockSize) >> 31), pResult);
}

void ref_rms_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult)
{
    uint32_t i;
    q63_t sumsq = 0;
    q31_t tmp1;
    q15_t tmp2;

    for (i = 0; i < blockSize; i++) {
        sumsq += (q63_t)pSrc[i] * pSrc[i];
    }
    tmp1 = (sumsq / (q63_t)blockSize) >> 15;
    tmp2 = ref_sat_q15(tmp1);
    riscv_sqrt_q15(tmp2, pResult);
}

void ref_std_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    uint32_t i;
    float32_t sum = 0, sumsq = 0;

    if (blockSize == 1) {
        *pResult = 0;
        return;
    }

    for (i = 0; i < blockSize; i++) {
        sum += pSrc[i];
        sumsq += pSrc[i] * pSrc[i];
    }
    *pResult = sqrtf((sumsq - sum * sum / (float32_t)blockSize) /
                     ((float32_t)blockSize - 1));
}

void ref_std_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult)
{
  uint32_t blkCnt;                   /* Loop counter */
  q63_t sum = 0;                     /* Accumulator */
  q63_t meanOfSquares, squareOfMean; /* Square of mean and mean of square */
  q63_t sumOfSquares = 0;            /* Sum of squares */
  q31_t in;                    /* Temporary variable to store input value */

  blkCnt = blockSize;
  while (blkCnt > 0U)
  {
    /* C = A[0] * A[0] + A[1] * A[1] + ... + A[blockSize-1] * A[blockSize-1] */
    /* C = A[0] + A[1] + ... + A[blockSize-1] */

    in = *pSrc++ >> 8U;
    /* Compute sum of squares and store result in a temporary variable, sumOfSquares. */
    sumOfSquares += ((q63_t)(in) * (in));
    /* Compute sum and store result in a temporary variable, sum. */
    sum += in;

    /* Decrement loop counter */
    blkCnt--;
  }
  /* Compute Mean of squares and store result in a temporary variable, meanOfSquares. */
  meanOfSquares = (sumOfSquares / (q63_t)(blockSize - 1U));

  /* Compute square of mean */
  squareOfMean = (sum * sum / (q63_t)(blockSize * (blockSize - 1U)));

  /* Compute standard deviation and store result in destination */
  riscv_sqrt_q31((meanOfSquares - squareOfMean) >> 15U, pResult);
}

void ref_std_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult)
{
    uint32_t i;
    q31_t sum = 0;
    q63_t sumsq = 0;

    if (blockSize == 1) {
        *pResult = 0;
        return;
    }

    for (i = 0; i < blockSize; i++) {
        sum += pSrc[i];
        sumsq += (q63_t)pSrc[i] * pSrc[i];
    }
    sumsq /= (q63_t)(blockSize - 1);
    sum = (q31_t)((q63_t)sum * sum / (q63_t)(blockSize * (blockSize - 1)));
    riscv_sqrt_q15(ref_sat_q15((sumsq - sum) >> 15), pResult);
}

void ref_var_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    uint32_t i;
    float32_t sum = 0, sumsq = 0;

    if (blockSize == 1) {
        *pResult = 0;
        return;
    }

    for (i = 0; i < blockSize; i++) {
        sum += pSrc[i];
        sumsq += pSrc[i] * pSrc[i];
    }
    *pResult =
        (sumsq - sum * sum / (float32_t)blockSize) / ((float32_t)blockSize - 1);
}

void ref_var_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult)
{
    uint32_t i;
    q63_t sum = 0, sumsq = 0;
    q31_t in;

    if (blockSize == 1) {
        *pResult = 0;
        return;
    }

    for (i = 0; i < blockSize; i++) {
        in = pSrc[i] >> 8;
        sum += in;
        sumsq += (q63_t)in * in;
    }
    *pResult =
        (sumsq - sum * sum / (q31_t)blockSize) / ((q31_t)blockSize - 1) >> 15;
}

void ref_var_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult)
{
    uint32_t i;
    q31_t sum = 0;
    q63_t sumsq = 0;

    if (blockSize == 1) {
        *pResult = 0;
        return;
    }

    for (i = 0; i < blockSize; i++) {
        sum += pSrc[i];
        sumsq += (q63_t)pSrc[i] * pSrc[i];
    }
    *pResult = (q31_t)((sumsq - (q63_t)sum * sum / (q63_t)blockSize) /
                       ((q63_t)blockSize - 1)) >> 15;
}
