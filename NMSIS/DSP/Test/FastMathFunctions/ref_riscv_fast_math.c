#include "ref.h"
#include "riscv_common_tables.h"
#include <stdlib.h>

#define Q28QUARTER 0x20000000
#define Q12QUARTER 0x2000
riscv_status ref_sqrt_q31(q31_t in, q31_t *pOut)
{
  q31_t number, var1, signBits1, temp;

  number = in;

  /* If the input is a positive number then compute the signBits. */
  if (number > 0)
  {
    signBits1 = __CLZ(number) - 1;

    /* Shift by the number of signBits1 */
    if ((signBits1 % 2) == 0)
    {
      number = number << signBits1;
    }
    else
    {
      number = number << (signBits1 - 1);
    }

    /* Start value for 1/sqrt(x) for the Newton iteration */
    var1 = sqrt_initial_lut_q31[(number >> 26) - (Q28QUARTER >> 26)];

    /* 0.5 var1 * (3 - number * var1 * var1) */

    /* 1st iteration */

    temp = ((q63_t) var1 * var1) >> 28;
    temp = ((q63_t) number * temp) >> 31;
    temp = 0x30000000 - temp;
    var1 = ((q63_t) var1 * temp) >> 29;


    /* 2nd iteration */
    temp = ((q63_t) var1 * var1) >> 28;
    temp = ((q63_t) number * temp) >> 31;
    temp = 0x30000000 - temp;
    var1 = ((q63_t) var1 * temp) >> 29;

    /* 3nd iteration */
    temp = ((q63_t) var1 * var1) >> 28;
    temp = ((q63_t) number * temp) >> 31;
    temp = 0x30000000 - temp;
    var1 = ((q63_t) var1 * temp) >> 29;

    /* Multiply the inverse square root with the original value */
    var1 = ((q31_t) (((q63_t) number * var1) >> 28));

    /* Shift the output down accordingly */
    if ((signBits1 % 2) == 0)
    {
      var1 = var1 >> (signBits1 / 2);
    }
    else
    {
      var1 = var1 >> ((signBits1 - 1) / 2);
    }
    *pOut = var1;

    return (RISCV_MATH_SUCCESS);
  }
  /* If the number is a negative number then store zero as its square root value */
  else
  {
    *pOut = 0;

    return (RISCV_MATH_ARGUMENT_ERROR);
  }
}

riscv_status ref_sqrt_q15(q15_t in, q15_t *pOut)
{
  q15_t number, var1, signBits1,temp;

  number = in;

  /* If the input is a positive number then compute the signBits. */
  if (number > 0)
  {
    signBits1 = __CLZ(number) - 17;

    /* Shift by the number of signBits1 */
    if ((signBits1 % 2) == 0)
    {
      number = number << signBits1;
    }
    else
    {
      number = number << (signBits1 - 1);
    }
    /* Start value for 1/sqrt(x) for the Newton iteration */
    var1 = sqrt_initial_lut_q15[(number>> 11) - (Q12QUARTER >> 11)];

    /* 0.5 var1 * (3 - number * var1 * var1) */
    /* 1st iteration */

   temp = ((q31_t) var1 * var1) >> 12;
   temp = ((q31_t) number * temp) >> 15;
   temp = 0x3000 - temp;
   var1 = ((q31_t) var1 * temp) >> 13;

   temp = ((q31_t) var1 * var1) >> 12;
   temp = ((q31_t) number * temp) >> 15;
   temp = 0x3000 - temp;
   var1 = ((q31_t) var1 * temp) >> 13;

   temp = ((q31_t) var1 * var1) >> 12;
   temp = ((q31_t) number * temp) >> 15;
   temp = 0x3000 - temp;
   var1 = ((q31_t) var1 * temp) >> 13;

    /* Multiply the inverse square root with the original value */

    var1 = ((q15_t) (((q31_t) number * var1) >> 12));

    /* Shift the output down accordingly */
    if ((signBits1 % 2) == 0)
    {
      var1 = var1 >> (signBits1 / 2);
    }
    else
    {
      var1 = var1 >> ((signBits1 - 1) / 2);
    }
    *pOut = var1;


    return (RISCV_MATH_SUCCESS);
  }
  /* If the number is a negative number then store zero as its square root value */
  else
  {
    *pOut = 0;

    return (RISCV_MATH_ARGUMENT_ERROR);
  }
}

