#include "ref.h"
#include "riscv_common_tables.h"

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
