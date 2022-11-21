#include "ref.h"

void ref_abs_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] < 0 ? -pSrc[i] : pSrc[i];
    }
}

void ref_abs_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] < 0 ? -pSrc[i] : pSrc[i];
    }
}

void ref_abs_q15(q15_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] < 0 ? -pSrc[i] : pSrc[i];
    }
}

void ref_abs_q7(q7_t *pSrc, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        if (pSrc[i] == -128)
            pDst[i] = 127;
        else
            pDst[i] = pSrc[i] < 0 ? -pSrc[i] : pSrc[i];
    }
}


void ref_add_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                 uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrcA[i] + pSrcB[i];
    }
}

void ref_add_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q31((q63_t)pSrcA[i] + pSrcB[i]);
    }
}

void ref_add_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q15((q31_t)pSrcA[i] + pSrcB[i]);
    }
}

void ref_add_q7(q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q7((q15_t)pSrcA[i] + pSrcB[i]);
    }
}

void ref_dot_prod_f32(float32_t *pSrcA, float32_t *pSrcB, uint32_t blockSize,
                      float32_t *result)
{
    uint32_t i;
    float32_t sum = 0.0f;

    for (i = 0; i < blockSize; i++) {
        sum += pSrcA[i] * pSrcB[i];
    }
    *result = sum;
}

void ref_dot_prod_q31(q31_t *pSrcA, q31_t *pSrcB, uint32_t blockSize,
                      q63_t *result)
{
    uint32_t i;
    q63_t sum = 0.0f;

    for (i = 0; i < blockSize; i++) {
        sum += ((q63_t)pSrcA[i] * pSrcB[i]) >> 14; // 16.48
    }
    *result = sum;
}

void ref_dot_prod_q15(q15_t *pSrcA, q15_t *pSrcB, uint32_t blockSize,
                      q63_t *result)
{
    uint32_t i;
    q63_t sum = 0.0f;

    for (i = 0; i < blockSize; i++) {
        sum += (q31_t)pSrcA[i] * pSrcB[i]; // 34.30
    }
    *result = sum;
}

void ref_dot_prod_q7(q7_t *pSrcA, q7_t *pSrcB, uint32_t blockSize,
                     q31_t *result)
{
    uint32_t i;
    q31_t sum = 0.0f;

    for (i = 0; i < blockSize; i++) {
        sum += (q31_t)pSrcA[i] * pSrcB[i]; // 18.14
    }
    *result = sum;
}



void ref_mult_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                  uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrcA[i] * pSrcB[i];
    }
}

void ref_mult_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    q63_t temp;

    for (i = 0; i < blockSize; i++) {
        temp = ((q63_t)pSrcA[i] * pSrcB[i]) >> 32;
        temp = temp << 1;
        pDst[i] = ref_sat_q31(temp);
    }
}

void ref_mult_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    q31_t temp;

    for (i = 0; i < blockSize; i++) {
        temp = ((q31_t)pSrcA[i] * pSrcB[i]) >>
               15; // this comment is for JD, this is specifically 15 and not 16
                   // like the q31 case might imply.  This is because NMSIS DSP
                   // lib does it this way.  No other reason.
        pDst[i] = ref_sat_q15(temp);
    }
}

void ref_mult_q7(q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    q15_t temp;

    for (i = 0; i < blockSize; i++) {
        temp = ((q15_t)pSrcA[i] * pSrcB[i]) >> 7;
        pDst[i] = ref_sat_q7(temp);
    }
}

void ref_negate_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = -pSrc[i];
    }
}

void ref_negate_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = -pSrc[i];
    }
}

void ref_negate_q15(q15_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = -pSrc[i];
    }
}

void ref_negate_q7(q7_t *pSrc, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = -pSrc[i];
    }
}

void ref_offset_f32(float32_t *pSrc, float32_t offset, float32_t *pDst,
                    uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] + offset;
    }
}

void ref_offset_q31(q31_t *pSrc, q31_t offset, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q31((q63_t)pSrc[i] + offset);
    }
}

void ref_offset_q15(q15_t *pSrc, q15_t offset, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q15((q31_t)pSrc[i] + offset);
    }
}

void ref_offset_q7(q7_t *pSrc, q7_t offset, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q7((q15_t)pSrc[i] + offset);
    }
}

void ref_scale_f32(float32_t *pSrc, float32_t scale, float32_t *pDst,
                   uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] * scale;
    }
}

void ref_scale_q31(q31_t *pSrc, q31_t scaleFract, int8_t shift, q31_t *pDst,
                   uint32_t blockSize)
{
    uint32_t i;
    int8_t kShift = shift + 1; /* Shift to apply after scaling */
    int8_t sign = (kShift & 0x80);
    q63_t temp;

    for (i = 0; i < blockSize; i++) {
        temp = ((q63_t)pSrc[i] * scaleFract) >> 32;
        if (sign)
            pDst[i] = temp >> -kShift;
        else
            pDst[i] = ref_sat_q31((q63_t)temp << kShift);
    }
}

void ref_scale_q15(q15_t *pSrc, q15_t scaleFract, int8_t shift, q15_t *pDst,
                   uint32_t blockSize)
{
    uint32_t i;
    int8_t kShift = 15 - shift; /* Shift to apply after scaling */

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q15(((q31_t)pSrc[i] * scaleFract) >> kShift);
    }
}

void ref_scale_q7(q7_t *pSrc, q7_t scaleFract, int8_t shift, q7_t *pDst,
                  uint32_t blockSize)
{
    uint32_t i;
    int8_t kShift = 7 - shift; /* Shift to apply after scaling */

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q7(((q15_t)pSrc[i] * scaleFract) >> kShift);
    }
}