riscv_status ref_divide_q15(q15_t numerator,
  q15_t denominator,
  q15_t *quotient,
  int16_t *shift)
{
  int16_t sign=0;
  q31_t temp;
  int16_t shiftForNormalizing;

  *shift = 0;

  sign = (numerator>>15) ^ (denominator>>15);

  if (denominator == 0)
  {
     if (sign)
     {
        *quotient = 0x8000;
     }
     else
     {
        *quotient = 0x7FFF;
     }
     return(RISCV_MATH_NANINF);
  }

  numerator = abs(numerator);
  denominator = abs(denominator);

  temp = ((q31_t)numerator << 15) / ((q31_t)denominator);

  shiftForNormalizing= 17 - __CLZ(temp);
  if (shiftForNormalizing > 0)
  {
     *shift = shiftForNormalizing;
     temp = temp >> shiftForNormalizing;
  }

  if (sign)
  {
    temp = -temp;
  }

  *quotient=temp;

  return(RISCV_MATH_SUCCESS);
}

void ref_vexp_f32(
  const float32_t * pSrc,
        float32_t * pDst,
        uint32_t blockSize)
{
   uint32_t blkCnt;

   blkCnt = blockSize;

   while (blkCnt > 0U)
   {
      /* C = log(A) */

      /* Calculate log and store result in destination buffer. */
      *pDst++ = expf(*pSrc++);

      /* Decrement loop counter */
      blkCnt--;
   }
}

void ref_vlog_f32(
  const float32_t * pSrc,
        float32_t * pDst,
        uint32_t blockSize)
{
   uint32_t blkCnt;

   blkCnt = blockSize;

   while (blkCnt > 0U)
   {
      /* C = log(A) */

      /* Calculate log and store result in destination buffer. */
      *pDst++ = logf(*pSrc++);

      /* Decrement loop counter */
      blkCnt--;
   }
}

float32_t ref_sin_f32(
  float32_t x)
{
  float32_t sinVal, fract, in;                   /* Temporary input, output variables */
  uint16_t index;                                /* Index variable */
  float32_t a, b;                                /* Two nearest output values */
  int32_t n;
  float32_t findex;

  /* input x is in radians */
  /* Scale input to [0 1] range from [0 2*PI] , divide input by 2*pi */
  in = x * 0.159154943092f;

  /* Calculation of floor value of input */
  n = (int32_t) in;

  /* Make negative values towards -infinity */
  if (in < 0.0f)
  {
    n--;
  }

  /* Map input value to [0 1] */
  in = in - (float32_t) n;

  /* Calculation of index of the table */
  findex = (float32_t)FAST_MATH_TABLE_SIZE * in;
  index = (uint16_t)findex;

  /* when "in" is exactly 1, we need to rotate the index down to 0 */
  if (index >= FAST_MATH_TABLE_SIZE) {
    index = 0;
    findex -= (float32_t)FAST_MATH_TABLE_SIZE;
  }

  /* fractional value calculation */
  fract = findex - (float32_t) index;

  /* Read two nearest values of input value from the sin table */
  a = sinTable_f32[index];
  b = sinTable_f32[index + 1];

  /* Linear interpolation process */
  sinVal = (1.0f - fract) * a + fract * b;

  /* Return output value */
  return (sinVal);
}

q31_t ref_sin_q31(q31_t x)
{
  q31_t sinVal;                                  /* Temporary variables for input, output */
  int32_t index;                                 /* Index variable */
  q31_t a, b;                                    /* Two nearest output values */
  q31_t fract;                                   /* Temporary values for fractional values */

  if (x < 0)
  { /* convert negative numbers to corresponding positive ones */
    x = (uint32_t)x + 0x80000000;
  }

  /* Calculate the nearest index */
  index = (uint32_t)x >> FAST_MATH_Q31_SHIFT;

  /* Calculation of fractional value */
  fract = (x - (index << FAST_MATH_Q31_SHIFT)) << 9;

  /* Read two nearest values of input value from the sin table */
  a = sinTable_q31[index];
  b = sinTable_q31[index + 1];

  /* Linear interpolation process */
  sinVal = (q63_t) (0x80000000 - fract) * a >> 32;
  sinVal = (q31_t) ((((q63_t) sinVal << 32) + ((q63_t) fract * b)) >> 32);

  /* Return output value */
  return (sinVal << 1);
}

