#include "ref.h"

void ref_quaternion_conjugate_f32(const float32_t *pInputQuaternions,
    float32_t *pConjugateQuaternions,
    uint32_t nbQuaternions)
{
   for(uint32_t i=0; i < nbQuaternions; i++)
   {

      pConjugateQuaternions[4 * i + 0] = pInputQuaternions[4 * i + 0];
      pConjugateQuaternions[4 * i + 1] = -pInputQuaternions[4 * i + 1];
      pConjugateQuaternions[4 * i + 2] = -pInputQuaternions[4 * i + 2];
      pConjugateQuaternions[4 * i + 3] = -pInputQuaternions[4 * i + 3];
   }
}


void ref_quaternion_inverse_f32(const float32_t *pInputQuaternions,
  float32_t *pInverseQuaternions,
  uint32_t nbQuaternions)
{
   float32_t temp;

   for(uint32_t i=0; i < nbQuaternions; i++)
   {

      temp = SQ(pInputQuaternions[4 * i + 0]) +
             SQ(pInputQuaternions[4 * i + 1]) +
             SQ(pInputQuaternions[4 * i + 2]) +
             SQ(pInputQuaternions[4 * i + 3]);

      pInverseQuaternions[4 * i + 0] = pInputQuaternions[4 * i + 0] / temp;
      pInverseQuaternions[4 * i + 1] = -pInputQuaternions[4 * i + 1] / temp;
      pInverseQuaternions[4 * i + 2] = -pInputQuaternions[4 * i + 2] / temp;
      pInverseQuaternions[4 * i + 3] = -pInputQuaternions[4 * i + 3] / temp;
   }
}

void ref_quaternion_norm_f32(const float32_t *pInputQuaternions,
  float32_t *pNorms,
  uint32_t nbQuaternions)
{
   float32_t temp;

   for(uint32_t i=0; i < nbQuaternions; i++)
   {
      temp = SQ(pInputQuaternions[4 * i + 0]) +
             SQ(pInputQuaternions[4 * i + 1]) +
             SQ(pInputQuaternions[4 * i + 2]) +
             SQ(pInputQuaternions[4 * i + 3]);
      pNorms[i] = sqrtf(temp);
   }
}

void ref_quaternion_normalize_f32(const float32_t *pInputQuaternions,
    float32_t *pNormalizedQuaternions,
    uint32_t nbQuaternions)
{
   float32_t temp;

   for(uint32_t i=0; i < nbQuaternions; i++)
   {
      temp = SQ(pInputQuaternions[4 * i + 0]) +
             SQ(pInputQuaternions[4 * i + 1]) +
             SQ(pInputQuaternions[4 * i + 2]) +
             SQ(pInputQuaternions[4 * i + 3]);
      temp = sqrtf(temp);

      pNormalizedQuaternions[4 * i + 0] = pInputQuaternions[4 * i + 0] / temp;
      pNormalizedQuaternions[4 * i + 1] = pInputQuaternions[4 * i + 1] / temp;
      pNormalizedQuaternions[4 * i + 2] = pInputQuaternions[4 * i + 2] / temp;
      pNormalizedQuaternions[4 * i + 3] = pInputQuaternions[4 * i + 3] / temp;
   }
}

void ref_quaternion_product_f32(const float32_t *qa,
    const float32_t *qb,
    float32_t *qr,
    uint32_t nbQuaternions)
{
   for(uint32_t i=0; i < nbQuaternions; i++)
   {
     ref_quaternion_product_single_f32(qa, qb, qr);

     qa += 4;
     qb += 4;
     qr += 4;
   }
}

void ref_quaternion_product_single_f32(const float32_t *qa,
    const float32_t *qb,
    float32_t *qr)
{
    qr[0] = qa[0] * qb[0] - qa[1] * qb[1] - qa[2] * qb[2] - qa[3] * qb[3];
    qr[1] = qa[0] * qb[1] + qa[1] * qb[0] + qa[2] * qb[3] - qa[3] * qb[2];
    qr[2] = qa[0] * qb[2] + qa[2] * qb[0] + qa[3] * qb[1] - qa[1] * qb[3];
    qr[3] = qa[0] * qb[3] + qa[3] * qb[0] + qa[1] * qb[2] - qa[2] * qb[1];
}

