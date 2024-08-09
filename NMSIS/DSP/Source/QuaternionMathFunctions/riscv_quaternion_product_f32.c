/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_quaternion_product_f32.c
 * Description:  Floating-point quaternion product
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: RISC-V Cores
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

#include "dsp/quaternion_math_functions.h"
#include <math.h>

/**
  @ingroup groupQuaternionMath
 */

/**
  @defgroup QuatProd Quaternion Product

  Compute the product of quaternions.
 */

/**
  @ingroup QuatProd
 */

/**
  @defgroup QuatProdVect Elementwise Quaternion Product

  Compute the elementwise product of quaternions.
 */

/**
  @addtogroup QuatProdVect
  @{
 */

/**
  @brief         Floating-point elementwise product two quaternions.
  @param[in]     qa                  first array of quaternions
  @param[in]     qb                  second array of quaternions
  @param[out]    qr                   elementwise product of quaternions
  @param[in]     nbQuaternions       number of quaternions in the array
 */


#include "riscv_helium_utils.h"

RISCV_DSP_ATTRIBUTE void riscv_quaternion_product_f32(const float32_t *qa, 
    const float32_t *qb, 
    float32_t *qr,
    uint32_t nbQuaternions)
{
#if defined(RISCV_MATH_VECTOR)
    uint32_t blkCnt = nbQuaternions;                               /* Loop counter */
    size_t l;
    vfloat32m2_t v_QA0, v_QA1, v_QA2, v_QA3;
    vfloat32m2_t v_QB0, v_QB1, v_QB2, v_QB3;
    vfloat32m2_t v_QR0, v_QR1, v_QR2, v_QR3;
    vfloat32m2x4_t v_tuple;
    vfloat32m2_t v_temp;
    const float32_t *pQA = qa;
    const float32_t *pQB = qb;
    float32_t *pQR = qr;
    ptrdiff_t bstride = 16;
    for (; (l = __riscv_vsetvl_e32m2(blkCnt)) > 0; blkCnt -= l) {
        //v_QA0 = __riscv_vlse32_v_f32m2(pQA, bstride, l);
        //v_QA1 = __riscv_vlse32_v_f32m2(pQA + 1, bstride, l);
        //v_QA2 = __riscv_vlse32_v_f32m2(pQA + 2, bstride, l);
        //v_QA3 = __riscv_vlse32_v_f32m2(pQA + 3, bstride, l);
        //vlsseg4e32_v_f32m2(&v_QA0, &v_QA1, &v_QA2, &v_QA3, pQA, bstride, l);

        v_tuple = __riscv_vlsseg4e32_v_f32m2x4 (pQA, bstride, l);
        v_QA0 = __riscv_vget_v_f32m2x4_f32m2(v_tuple, 0);
        v_QA1 = __riscv_vget_v_f32m2x4_f32m2(v_tuple, 1);
        v_QA2 = __riscv_vget_v_f32m2x4_f32m2(v_tuple, 2);
        v_QA3 = __riscv_vget_v_f32m2x4_f32m2(v_tuple, 3);

        //v_QB0 = __riscv_vlse32_v_f32m2(pQB, bstride, l);
        //v_QB1 = __riscv_vlse32_v_f32m2(pQB + 1, bstride, l);
        //v_QB2 = __riscv_vlse32_v_f32m2(pQB + 2, bstride, l);
        //v_QB3 = __riscv_vlse32_v_f32m2(pQB + 3, bstride, l);
        //vlsseg4e32_v_f32m2(&v_QB0, &v_QB1, &v_QB2, &v_QB3, pQB, bstride, l);

        v_tuple = __riscv_vlsseg4e32_v_f32m2x4 (pQB, bstride, l);
        v_QB0 = __riscv_vget_v_f32m2x4_f32m2(v_tuple, 0);
        v_QB1 = __riscv_vget_v_f32m2x4_f32m2(v_tuple, 1);
        v_QB2 = __riscv_vget_v_f32m2x4_f32m2(v_tuple, 2);
        v_QB3 = __riscv_vget_v_f32m2x4_f32m2(v_tuple, 3);

        v_QR0 = __riscv_vfmul_vv_f32m2(v_QA0, v_QB0, l);
        v_QR0 = __riscv_vfnmsac_vv_f32m2(v_QR0, v_QA1, v_QB1, l);
        v_QR0 = __riscv_vfnmsac_vv_f32m2(v_QR0, v_QA2, v_QB2, l);
        v_QR0 = __riscv_vfnmsac_vv_f32m2(v_QR0, v_QA3, v_QB3, l);
        //__riscv_vsse32_v_f32m2(pQR, bstride, v_QR0, l);

        v_QR1 = __riscv_vfmul_vv_f32m2(v_QA0, v_QB1, l);
        v_QR1 = __riscv_vfmacc_vv_f32m2(v_QR1, v_QA1, v_QB0, l);
        v_QR1 = __riscv_vfmacc_vv_f32m2(v_QR1, v_QA2, v_QB3, l);
        v_QR1 = __riscv_vfnmsac_vv_f32m2(v_QR1, v_QA3, v_QB2, l);
        //__riscv_vsse32_v_f32m2(pQR + 1, bstride, v_QR1, l);

        v_QR2 = __riscv_vfmul_vv_f32m2(v_QA0, v_QB2, l);
        v_QR2 = __riscv_vfmacc_vv_f32m2(v_QR2, v_QA2,v_QB0, l);
        v_QR2 = __riscv_vfmacc_vv_f32m2(v_QR2, v_QA3, v_QB1, l);
        v_QR2 = __riscv_vfnmsac_vv_f32m2(v_QR2, v_QA1, v_QB3, l);
        //__riscv_vsse32_v_f32m2(pQR + 2,bstride, v_QR2, l);

        v_QR3 = __riscv_vfmul_vv_f32m2(v_QA0, v_QB3, l);
        v_QR3 = __riscv_vfmacc_vv_f32m2(v_QR3, v_QA3, v_QB0, l);
        v_QR3 = __riscv_vfmacc_vv_f32m2(v_QR3, v_QA1, v_QB2, l);
        v_QR3 = __riscv_vfnmsac_vv_f32m2(v_QR3, v_QA2, v_QB1, l);
        //__riscv_vsse32_v_f32m2(pQR + 3, bstride, v_QR3, l);
        //vssseg4e32_v_f32m2(pQR, bstride, v_QR0, v_QR1, v_QR2, v_QR3, l);
        v_tuple = __riscv_vset_v_f32m2_f32m2x4 (v_tuple, 0, v_QR0);
        v_tuple = __riscv_vset_v_f32m2_f32m2x4 (v_tuple, 1, v_QR1);
        v_tuple = __riscv_vset_v_f32m2_f32m2x4 (v_tuple, 2, v_QR2);
        v_tuple = __riscv_vset_v_f32m2_f32m2x4 (v_tuple, 3, v_QR3);
        __riscv_vssseg4e32_v_f32m2x4 (pQR, bstride, v_tuple, l);
        pQA += l * 4;
        pQB += l * 4;
        pQR += l * 4;
    }
#else
   for (uint32_t i = 0; i < nbQuaternions; i++)
   {
     riscv_quaternion_product_single_f32(qa, qb, qr);

     qa += 4;
     qb += 4;
     qr += 4;
   }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of QuatProdVect group
 */
