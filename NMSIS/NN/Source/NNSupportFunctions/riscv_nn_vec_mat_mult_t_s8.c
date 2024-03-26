/*
 * SPDX-FileCopyrightText: Copyright 2020-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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

/* ----------------------------------------------------------------------
 * Project:      NMSIS NN Library
 * Title:        riscv_nn_vec_mat_mult_t_s8
 * Description:  s8 vector by matrix (transposed) multiplication
 *
 * $Date:        27 March 2023
 * $Revision:    V.5.4.0
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @defgroup supportFC Fully Connected
 *
 * Support functions for Fully Connected
 *
 */

/**
 * @addtogroup supportFC
 * @{
 */

/*
 * s8 vector(lhs) by matrix (transposed) multiplication
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_vec_mat_mult_t_s8(const int8_t *lhs,
                                             const int8_t *rhs,
                                             const int32_t *bias,
                                             int8_t *dst,
                                             const int32_t lhs_offset,
                                             const int32_t dst_offset,
                                             const int32_t dst_multiplier,
                                             const int32_t dst_shift,
                                             const int32_t rhs_cols,
                                             const int32_t rhs_rows,
                                             const int32_t activation_min,
                                             const int32_t activation_max,
                                             const int32_t address_offset)
{
#if defined (RISCV_MATH_VECTOR)
    uint32_t i, j;
    size_t l;
    const int loop_cnt = rhs_rows;
    vint8m2_t resm2;
    vint32m8_t rhs_value, lhs_value, vres0m8;

    for (i = loop_cnt; i > 0; i -= l) {
      const q7_t *lhs_ptr = &lhs[0];
      const q7_t *rhs_ptr = &rhs[0];
      l = __riscv_vsetvl_e8m2(i);
      if (bias) {
          vres0m8 = __riscv_vle32_v_i32m8(bias, l);
          bias += l;
      } else {
          vres0m8 = __riscv_vmv_v_x_i32m8(0, l);
      }
      for (j = 0; j < rhs_cols; j++) {
        rhs_value = __riscv_vadd_vx_i32m8(__riscv_vsext_vf4_i32m8(__riscv_vlse8_v_i8m2(rhs_ptr, rhs_cols, l), l), 0, l);
        rhs_ptr += 1;
        vres0m8 = __riscv_vmacc_vx_i32m8(vres0m8, *(lhs_ptr + j) + lhs_offset, rhs_value, l);
      }

      /* res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift); */
      vres0m8 = riscv_nn_requantize_m8_rvv(vres0m8, l, dst_multiplier, dst_shift);
      vres0m8 = __riscv_vadd_vx_i32m8(vres0m8, dst_offset, l);
      vres0m8 = __riscv_vmin_vx_i32m8(__riscv_vmax_vx_i32m8(vres0m8, activation_min, l), activation_max, l);

     // resm2 = __riscv_vncvt_x_x_w_i8m2(__riscv_vncvt_x_x_w_i16m4(vres0m8, l), l);
      resm2 = __riscv_vnsra_wx_i8m2(__riscv_vnsra_wx_i16m4(vres0m8, 0, l), 0, l);
      __riscv_vsse8_v_i8m2(dst, address_offset, resm2, l);
      dst += address_offset * l;
      rhs += rhs_cols * l;
  }
#elif defined(RISCV_MATH_DSP)
    const int32_t row_loop_cnt = rhs_rows / 2;
    const int16_t lhs_offset_s16 = (int16_t)lhs_offset;
    const uint32_t lhs_offset_s16x2 = __NN_PKHBT(lhs_offset_s16, lhs_offset_s16, 16);
#if (__RISCV_XLEN == 64)
    int64_t lhs_offset_s16x4 = __RV_PKBB32(lhs_offset_s16x2, lhs_offset_s16x2);