void ref_quaternion2rotation_f32(const float32_t *pInputQuaternions,
    float32_t *pOutputRotations,
    uint32_t nbQuaternions)
{
   for(uint32_t nb=0; nb < nbQuaternions; nb++)
   {
        float32_t q00 = SQ(pInputQuaternions[0 + nb * 4]);
        float32_t q11 = SQ(pInputQuaternions[1 + nb * 4]);
        float32_t q22 = SQ(pInputQuaternions[2 + nb * 4]);
        float32_t q33 = SQ(pInputQuaternions[3 + nb * 4]);
        float32_t q01 =  pInputQuaternions[0 + nb * 4]*pInputQuaternions[1 + nb * 4];
        float32_t q02 =  pInputQuaternions[0 + nb * 4]*pInputQuaternions[2 + nb * 4];
        float32_t q03 =  pInputQuaternions[0 + nb * 4]*pInputQuaternions[3 + nb * 4];
        float32_t q12 =  pInputQuaternions[1 + nb * 4]*pInputQuaternions[2 + nb * 4];
        float32_t q13 =  pInputQuaternions[1 + nb * 4]*pInputQuaternions[3 + nb * 4];
        float32_t q23 =  pInputQuaternions[2 + nb * 4]*pInputQuaternions[3 + nb * 4];

        float32_t xx = q00 + q11 - q22 - q33;
        float32_t yy = q00 - q11 + q22 - q33;
        float32_t zz = q00 - q11 - q22 + q33;
        float32_t xy = 2*(q12 - q03);
        float32_t xz = 2*(q13 + q02);
        float32_t yx = 2*(q12 + q03);
        float32_t yz = 2*(q23 - q01);
        float32_t zx = 2*(q13 - q02);
        float32_t zy = 2*(q23 + q01);

        pOutputRotations[0 + nb * 9] = xx; pOutputRotations[1 + nb * 9] = xy; pOutputRotations[2 + nb * 9] = xz;
        pOutputRotations[3 + nb * 9] = yx; pOutputRotations[4 + nb * 9] = yy; pOutputRotations[5 + nb * 9] = yz;
        pOutputRotations[6 + nb * 9] = zx; pOutputRotations[7 + nb * 9] = zy; pOutputRotations[8 + nb * 9] = zz;
   }
}

#define RI(x,y) r[(3*(x) + (y))]

void ref_rotation2quaternion_f32(const float32_t *pInputRotations,
    float32_t *pOutputQuaternions,
    uint32_t nbQuaternions)
{
   for(uint32_t nb=0; nb < nbQuaternions; nb++)
   {
       const float32_t *r=&pInputRotations[nb*9];
       float32_t *q=&pOutputQuaternions[nb*4];

       float32_t trace = RI(0,0) + RI(1,1) + RI(2,2);

       float32_t doubler;
       float32_t s;

      if (trace > 0)
      {
        doubler = sqrtf(trace + 1.0) * 2; // invs=4*qw
        s = 1.0 / doubler;
        q[0] = 0.25 * doubler;
        q[1] = (RI(2,1) - RI(1,2)) * s;
        q[2] = (RI(0,2) - RI(2,0)) * s;
        q[3] = (RI(1,0) - RI(0,1)) * s;
      }
      else if ((RI(0,0) > RI(1,1)) && (RI(0,0) > RI(2,2)) )
      {
        doubler = sqrtf(1.0 + RI(0,0) - RI(1,1) - RI(2,2)) * 2; // invs=4*qx
        s = 1.0 / doubler;
        q[0] = (RI(2,1) - RI(1,2)) * s;
        q[1] = 0.25 * doubler;
        q[2] = (RI(0,1) + RI(1,0)) * s;
        q[3] = (RI(0,2) + RI(2,0)) * s;
      }
      else if (RI(1,1) > RI(2,2))
      {
        doubler = sqrtf(1.0 + RI(1,1) - RI(0,0) - RI(2,2)) * 2; // invs=4*qy
        s = 1.0 / doubler;
        q[0] = (RI(0,2) - RI(2,0)) * s;
        q[1] = (RI(0,1) + RI(1,0)) * s;
        q[2] = 0.25 * doubler;
        q[3] = (RI(1,2) + RI(2,1)) * s;
      }
      else
      {
        doubler = sqrtf(1.0 + RI(2,2) - RI(0,0) - RI(1,1)) * 2; // invs=4*qz
        s = 1.0 / doubler;
        q[0] = (RI(1,0) - RI(0,1)) * s;
        q[1] = (RI(0,2) + RI(2,0)) * s;
        q[2] = (RI(1,2) + RI(2,1)) * s;
        q[3] = 0.25 * doubler;
      }

    }
}