q15_t ref_sin_q15(q15_t x)
{
  q15_t sinVal;                                  /* Temporary input, output variables */
  int32_t index;                                 /* Index variable */
  q15_t a, b;                                    /* Two nearest output values */
  q15_t fract;                                   /* Temporary values for fractional values */


  if (x < 0)
  { /* convert negative numbers to corresponding positive ones */
    x = (uint16_t)x + 0x8000;
  }

  /* Calculate the nearest index */
  index = (uint32_t)x >> FAST_MATH_Q15_SHIFT;

  /* Calculation of fractional value */
  fract = (x - (index << FAST_MATH_Q15_SHIFT)) << 9;

  /* Read two nearest values of input value from the sin table */
  a = sinTable_q15[index];
  b = sinTable_q15[index + 1];

  /* Linear interpolation process */
  sinVal = (q31_t) (0x8000 - fract) * a >> 16;
  sinVal = (q15_t) ((((q31_t) sinVal << 16) + ((q31_t) fract * b)) >> 16);

  /* Return output value */
  return (sinVal << 1);

}

float32_t ref_cos_f32(
  float32_t x)
{
  float32_t cosVal, fract, in;                   /* Temporary input, output variables */
  uint16_t index;                                /* Index variable */
  float32_t a, b;                                /* Two nearest output values */
  int32_t n;
  float32_t findex;

  /* input x is in radians */
  /* Scale input to [0 1] range from [0 2*PI] , divide input by 2*pi, add 0.25 (pi/2) to read sine table */
  in = x * 0.159154943092f + 0.25f;

  /* Calculation of floor value of input */
  n = (int32_t) in;

  /* Make negative values towards -infinity */
  if (in < 0.0f)
  {
    n--;
  }

  /* Map input value to [0 1] */
  in = in - (float32_t) n;

  /* Calculation of index of the table */
  findex = (float32_t)FAST_MATH_TABLE_SIZE * in;
  index = (uint16_t)findex;

  /* when "in" is exactly 1, we need to rotate the index down to 0 */
  if (index >= FAST_MATH_TABLE_SIZE) {
    index = 0;
    findex -= (float32_t)FAST_MATH_TABLE_SIZE;
  }

  /* fractional value calculation */
  fract = findex - (float32_t) index;

  /* Read two nearest values of input value from the cos table */
  a = sinTable_f32[index];
  b = sinTable_f32[index + 1];

  /* Linear interpolation process */
  cosVal = (1.0f - fract) * a + fract * b;

  /* Return output value */
  return (cosVal);
}

q31_t ref_cos_q31(q31_t x)
{
  q31_t cosVal;                                  /* Temporary input, output variables */
  int32_t index;                                 /* Index variable */
  q31_t a, b;                                    /* Two nearest output values */
  q31_t fract;                                   /* Temporary values for fractional values */

  /* add 0.25 (pi/2) to read sine table */
  x = (uint32_t)x + 0x20000000;
  if (x < 0)
  { /* convert negative numbers to corresponding positive ones */
    x = (uint32_t)x + 0x80000000;
  }

  /* Calculate the nearest index */
  index = (uint32_t)x >> FAST_MATH_Q31_SHIFT;

  /* Calculation of fractional value */
  fract = (x - (index << FAST_MATH_Q31_SHIFT)) << 9;

  /* Read two nearest values of input value from the sin table */
  a = sinTable_q31[index];
  b = sinTable_q31[index + 1];

  /* Linear interpolation process */
  cosVal = (q63_t) (0x80000000 - fract) * a >> 32;
  cosVal = (q31_t) ((((q63_t) cosVal << 32) + ((q63_t) fract * b)) >> 32);

  /* Return output value */
  return (cosVal << 1);
}

q15_t ref_cos_q15(q15_t x)
{
  q15_t cosVal;                                  /* Temporary input, output variables */
  int32_t index;                                 /* Index variable */
  q15_t a, b;                                    /* Two nearest output values */
  q15_t fract;                                   /* Temporary values for fractional values */

  /* add 0.25 (pi/2) to read sine table */
  x = (uint16_t)x + 0x2000;
  if (x < 0)
  { /* convert negative numbers to corresponding positive ones */
    x = (uint16_t)x + 0x8000;
  }

  /* Calculate the nearest index */
  index = (uint32_t)x >> FAST_MATH_Q15_SHIFT;

  /* Calculation of fractional value */
  fract = (x - (index << FAST_MATH_Q15_SHIFT)) << 9;

  /* Read two nearest values of input value from the sin table */
  a = sinTable_q15[index];
  b = sinTable_q15[index + 1];

  /* Linear interpolation process */
  cosVal = (q31_t) (0x8000 - fract) * a >> 16;
  cosVal = (q15_t) ((((q31_t) cosVal << 16) + ((q31_t) fract * b)) >> 16);

  /* Return output value */
  return (cosVal << 1);
}