#else
#if defined (NUCLEI_DSP_N3)
    int64_t lhs_offset_s16x4 = __RV_DPACK32(lhs_offset_s16x2, lhs_offset_s16x2);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* (__RISCV_XLEN == 64) */

    for (int32_t i = 0; i < row_loop_cnt; i++)
    {
        int32_t acc_0 = 0;
        int32_t acc_1 = 0;
        if (bias)
        {
            acc_0 = *bias++;
            acc_1 = *bias++;
        }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        int64_t acc64_0 = 0, acc64_1 = 0;
        int64_t vec_0, vec_1, ker_0, ker_1;
        const int32_t col_loop_cnt = rhs_cols >> 3;
#else
        const int32_t col_loop_cnt = rhs_cols >> 2;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */

        const int8_t *lhs_vec = lhs;
        const int8_t *rhs_0 = rhs;
        const int8_t *rhs_1 = rhs + rhs_cols;
        rhs += 2 * rhs_cols;

        for (int j = col_loop_cnt; j != 0; j--)
        {
#if __RISCV_XLEN == 64
            lhs_vec = read_and_pad_reordered64(lhs_vec, &vec_0, &vec_1);
            vec_0 = __RV_ADD16(lhs_offset_s16x4, vec_0);
            vec_1 = __RV_ADD16(lhs_offset_s16x4, vec_1);

            rhs_0 = read_and_pad_reordered64(rhs_0, &ker_0, &ker_1);
            acc64_0 = __SMLAD(ker_1, vec_1, acc64_0);
            acc64_0 = __SMLAD(ker_0, vec_0, acc64_0);

            rhs_1 = read_and_pad_reordered64(rhs_1, &ker_0, &ker_1);
            acc64_1 = __SMLAD(ker_1, vec_1, acc64_1);
            acc64_1 = __SMLAD(ker_0, vec_0, acc64_1);
#else
#if defined (NUCLEI_DSP_N3)
            lhs_vec = read_and_pad_reordered32(lhs_vec, &vec_0, &vec_1);
            vec_0 = __RV_DADD16(lhs_offset_s16x4, vec_0);
            vec_1 = __RV_DADD16(lhs_offset_s16x4, vec_1);

            rhs_0 = read_and_pad_reordered32(rhs_0, &ker_0, &ker_1);
            acc64_0 = __RV_DKMADA(acc64_0, ker_1, vec_1);
            acc64_0 = __RV_DKMADA(acc64_0, ker_0, vec_0);

            rhs_1 = read_and_pad_reordered32(rhs_1, &ker_0, &ker_1);
            acc64_1 = __RV_DKMADA(acc64_1, ker_1, vec_1);
            acc64_1 = __RV_DKMADA(acc64_1, ker_0, vec_0);
#else
            int32_t vec_0 = riscv_nn_read_s8x4_ia(&lhs_vec);
            int32_t vec_1 = __SXTAB16_RORn(lhs_offset_s16x2, (uint32_t)vec_0, 8);

            vec_0 = __SXTAB16(lhs_offset_s16x2, vec_0);

            int32_t ker_0 = riscv_nn_read_s8x4_ia(&rhs_0);
            int32_t ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);

            acc_0 = __SMLAD(ker_1, vec_1, acc_0);
            acc_0 = __SMLAD(ker_0, vec_0, acc_0);

            ker_0 = riscv_nn_read_s8x4_ia(&rhs_1);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);

            acc_1 = __SMLAD(ker_1, vec_1, acc_1);
            acc_1 = __SMLAD(ker_0, vec_0, acc_1);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
        }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        acc_0 += (int32_t)acc64_0 + (int32_t)(acc64_0 >> 32);
        acc_1 += (int32_t)acc64_1 + (int32_t)(acc64_1 >> 32);
        for (int k = col_loop_cnt * 8; k < rhs_cols; k++)
        {
            const int32_t lhs_temp = (*lhs_vec + lhs_offset);
            lhs_vec++;
            acc_0 += lhs_temp * (*rhs_0);
            rhs_0++;
            acc_1 += lhs_temp * (*rhs_1);
            rhs_1++;
        }
