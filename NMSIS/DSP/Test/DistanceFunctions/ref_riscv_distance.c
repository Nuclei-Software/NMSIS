#include "ref.h"

float32_t ref_braycurtis_distance_f32(const float32_t *pA, const float32_t *pB, uint32_t blockSize)
{
   float32_t accumDiff=0.0f, accumSum=0.0f, tmpA, tmpB;

   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;
      accumDiff += fabsf(tmpA - tmpB);
      accumSum += fabsf(tmpA + tmpB);
      blockSize --;
   }
   /*

   It is assumed that accumSum is not zero. Since it is the sum of several absolute
   values it would imply that all of them are zero. It is very unlikely for long vectors.

   */
   return(accumDiff / accumSum);
}

float32_t ref_canberra_distance_f32(const float32_t *pA, const float32_t *pB, uint32_t blockSize)
{
   float32_t accum=0.0f, tmpA, tmpB, diff, sum;

   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;

      diff = fabsf(tmpA - tmpB);
      sum = fabsf(tmpA) + fabsf(tmpB);
      if ((tmpA != 0.0f) || (tmpB != 0.0f))
      {
         accum += (diff / sum);
      }
      blockSize --;
   }
   return(accum);
}

float32_t ref_chebyshev_distance_f32(const float32_t *pA, const float32_t *pB, uint32_t blockSize)
{
   float32_t diff=0.0f,  maxVal, tmpA, tmpB;

   tmpA = *pA++;
   tmpB = *pB++;
   diff = fabsf(tmpA - tmpB);
   maxVal = diff;
   blockSize--;

   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;
      diff = fabsf(tmpA - tmpB);
      if (diff > maxVal)
      {
        maxVal = diff;
      }
      blockSize --;
   }

   return(maxVal);
}

float32_t ref_cityblock_distance_f32(const float32_t *pA, const float32_t *pB, uint32_t blockSize)
{
   float32_t accum, tmpA, tmpB;

   accum = 0.0f;
   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;
      accum += fabsf(tmpA - tmpB);

      blockSize --;
   }

   return(accum);
}

float32_t ref_cosine_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize)
{
    float32_t pwra,pwrb,dot,tmp;

    riscv_power_f32(pA, blockSize, &pwra);
    riscv_power_f32(pB, blockSize, &pwrb);

    riscv_dot_prod_f32(pA,pB,blockSize,&dot);

    riscv_sqrt_f32(pwra * pwrb, &tmp);
    return(1.0f - dot / tmp);

}

float32_t ref_euclidean_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize)
{
   float32_t accum=0.0f, tmp;

   while(blockSize > 0)
   {
      tmp = *pA++ - *pB++;
      accum += RISCV_SQ(tmp);
      blockSize --;
   }

   riscv_sqrt_f32(accum,&tmp);
   return(tmp);
}

__STATIC_INLINE float32_t rel_entr(float32_t x, float32_t y)
{
    return (x * logf(x / y));
}

float32_t ref_jensenshannon_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize)
{
    float32_t left, right,sum, result, tmp;
    uint32_t i;

    left = 0.0f;
    right = 0.0f;
    for(i=0; i < blockSize; i++)
    {
      tmp = (pA[i] + pB[i]) / 2.0f;
      left  += rel_entr(pA[i], tmp);
      right += rel_entr(pB[i], tmp);
    }

    sum = left + right;
    riscv_sqrt_f32(sum/2.0f, &result);
    return(result);

}

float32_t ref_minkowski_distance_f32(const float32_t *pA, const float32_t *pB, int32_t order, uint32_t blockSize)
{
    float32_t sum;
    uint32_t i;

    sum = 0.0f;
    for(i=0; i < blockSize; i++)
    {
       sum += powf(fabsf(pA[i] - pB[i]),order);
    }


    return(powf(sum,(1.0f/order)));

}
