/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_solve_upper_triangular_f64.c
 * Description:  Solve linear system UT X = A with UT upper triangular matrix
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

#include "dsp/matrix_functions.h"


/**
  @ingroup groupMatrix
 */


/**
  @addtogroup MatrixInv
  @{
 */

/**
   * @brief Solve UT . X = A where UT is an upper triangular matrix
   * @param[in]  ut  The upper triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of UT . X = A
   * @return The function returns RISCV_MATH_SINGULAR, if the system can't be solved.
  */

/**
   * Notice: The instruction vfredusum may introduce errors. So, if we use the V-extension implementation,
   * we have to accept the errors that may happen in this function.
  */

RISCV_DSP_ATTRIBUTE riscv_status riscv_mat_solve_upper_triangular_f64(
  const riscv_matrix_instance_f64 * ut,
  const riscv_matrix_instance_f64 * a,
  riscv_matrix_instance_f64 * dst)
  {
riscv_status status;                             /* status of matrix inverse */


#ifdef RISCV_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((ut->numRows != ut->numCols) ||
      (ut->numRows != a->numRows)   )
  {
    /* Set status as RISCV_MATH_SIZE_MISMATCH */
    status = RISCV_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef RISCV_MATH_MATRIX_CHECK */

  {

    int i,j,k,n,cols;

    float64_t *pX = dst->pData;
    float64_t *pUT = ut->pData;
    float64_t *pA = a->pData;

    float64_t *ut_row;
    float64_t *a_col;

    n = dst->numRows;
    cols = dst->numCols;

    for(j=0; j < cols; j ++)
    {
       a_col = &pA[j];

       for(i=n-1; i >= 0 ; i--)
       {
            float64_t tmp=a_col[i * cols];

            ut_row = &pUT[n*i];

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64))
            uint32_t blkCnt;                               /* Loop counter */
            size_t l;
            vfloat64m8_t v_x, v_y;
            vfloat64m1_t v_a;

            float64_t *pVut_row;
            float64_t *pX_row;
            ptrdiff_t bstride;

            blkCnt = n - i - 1;
            pVut_row = ut_row + i + 1;
            pX_row = pX + cols * (i + 1) + j;
            l = __riscv_vsetvl_e64m1(1);
            v_a = __riscv_vfsub_vv_f64m1(v_a, v_a, l);
            bstride = 8 * n;
            for (; (l = __riscv_vsetvl_e64m8(blkCnt)) > 0; blkCnt -= l) {
                v_x = __riscv_vle64_v_f64m8(pVut_row, l);
                pVut_row += l;
                v_y = __riscv_vlse64_v_f64m8(pX_row, bstride, l);
                pX_row += l * cols;
                v_a = __riscv_vfredusum_vs_f64m8_f64m1(__riscv_vfmul_vv_f64m8(v_x, v_y, l), v_a, l);
            }
            tmp -= __riscv_vfmv_f_s_f64m1_f64(v_a);
#else
            for(k=n-1; k > i; k--)
            {
                tmp -= ut_row[k] * pX[cols*k+j];
            }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64)) */

            if (ut_row[i]==0.0L)
            {
              return(RISCV_MATH_SINGULAR);
            }
            tmp = tmp / ut_row[i];
            pX[i*cols+j] = tmp;
       }

    }
    status = RISCV_MATH_SUCCESS;

  }


  /* Return to application */
  return (status);
}


/**
  @} end of MatrixInv group
 */
