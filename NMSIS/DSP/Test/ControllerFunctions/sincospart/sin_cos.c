#include "ref.h"
#include "riscv_common_tables.h"

void ref_sin_cos_f32(float32_t theta, float32_t *pSinVal, float32_t *pCosVal)
{
    // theta is given in degrees
    *pSinVal = sinf(theta * 6.28318530717959f / 360.0f);
    *pCosVal = cosf(theta * 6.28318530717959f / 360.0f);
}

void ref_sin_cos_q31(q31_t theta, q31_t *pSinVal, q31_t *pCosVal)
{
  q31_t fract;                                     /* Temporary input, output variables */
  uint16_t indexS, indexC;                         /* Index variable */
  q31_t f1, f2, d1, d2;                            /* Two nearest output values */
  q31_t Dn, Df;
  q63_t temp;

  /* Calculate the nearest index */
  indexS = (uint32_t)theta >> CONTROLLER_Q31_SHIFT;
  indexC = (indexS + 128) & 0x1ff;

  /* Calculation of fractional value */
  fract = (theta - (indexS << CONTROLLER_Q31_SHIFT)) << 8;

  /* Read two nearest values of input value from the cos & sin tables */
  f1 =  sinTable_q31[indexC  ];
  f2 =  sinTable_q31[indexC + 1];
  d1 = -sinTable_q31[indexS  ];
  d2 = -sinTable_q31[indexS + 1];

  Dn = 0x1921FB5; /* delta between the two points (fixed), in this case 2*pi/FAST_MATH_TABLE_SIZE */
  Df = f2 - f1;   /* delta between the values of the functions */

  temp = Dn * ((q63_t)d1 + d2);
  temp = temp - ((q63_t)Df << 32);
  temp = (q63_t)fract * (temp >> 31);
  temp = temp + ((3 * (q63_t)Df << 31) - (d2 + ((q63_t)d1 << 1)) * Dn);
  temp = (q63_t)fract * (temp >> 31);
  temp = temp + (q63_t)d1 * Dn;
  temp = (q63_t)fract * (temp >> 31);

  /* Calculation of cosine value */
  *pCosVal = clip_q63_to_q31((temp >> 31) + (q63_t)f1);

  /* Read two nearest values of input value from the cos & sin tables */
  f1 = sinTable_q31[indexS ];
  f2 = sinTable_q31[indexS + 1];
  d1 = sinTable_q31[indexC ];
  d2 = sinTable_q31[indexC + 1];

  Df = f2 - f1; // delta between the values of the functions
  temp = Dn * ((q63_t)d1 + d2);
  temp = temp - ((q63_t)Df << 32);
  temp = (q63_t)fract * (temp >> 31);
  temp = temp + ((3 * (q63_t)Df << 31) - (d2 + ((q63_t)d1 << 1)) * Dn);
  temp = (q63_t)fract * (temp >> 31);
  temp = temp + (q63_t)d1 * Dn;
  temp = (q63_t)fract * (temp >> 31);

  /* Calculation of sine value */
  *pSinVal = clip_q63_to_q31((temp >> 31) + (q63_t)f1);
}