#else
        for (int k = col_loop_cnt * 4; k < rhs_cols; k++)
        {
            const int32_t lhs_temp = (*lhs_vec + lhs_offset);
            lhs_vec++;
            acc_0 += lhs_temp * (*rhs_0);
            rhs_0++;
            acc_1 += lhs_temp * (*rhs_1);
            rhs_1++;
        }
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */

        acc_0 = riscv_nn_requantize(acc_0, dst_multiplier, dst_shift);
        acc_1 = riscv_nn_requantize(acc_1, dst_multiplier, dst_shift);

        // Add offset
        acc_0 += dst_offset;
        acc_1 += dst_offset;
        // Clamp the result
        acc_0 = MAX(acc_0, activation_min);
        acc_0 = MIN(acc_0, activation_max);
        acc_1 = MAX(acc_1, activation_min);
        acc_1 = MIN(acc_1, activation_max);
        *dst = (int8_t)acc_0;
        *(dst + address_offset) = (int8_t)acc_1;
        dst += 2 * address_offset;
    }

    if (rhs_rows & 0x1)
    {
        int32_t acc_0 = 0;
        if (bias)
        {
            acc_0 = *bias++;
        }
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        int64_t acc64_0 = 0;
        int64_t vec_0, vec_1, ker_0, ker_1;
        const int32_t col_loop_cnt = rhs_cols >> 3;
#else
        const int32_t col_loop_cnt = rhs_cols >> 2;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */

        const int8_t *lhs_vec = lhs;
        const int8_t *rhs_0 = rhs;

        for (int i = col_loop_cnt; i != 0; i--)
        {
#if __RISCV_XLEN == 64
            lhs_vec = read_and_pad_reordered64(lhs_vec, &vec_0, &vec_1);
            vec_0 = __RV_ADD16(lhs_offset_s16x4, vec_0);
            vec_1 = __RV_ADD16(lhs_offset_s16x4, vec_1);

            rhs_0 = read_and_pad_reordered64(rhs_0, &ker_0, &ker_1);
            acc64_0 = __SMLAD(ker_1, vec_1, acc64_0);
            acc64_0 = __SMLAD(ker_0, vec_0, acc64_0);
#else
#if defined (NUCLEI_DSP_N3)
            lhs_vec = read_and_pad_reordered32(lhs_vec, &vec_0, &vec_1);
            vec_0 = __RV_DADD16(lhs_offset_s16x4, vec_0);
            vec_1 = __RV_DADD16(lhs_offset_s16x4, vec_1);

            rhs_0 = read_and_pad_reordered32(rhs_0, &ker_0, &ker_1);
            acc64_0 = __RV_DKMADA(acc64_0, ker_1, vec_1);
            acc64_0 = __RV_DKMADA(acc64_0, ker_0, vec_0);
#else
            int32_t vec_0 = riscv_nn_read_s8x4_ia(&lhs_vec);
            int32_t vec_1 = __SXTAB16_RORn(lhs_offset_s16x2, (uint32_t)vec_0, 8);
            vec_0 = __SXTAB16(lhs_offset_s16x2, vec_0);

            int32_t ker_0 = riscv_nn_read_s8x4_ia(&rhs_0);
            int32_t ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);

            acc_0 = __SMLAD(ker_1, vec_1, acc_0);
            acc_0 = __SMLAD(ker_0, vec_0, acc_0);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
        }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        acc_0 += (int32_t)acc64_0 + (int32_t)(acc64_0 >> 32);
        for (int k = col_loop_cnt * 8; k < rhs_cols; k++)
        {
            const int32_t lhs_temp = (*lhs_vec + lhs_offset);
            lhs_vec++;
            acc_0 += lhs_temp * (*rhs_0);
            rhs_0++;
        }
