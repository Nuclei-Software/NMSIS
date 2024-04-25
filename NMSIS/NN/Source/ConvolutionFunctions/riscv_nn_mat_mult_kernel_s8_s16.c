/*
 * SPDX-FileCopyrightText: Copyright 2010-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_nn_mat_mult_kernel_s8_s16.c
 * Description:  Matrix-multiplication function for convolution
 *
 * $Date:        29 May 2023
 * $Revision:    V.2.0.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/*
 * Matrix-multiplication function for convolution with per-channel requantization.
 *
 * Refer header file for details.
 *
 */

int8_t *riscv_nn_mat_mult_kernel_s8_s16(const int8_t *input_a,
                                      const int16_t *input_b,
                                      const uint16_t output_ch,
                                      const int32_t *out_shift,
                                      const int32_t *out_mult,
                                      const int32_t out_offset,
                                      const int16_t activation_min,
                                      const int16_t activation_max,
                                      const int32_t num_col_a,
                                      const int32_t aligned_num_col_a,
                                      const int32_t *const output_bias,
                                      int8_t *out_0)
{
    /* set up the second output pointers */
    int8_t *out_1 = out_0 + output_ch;
    const int32_t *bias = output_bias;

#if defined(RISCV_MATH_VECTOR)
    uint16_t row_count = output_ch;
    const int8_t *ip_a0 = input_a;
    /* this loop over rows in A */
    while (row_count)
    {
        /* setup pointers for B */
        const int16_t *ip_b0 = input_b;
        const int16_t *ip_b1 = ip_b0 + aligned_num_col_a;

        int32_t ch_0_out_0 = 0;
        int32_t ch_0_out_1 = 0;

        /* load the bias */
        if (bias)
        {
            ch_0_out_0 = *bias;
            ch_0_out_1 = *bias++;
        }

        uint16_t col_count = num_col_a;
        size_t l;

        vint16m4_t va0m4, vb0m4, vb1m4;
        vint32m1_t vtemp00m1, vtemp01m1;

        l = __riscv_vsetvl_e32m1(1);
        vtemp00m1 = __riscv_vmv_v_x_i32m1(0, l);
        vtemp01m1 = __riscv_vmv_v_v_i32m1(vtemp00m1, l);
        for (; (l = __riscv_vsetvl_e8m2(col_count)) > 0; col_count -= l) {
            va0m4 = __riscv_vwadd_vx_i16m4(__riscv_vle8_v_i8m2(ip_a0, l), 0, l);
            vb0m4 = __riscv_vle16_v_i16m4(ip_b0, l);
            vb1m4 = __riscv_vle16_v_i16m4(ip_b1, l);
            ip_a0 += l;
            ip_b0 += l;
            ip_b1 += l;

            vtemp00m1 = __riscv_vredsum_vs_i32m8_i32m1(__riscv_vwmul_vv_i32m8(va0m4, vb0m4, l), vtemp00m1, l);
            vtemp01m1 = __riscv_vredsum_vs_i32m8_i32m1(__riscv_vwmul_vv_i32m8(va0m4, vb1m4, l), vtemp01m1, l);
        }
        ch_0_out_0 += (int32_t)__riscv_vmv_x_s_i32m1_i32(vtemp00m1);
        ch_0_out_1 += (int32_t)__riscv_vmv_x_s_i32m1_i32(vtemp01m1);

        ch_0_out_0 = riscv_nn_requantize(ch_0_out_0, *out_mult, *out_shift);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = MIN(ch_0_out_0, activation_max);
        *out_0++ = (int8_t)ch_0_out_0;

        ch_0_out_1 = riscv_nn_requantize(ch_0_out_1, *out_mult, *out_shift);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = MIN(ch_0_out_1, activation_max);
        *out_1++ = (int8_t)ch_0_out_1;

        out_mult++;
        out_shift++;
        row_count--;
    }
    out_0 += output_ch;
#else
    uint16_t row_count = output_ch / 2;
    const int8_t *ip_a0 = input_a;
    /* this loop over rows in A */
    while (row_count)
    {
        /* setup pointers for B */
        const int16_t *ip_b0 = input_b;
        const int16_t *ip_b1 = ip_b0 + aligned_num_col_a;

        /* align the second pointer for A */
        const int8_t *ip_a1 = ip_a0 + num_col_a;

        int32_t ch_0_out_0 = 0;
        int32_t ch_0_out_1 = 0;
        int32_t ch_1_out_0 = 0;
        int32_t ch_1_out_1 = 0;
        /* Init accumulator with bias for channel N and N + 1 */
        if (bias)
        {
            ch_0_out_0 = *bias;
            ch_0_out_1 = *bias++;
            ch_1_out_0 = *bias;
            ch_1_out_1 = *bias++;
        }

#if defined(RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        int64_t ch_0_out_64_0 = 0;
        int64_t ch_0_out_64_1 = 0;
        int64_t ch_1_out_64_0 = 0;
        int64_t ch_1_out_64_1 = 0;
        int64_t a01, a02, a11, a12;
        int32_t col_count = num_col_a >> 3;
#else
        int32_t a01, a02, a11, a12;
        int32_t col_count = num_col_a >> 2;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
        /* accumulate over the vector */
        while (col_count)
        {
#if (__RISCV_XLEN == 64)
            int64_t b0 = riscv_nn_read_s16x4_ia(&ip_b0);
            int64_t b1 = riscv_nn_read_s16x4_ia(&ip_b1);

            ip_a0 = read_and_pad_reordered64(ip_a0, &a01, &a02);
            ch_0_out_64_0 = __SMLAD(a01, b0, ch_0_out_64_0);
            ch_0_out_64_1 = __SMLAD(a01, b1, ch_0_out_64_1);

            ip_a1 = read_and_pad_reordered64(ip_a1, &a11, &a12);
            ch_1_out_64_0 = __SMLAD(a11, b0, ch_1_out_64_0);
            ch_1_out_64_1 = __SMLAD(a11, b1, ch_1_out_64_1);

            b0 = riscv_nn_read_s16x4_ia(&ip_b0);
            b1 = riscv_nn_read_s16x4_ia(&ip_b1);
            ch_0_out_64_0 = __SMLAD(a02, b0, ch_0_out_64_0);
            ch_0_out_64_1 = __SMLAD(a02, b1, ch_0_out_64_1);
            ch_1_out_64_0 = __SMLAD(a12, b0, ch_1_out_64_0);
            ch_1_out_64_1 = __SMLAD(a12, b1, ch_1_out_64_1);
#else
#if defined (NUCLEI_DSP_N3)
            int64_t b0 = riscv_nn_read_s16x4_ia(&ip_b0);
            int64_t b1 = riscv_nn_read_s16x4_ia(&ip_b1);

            ip_a0 = read_and_pad_reordered64(ip_a0, &a01, &a02);
            ch_0_out_64_0 = __RV_DKMADA(ch_0_out_64_0, a01, b0);
            ch_0_out_64_1 = __RV_DKMADA(ch_0_out_64_1, a01, b1);
            ip_a1 = read_and_pad_reordered64(ip_a1, &a11, &a12);
            ch_1_out_64_0 = __RV_DKMADA(ch_1_out_64_0, a11, b0);
            ch_1_out_64_1 = __RV_DKMADA(ch_1_out_64_1, a11, b1);

            b0 = riscv_nn_read_s16x4_ia(&ip_b0);
            b1 = riscv_nn_read_s16x4_ia(&ip_b1);
            ch_0_out_64_0 = __RV_DKMADA(ch_0_out_64_0, a02, b0);
            ch_0_out_64_1 = __RV_DKMADA(ch_0_out_64_1, a02, b1);
            ch_1_out_64_0 = __RV_DKMADA(ch_1_out_64_0, a12, b0);
            ch_1_out_64_1 = __RV_DKMADA(ch_1_out_64_1, a12, b1);
#else
            int32_t b0 = riscv_nn_read_s16x2_ia(&ip_b0);
            int32_t b1 = riscv_nn_read_s16x2_ia(&ip_b1);

            ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);
            ip_a1 = read_and_pad_reordered(ip_a1, &a11, &a12);

            ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
            ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);
            ch_1_out_0 = __SMLAD(a11, b0, ch_1_out_0);
            ch_1_out_1 = __SMLAD(a11, b1, ch_1_out_1);

            b0 = riscv_nn_read_s16x2_ia(&ip_b0);
            b1 = riscv_nn_read_s16x2_ia(&ip_b1);

            ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
            ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
            ch_1_out_0 = __SMLAD(a12, b0, ch_1_out_0);
            ch_1_out_1 = __SMLAD(a12, b1, ch_1_out_1);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* (__RISCV_XLEN == 64) */

            col_count--;
        } /* while over col_count */

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
       ch_0_out_0 += (int32_t)ch_0_out_64_0 + (int32_t)(ch_0_out_64_0 >> 32);
       ch_0_out_1 += (int32_t)ch_0_out_64_1 + (int32_t)(ch_0_out_64_1 >> 32);
       ch_1_out_0 += (int32_t)ch_1_out_64_0 + (int32_t)(ch_1_out_64_0 >> 32);
       ch_1_out_1 += (int32_t)ch_1_out_64_1 + (int32_t)(ch_1_out_64_1 >> 32);
       col_count = num_col_a & 0x7;