void ref_shift_q31(q31_t *pSrc, int8_t shiftBits, q31_t *pDst,
                   uint32_t blockSize)
{
    uint32_t i;

    if (shiftBits >= 0) {
        for (i = 0; i < blockSize; i++) {
            pDst[i] = pSrc[i] << shiftBits;
        }
    } else {
        for (i = 0; i < blockSize; i++) {
            pDst[i] = pSrc[i] >> -shiftBits;
        }
    }
}

void ref_shift_q15(q15_t *pSrc, int8_t shiftBits, q15_t *pDst,
                   uint32_t blockSize)
{
    uint32_t i;

    if (shiftBits >= 0) {
        for (i = 0; i < blockSize; i++) {
            pDst[i] = pSrc[i] << shiftBits;
        }
    } else {
        for (i = 0; i < blockSize; i++) {
            pDst[i] = pSrc[i] >> -shiftBits;
        }
    }
}

void ref_shift_q7(q7_t *pSrc, int8_t shiftBits, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    if (shiftBits >= 0) {
        for (i = 0; i < blockSize; i++) {
            pDst[i] = pSrc[i] << shiftBits;
        }
    } else {
        for (i = 0; i < blockSize; i++) {
            pDst[i] = pSrc[i] >> -shiftBits;
        }
    }
}

void ref_sub_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                 uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = pSrcA[i] - pSrcB[i];
    }
}

void ref_sub_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q31((q63_t)pSrcA[i] - pSrcB[i]);
    }
}

void ref_sub_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q15((q31_t)pSrcA[i] - pSrcB[i]);
    }
}

void ref_sub_q7(q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize)
{
    uint32_t i;

    for (i = 0; i < blockSize; i++) {
        pDst[i] = ref_sat_q7((q15_t)pSrcA[i] - pSrcB[i]);
    }
}

void ref_clip_f32(const float32_t * pSrc,
  float32_t * pDst,
  float32_t low,
  float32_t high,
  uint32_t numSamples)
{
    for (uint32_t i = 0; i < numSamples; i++)
    {
        if (pSrc[i] > high)
            pDst[i] = high;
        else if (pSrc[i] < low)
            pDst[i] = low;
        else
            pDst[i] = pSrc[i];
    }
}

void ref_clip_q7(const q7_t * pSrc,
  q7_t * pDst,
  q7_t low,
  q7_t high,
  uint32_t numSamples)
{
    for (uint32_t i = 0; i < numSamples; i++)
    {
        if (pSrc[i] > high)
            pDst[i] = high;
        else if (pSrc[i] < low)
            pDst[i] = low;
        else
            pDst[i] = pSrc[i];
    }
}

void ref_clip_q15(const q15_t * pSrc,
  q15_t * pDst,
  q15_t low,
  q15_t high,
  uint32_t numSamples)
{
    for (uint32_t i = 0; i < numSamples; i++)
    {
        if (pSrc[i] > high)
            pDst[i] = high;
        else if (pSrc[i] < low)
            pDst[i] = low;
        else
            pDst[i] = pSrc[i];
    }
}

void ref_clip_q31(const q31_t * pSrc,
  q31_t * pDst,
  q31_t low,
  q31_t high,
  uint32_t numSamples)
{
    for (uint32_t i = 0; i < numSamples; i++)
    {
        if (pSrc[i] > high)
            pDst[i] = high;
        else if (pSrc[i] < low)
            pDst[i] = low;
        else
            pDst[i] = pSrc[i];
    }
}

void ref_and_u16(
    const uint16_t * pSrcA,
    const uint16_t * pSrcB,
          uint16_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt;      /* Loop counter */
    blkCnt = blockSize;
    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)&(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_and_u32(
    const uint32_t * pSrcA,
    const uint32_t * pSrcB,
          uint32_t * pDst,
          uint32_t blockSize)
{
    /* Initialize blkCnt with number of samples */
    uint32_t blkCnt = blockSize;
    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)&(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_and_u8(
    const uint8_t * pSrcA,
    const uint8_t * pSrcB,
          uint8_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt = blockSize;
    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)&(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_or_u16(
    const uint16_t * pSrcA,
    const uint16_t * pSrcB,
          uint16_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt;      /* Loop counter */
    blkCnt = blockSize;

    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)|(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_or_u32(
    const uint32_t * pSrcA,
    const uint32_t * pSrcB,
          uint32_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt;      /* Loop counter */
    blkCnt = blockSize;
    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)|(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_or_u8(
    const uint8_t * pSrcA,
    const uint8_t * pSrcB,
          uint8_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt;      /* Loop counter */
    blkCnt = blockSize;
    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)|(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_not_u16(
    const uint16_t * pSrc,
          uint16_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt = blockSize;    /* Loop counter */
    blkCnt = blockSize;

    while (blkCnt > 0U)
    {
        *pDst++ = ~(*pSrc++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_not_u32(
    const uint32_t * pSrc,
          uint32_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt;      /* Loop counter */
    blkCnt = blockSize;

    while (blkCnt > 0U)
    {
        *pDst++ = ~(*pSrc++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_not_u8(
    const uint8_t * pSrc,
          uint8_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt;      /* Loop counter */
    blkCnt = blockSize;

    while (blkCnt > 0U)
    {
        *pDst++ = ~(*pSrc++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_xor_u16(
    const uint16_t * pSrcA,
    const uint16_t * pSrcB,
          uint16_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt = blockSize;
    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)^(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_xor_u32(
    const uint32_t * pSrcA,
    const uint32_t * pSrcB,
          uint32_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt = blockSize;

    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)^(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

void ref_xor_u8(
    const uint8_t * pSrcA,
    const uint8_t * pSrcB,
          uint8_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt = blockSize;

    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)^(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}