#else
        for (int j = col_loop_cnt * 4; j < rhs_cols; j++)
        {
            const int32_t lhs_temp = (*lhs_vec + lhs_offset);
            lhs_vec++;
            acc_0 += lhs_temp * (*rhs_0);
            rhs_0++;
        }
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */

        acc_0 = riscv_nn_requantize(acc_0, dst_multiplier, dst_shift);

        // Add offset
        acc_0 += dst_offset;
        // Clamp the result
        acc_0 = MAX(acc_0, activation_min);
        acc_0 = MIN(acc_0, activation_max);
        *dst = (int8_t)acc_0;
        dst += address_offset;
    }

#else

    const int32_t row_loop_cnt = rhs_rows / 3;

    for (int i_row_loop_cnt = 0; i_row_loop_cnt < row_loop_cnt; i_row_loop_cnt++)
    {
        const int8_t *lhs_ptr = lhs;
        const int8_t *rhs_ptr_0 = &rhs[0];
        const int8_t *rhs_ptr_1 = &rhs[rhs_cols];
        const int8_t *rhs_ptr_2 = &rhs[rhs_cols * 2];

        int32_t res00 = 0;
        int32_t res01 = 0;
        int32_t res02 = 0;
        if (bias)
        {
            res00 = *bias++;
            res01 = *bias++;
            res02 = *bias++;
        }
        for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
        {
            const int32_t rhs_value0 = (int8_t)*rhs_ptr_0;
            const int32_t rhs_value1 = (int8_t)*rhs_ptr_1;
            const int32_t rhs_value2 = (int8_t)*rhs_ptr_2;
            const int32_t lhs_value = (int8_t)*lhs_ptr + lhs_offset;

            res00 += lhs_value * rhs_value0;
            res01 += lhs_value * rhs_value1;
            res02 += lhs_value * rhs_value2;

            ++rhs_ptr_0;
            ++rhs_ptr_1;
            ++rhs_ptr_2;
            ++lhs_ptr;
        }
        // Quantize down
        res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift);
        res01 = riscv_nn_requantize(res01, dst_multiplier, dst_shift);
        res02 = riscv_nn_requantize(res02, dst_multiplier, dst_shift);

        // Add offset
        res00 += dst_offset;
        res01 += dst_offset;
        res02 += dst_offset;

        // Clamp the result
        res00 = MAX(res00, activation_min);
        res00 = MIN(res00, activation_max);
        res01 = MAX(res01, activation_min);
        res01 = MIN(res01, activation_max);
        res02 = MAX(res02, activation_min);
        res02 = MIN(res02, activation_max);

        *dst = (int8_t)res00;
        *(dst + address_offset) = (int8_t)res01;
        *(dst + 2 * address_offset) = (int8_t)res02;
        dst += 3 * address_offset;

        rhs += 3 * rhs_cols;
    }

    const int loop_cnt = rhs_rows % 3;

    for (int i_loop_cnt = 0; i_loop_cnt < loop_cnt; i_loop_cnt++)
    {
        const int8_t *lhs_ptr = &lhs[0];
        const int8_t *rhs_ptr = &rhs[0];

        int32_t res00 = 0;
        if (bias)
        {
            res00 = *bias++;
        }

        for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
        {
            int32_t rhs_value0 = (int8_t)rhs_ptr[0];
            int32_t lhs_value = (int8_t)lhs_ptr[0] + lhs_offset;

            res00 += lhs_value * rhs_value0;

            ++rhs_ptr;
            ++lhs_ptr;
        }

        // Quantize down
        res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift);

        // Add offset
        res00 += dst_offset;

        // Clamp the result
        res00 = MAX(res00, activation_min);
        res00 = MIN(res00, activation_max);

        *dst = (int8_t)res00;
        dst += address_offset;
        rhs += rhs_cols;
    }
#endif
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