#else
       col_count = num_col_a & 0x3;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
#else
        int32_t col_count = num_col_a;
#endif /* defined(RISCV_MATH_DSP) */
        while (col_count)
        {
            int8_t a0 = *ip_a0++;
            int16_t b0 = *ip_b0++;
            int8_t a1 = *ip_a1++;
            int16_t b1 = *ip_b1++;

            ch_0_out_0 += a0 * b0;
            ch_0_out_1 += a0 * b1;
            ch_1_out_0 += a1 * b0;
            ch_1_out_1 += a1 * b1;
            col_count--;
        } /* while over col_count */

        ch_0_out_0 = riscv_nn_requantize(ch_0_out_0, *out_mult, *out_shift);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = MIN(ch_0_out_0, activation_max);
        *out_0++ = (int8_t)ch_0_out_0;

        ch_0_out_1 = riscv_nn_requantize(ch_0_out_1, *out_mult, *out_shift);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = MIN(ch_0_out_1, activation_max);
        *out_1++ = (int8_t)ch_0_out_1;
        out_mult++;
        out_shift++;

        ch_1_out_0 = riscv_nn_requantize(ch_1_out_0, *out_mult, *out_shift);
        ch_1_out_0 += out_offset;
        ch_1_out_0 = MAX(ch_1_out_0, activation_min);
        ch_1_out_0 = MIN(ch_1_out_0, activation_max);
        *out_0++ = (int8_t)ch_1_out_0;

        ch_1_out_1 = riscv_nn_requantize(ch_1_out_1, *out_mult, *out_shift);
        ch_1_out_1 += out_offset;
        ch_1_out_1 = MAX(ch_1_out_1, activation_min);
        ch_1_out_1 = MIN(ch_1_out_1, activation_max);
        *out_1++ = (int8_t)ch_1_out_1;
        out_mult++;
        out_shift++;

        /* skip row */
        ip_a0 += num_col_a;
        row_count--;
    }

    /* compute the last odd numbered row if any */
    if (output_ch & 0x1)
    {
        /* setup pointers for B */
        const int16_t *ip_b0 = input_b;
        const int16_t *ip_b1 = ip_b0 + aligned_num_col_a;

        int32_t ch_0_out_0 = 0;
        int32_t ch_0_out_1 = 0;

        /* load the bias */
        if (bias)
        {
            ch_0_out_0 = *bias;
            ch_0_out_1 = *bias++;
        }

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        int64_t ch_0_out_64_0 = 0;
        int64_t ch_0_out_64_1 = 0;
        int64_t a01, a02;
        int32_t col_count = num_col_a >> 3;
#else
        int32_t a01, a02;
        int32_t col_count = num_col_a >> 2;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */
        while (col_count)
        {
#if (__RISCV_XLEN == 64)
            int64_t b0 = riscv_nn_read_s16x4_ia(&ip_b0);
            int64_t b1 = riscv_nn_read_s16x4_ia(&ip_b1);

            ip_a0 = read_and_pad_reordered64(ip_a0, &a01, &a02);

            ch_0_out_64_0 = __SMLAD(a01, b0, ch_0_out_64_0);
            ch_0_out_64_1 = __SMLAD(a01, b1, ch_0_out_64_1);

            b0 = riscv_nn_read_s16x4_ia(&ip_b0);
            b1 = riscv_nn_read_s16x4_ia(&ip_b1);
            ch_0_out_64_0 = __SMLAD(a02, b0, ch_0_out_64_0);
            ch_0_out_64_1 = __SMLAD(a02, b1, ch_0_out_64_1);
#else
#if defined (NUCLEI_DSP_N3)
            int64_t b0 = riscv_nn_read_s16x4_ia(&ip_b0);
            int64_t b1 = riscv_nn_read_s16x4_ia(&ip_b1);

            ip_a0 = read_and_pad_reordered64(ip_a0, &a01, &a02);

            ch_0_out_64_0 = __RV_DKMADA(ch_0_out_64_0, a01, b0);
            ch_0_out_64_1 = __RV_DKMADA(ch_0_out_64_1, a01, b1);

            b0 = riscv_nn_read_s16x4_ia(&ip_b0);
            b1 = riscv_nn_read_s16x4_ia(&ip_b1);
            ch_0_out_64_0 = __RV_DKMADA(ch_0_out_64_0, a02, b0);
            ch_0_out_64_1 = __RV_DKMADA(ch_0_out_64_1, a02, b1);
#else
            int32_t b0 = riscv_nn_read_s16x2_ia(&ip_b0);
            int32_t b1 = riscv_nn_read_s16x2_ia(&ip_b1);

            ip_a0 = read_and_pad_reordered(ip_a0, &a01, &a02);

            ch_0_out_0 = __SMLAD(a01, b0, ch_0_out_0);
            ch_0_out_1 = __SMLAD(a01, b1, ch_0_out_1);

            b0 = riscv_nn_read_s16x2_ia(&ip_b0);
            b1 = riscv_nn_read_s16x2_ia(&ip_b1);
            ch_0_out_0 = __SMLAD(a02, b0, ch_0_out_0);
            ch_0_out_1 = __SMLAD(a02, b1, ch_0_out_1);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* (__RISCV_XLEN == 64) */

            col_count--;
        }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
       ch_0_out_0 += (int32_t)ch_0_out_64_0 + (int32_t)(ch_0_out_64_0 >> 32);
       ch_0_out_1 += (int32_t)ch_0_out_64_1 + (int32_t)(ch_0_out_64_1 >> 32);
       col_count = num_col_a & 0x7;
#else
       col_count = num_col_a & 0x3;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
#else
        int32_t col_count = num_col_a;
#endif /* defined(RISCV_MATH_DSP) */
        while (col_count)
        {
            int8_t a0 = *ip_a0++;
            int16_t b0 = *ip_b0++;
            int16_t b1 = *ip_b1++;

            ch_0_out_0 += a0 * b0;
            ch_0_out_1 += a0 * b1;
            col_count--;
        }
        ch_0_out_0 = riscv_nn_requantize(ch_0_out_0, *out_mult, *out_shift);
        ch_0_out_0 += out_offset;
        ch_0_out_0 = MAX(ch_0_out_0, activation_min);
        ch_0_out_0 = MIN(ch_0_out_0, activation_max);
        *out_0++ = (int8_t)ch_0_out_0;

        ch_0_out_1 = riscv_nn_requantize(ch_0_out_1, *out_mult, *out_shift);
        ch_0_out_1 += out_offset;
        ch_0_out_1 = MAX(ch_0_out_1, activation_min);
        ch_0_out_1 = MIN(ch_0_out_1, activation_max);
        *out_1++ = (int8_t)ch_0_out_1;
        out_mult++;
        out_shift++;
    }

    out_0 += output_ch;
#endif /* #if defined(RISCV_MATH_VECTOR) */
    /* return the new output pointer with offset */
    return out_0;
}
