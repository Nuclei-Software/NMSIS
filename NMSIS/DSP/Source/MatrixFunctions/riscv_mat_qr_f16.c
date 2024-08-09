/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_qr_f16.c
 * Description:  Half floating-point matrix QR decomposition.
 *
 * $Date:        15 June 2022
 * $Revision:    V1.11.0
 *
 * Target Processor: RISC-V cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dsp/matrix_functions_f16.h"
#include "dsp/matrix_utils.h"

/**
  @ingroup groupMatrix
 */


/**
  @addtogroup MatrixQR
  @{
 */

/**
  @brief         QR decomposition of a m x n half floating point matrix with m >= n.
  @param[in]     pSrc      points to input matrix structure. The source matrix is modified by the function.
  @param[in]     threshold norm2 threshold.    
  @param[out]    pOutR     points to output R matrix structure of dimension m x n
  @param[out]    pOutQ     points to output Q matrix structure of dimension m x m (can be NULL)
  @param[out]    pOutTau   points to Householder scaling factors of dimension n
  @param[inout]  pTmpA     points to a temporary vector of dimension m.
  @param[inout]  pTmpB     points to a temporary vector of dimension m.
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed
  
  @par           pOutQ is optional:
                 pOutQ can be a NULL pointer.
                 In this case, the argument will be ignored
                 and the output Q matrix won't be computed.

  @par           f16 implementation
                 The f16 implementation is not very accurate.

  @par           Norm2 threshold 
                 For the meaning of this argument please 
                 refer to the \ref MatrixHouseholder documentation

 */

#if defined(RISCV_FLOAT16_SUPPORTED)

RISCV_DSP_ATTRIBUTE riscv_status riscv_mat_qr_f16(
    const riscv_matrix_instance_f16 * pSrc,
    const float16_t threshold,
    riscv_matrix_instance_f16 * pOutR,
    riscv_matrix_instance_f16 * pOutQ,
    float16_t * pOutTau,
    float16_t *pTmpA,
    float16_t *pTmpB
    )

