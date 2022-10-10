#include "ref.h"
#include "riscv_common_tables.h"

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
    var1 = sqrt_initial_lut_q15[(number >> 11) - (Q12QUARTER >> 11)];

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
