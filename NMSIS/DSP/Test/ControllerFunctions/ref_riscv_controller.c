#include "ref.h"
#include "riscv_common_tables.h"

float32_t ref_pid_f32(riscv_pid_instance_f32 *S, float32_t in)
{
    float32_t out;

    /* y[n] = y[n-1] + A0 * x[n] + A1 * x[n-1] + A2 * x[n-2]  */
    out = (S->A0 * in) + (S->A1 * S->state[0]) + (S->A2 * S->state[1]) + (S->state[2]);

    /* Update state */
    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    /* return to application */
    return (out);
}

q31_t ref_pid_q31(riscv_pid_instance_q31 *S, q31_t in)
{
    q63_t acc;
    q31_t out;

    /* acc = A0 * x[n]  */
    acc = (q63_t)S->A0 * in;

    /* acc += A1 * x[n-1] */
    acc += (q63_t)S->A1 * S->state[0];

    /* acc += A2 * x[n-2]  */
    acc += (q63_t)S->A2 * S->state[1];

    /* convert output to 1.31 format to add y[n-1] */
    out = (q31_t)(acc >> 31U);

    /* out += y[n-1] */
    out += S->state[2];

    /* Update state */
    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    /* return to application */
    return (out);
}

q15_t ref_pid_q15(riscv_pid_instance_q15 *S, q15_t in)
{
    q63_t acc;
    q15_t out;
    q15_t A1, A2;

#if defined(riscv_MATH_DSP)

    A2 = S->A1 >> 16;
    A1 = (q15_t)S->A1;

#else

    A1 = S->A1;
    A2 = S->Kd;

#endif

    /* acc = A0 * x[n]  */
    acc = ((q31_t)S->A0) * in;

    /* acc += A1 * x[n-1] + A2 * x[n-2]  */
    acc += (q31_t)A1 * S->state[0];
    acc += (q31_t)A2 * S->state[1];

    /* acc += y[n-1] */
    acc += (q31_t)S->state[2] << 15;

    /* saturate the output */
    out = ref_sat_q15((q31_t)acc >> 15);

    /* Update state */
    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    /* return to application */
    return (out);
}

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