{
  int32_t col=0;
  int32_t nb,pos;
  float16_t *pa,*pc;
  float16_t beta;
  float16_t *pv;
  float16_t *pdst;
  float16_t *p;

  if (pSrc->numRows < pSrc->numCols)
  {
    return(RISCV_MATH_SIZE_MISMATCH);
  }

  memcpy(pOutR->pData,pSrc->pData,pSrc->numCols * pSrc->numRows*sizeof(float16_t));
  pOutR->numCols = pSrc->numCols;
  pOutR->numRows = pSrc->numRows;
  
  p = pOutR->pData;
  
  pc = pOutTau;
  for(col=0 ; col < pSrc->numCols; col++)
  {
      int32_t i,j,k,blkCnt;
      float16_t *pa0,*pa1,*pa2,*pa3;
      COPY_COL_F16(pOutR,col,col,pTmpA);

      beta = riscv_householder_f16(pTmpA,threshold,pSrc->numRows - col,pTmpA);
      *pc++ = beta;
    
      pdst = pTmpB;

      /* v.T A(col:,col:) -> tmpb */
      pv = pTmpA;
      pa = p;
      for(j=0;j<pSrc->numCols-col; j++)
      {
              *pdst++ = (_Float16)*pv * (_Float16)*pa++; 
      }
      pa += col;
      pv++;
      pdst = pTmpB;

      pa0 = pa;
      pa1 = pa0 + pSrc->numCols;
      pa2 = pa1 + pSrc->numCols;
      pa3 = pa2 + pSrc->numCols;

      /* Unrolled loop */
      blkCnt = (pSrc->numRows-col - 1) >> 2;
      k=1;
      while(blkCnt > 0)
      {
          float16_t sum;

          for(j=0;j<pSrc->numCols-col; j++)
          {
              sum = *pdst;

              sum += (_Float16)pv[0] * (_Float16)*pa0++;
              sum += (_Float16)pv[1] * (_Float16)*pa1++;
              sum += (_Float16)pv[2] * (_Float16)*pa2++;
              sum += (_Float16)pv[3] * (_Float16)*pa3++;
              
              *pdst++ = sum; 
          }
          pa0 += col + 3*pSrc->numCols;
          pa1 += col + 3*pSrc->numCols;
          pa2 += col + 3*pSrc->numCols;
          pa3 += col + 3*pSrc->numCols;
          pv  += 4;
          pdst = pTmpB;
          k += 4;
          blkCnt--;
      }

      pa = pa0;
      for(;k<pSrc->numRows-col; k++)
      {
          for(j=0;j<pSrc->numCols-col; j++)
          {
              *pdst++ += (_Float16)*pv * (_Float16)*pa++; 
          }
          pa += col;
          pv++;
          pdst = pTmpB;
      }

      /* A(col:,col:) - beta v tmpb */
      pa = p;
      for(j=0;j<pSrc->numRows-col; j++)
      {
        float16_t f = (_Float16)beta * (_Float16)pTmpA[j];

        for(i=0;i<pSrc->numCols-col; i++)
        {
          *pa = (_Float16)*pa - (_Float16)f * (_Float16)pTmpB[i] ;
          pa++;
        }
        pa += col;
      } 

      /* Copy Householder reflectors into R matrix */
      pa = p + pOutR->numCols;
      for(k=0;k<pSrc->numRows-col-1; k++)
      {
         *pa = pTmpA[k+1];
         pa += pOutR->numCols;
      }

      p += 1 + pOutR->numCols;
  }

  /* Generate Q if requested by user matrix */

  if (pOutQ != NULL)
  {
     /* Initialize Q matrix to identity */
     memset(pOutQ->pData,0,sizeof(float16_t)*pOutQ->numRows*pOutQ->numRows);
     
     pa = pOutQ->pData;
     for(col=0 ; col < pOutQ->numCols; col++)
     {
        *pa = 1.0f16;
        pa += pOutQ->numCols+1;
     }
   
     nb = pOutQ->numRows - pOutQ->numCols + 1;
   
     pc = pOutTau + pOutQ->numCols - 1;
     for(col=0 ; col < pOutQ->numCols; col++)
     {
       int32_t i,j,k, blkCnt;
       float16_t *pa0,*pa1,*pa2,*pa3;
       pos = pSrc->numRows - nb;
       p = pOutQ->pData + pos + pOutQ->numCols*pos ;
   
       
       COPY_COL_F16(pOutR,pos,pos,pTmpA);
       pTmpA[0] = 1.0f16;
       pdst = pTmpB;
      
       /* v.T A(col:,col:) -> tmpb */
       
       pv = pTmpA;
       pa = p;
       for(j=0;j<pOutQ->numRows-pos; j++)
       {
               *pdst++ = (_Float16)*pv * (_Float16)*pa++; 
       }
       pa += pos;
       pv++;
       pdst = pTmpB;
       pa0 = pa;
       pa1 = pa0 + pOutQ->numRows;
       pa2 = pa1 + pOutQ->numRows;
       pa3 = pa2 + pOutQ->numRows;

       /* Unrolled loop */
       blkCnt = (pOutQ->numRows-pos - 1) >> 2;
       k=1;
       while(blkCnt > 0)
       {
           float16_t sum;

           for(j=0;j<pOutQ->numRows-pos; j++)
           {
              sum = *pdst;

              sum += (_Float16)pv[0] * (_Float16)*pa0++;
              sum += (_Float16)pv[1] * (_Float16)*pa1++;
              sum += (_Float16)pv[2] * (_Float16)*pa2++;
              sum += (_Float16)pv[3] * (_Float16)*pa3++;
              
              *pdst++ = sum; 
           }
           pa0 += pos + 3*pOutQ->numRows;
           pa1 += pos + 3*pOutQ->numRows;
           pa2 += pos + 3*pOutQ->numRows;
           pa3 += pos + 3*pOutQ->numRows;
           pv  += 4;
           pdst = pTmpB;
           k += 4;
           blkCnt--;
       }

       pa = pa0;
       for(;k<pOutQ->numRows-pos; k++)
       {
           for(j=0;j<pOutQ->numRows-pos; j++)
           {
               *pdst++ += (_Float16)*pv * (_Float16)*pa++; 
           }
           pa += pos;
           pv++;
           pdst = pTmpB;
       }
   
       pa = p;
       beta = *pc--;
       for(j=0;j<pOutQ->numRows-pos; j++)
       {
           float16_t f = (_Float16)beta * (_Float16)pTmpA[j];

           for(i=0;i<pOutQ->numCols-pos; i++)
           {
             *pa = (_Float16)*pa - (_Float16)f * (_Float16)pTmpB[i] ;
             pa++;
           }
           pa += pos;
       } 
   
   
       nb++;
     }
  }

  riscv_status status = RISCV_MATH_SUCCESS;
  /* Return to application */
  return (status);
}

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
/**
  @} end of MatrixQR group
 */
