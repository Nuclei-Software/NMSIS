/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        riscv_convolve_HWC_q7_fast_nonsquare.c
 * Description:  Fast Q7 version of convolution (non-sqaure shape)
 *
 * $Date:        July 20, 2021
 * $Revision:    V.1.1.2
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup NNConv
 * @{
 */

/**
 * @brief Fast Q7 convolution function (non-sqaure shape)
 * @param[in]       Im_in        pointer to input tensor
 * @param[in]       dim_im_in_x  input tensor dimention x
 * @param[in]       dim_im_in_y  input tensor dimention y
 * @param[in]       ch_im_in     number of input tensor channels
 * @param[in]       wt           pointer to kernel weights
 * @param[in]       ch_im_out    number of filters, i.e., output tensor channels
 * @param[in]       dim_kernel_x filter kernel size x
 * @param[in]       dim_kernel_y filter kernel size y
 * @param[in]       padding_x    padding size x
 * @param[in]       padding_y    padding size y
 * @param[in]       stride_x     convolution stride x
 * @param[in]       stride_y     convolution stride y
 * @param[in]       bias         pointer to bias
 * @param[in]       bias_shift   amount of left-shift for bias
 * @param[in]       out_shift    amount of right-shift for output
 * @param[in,out]   Im_out       pointer to output tensor
 * @param[in]       dim_im_out_x output tensor dimension x
 * @param[in]       dim_im_out_y output tensor dimension y
 * @param[in,out]   bufferA      pointer to buffer space for input
 * @param[in,out]   bufferB      pointer to buffer space for output
 * @return     The function returns either
 * <code>RISCV_NMSIS_NN_SIZE_MISMATCH</code> or <code>RISCV_NMSIS_NN_SUCCESS</code> based on the outcome of size checking.
 *
 * This function is the version with full list of optimization tricks, but with
 * some constraints:
 *   ch_im_in is multiple of 4
 *   ch_im_out is multiple of 2
 */

riscv_nmsis_nn_status riscv_convolve_HWC_q7_fast_nonsquare(const q7_t *Im_in,
                                              const uint16_t dim_im_in_x,
                                              const uint16_t dim_im_in_y,
                                              const uint16_t ch_im_in,
                                              const q7_t *wt,
                                              const uint16_t ch_im_out,
                                              const uint16_t dim_kernel_x,
                                              const uint16_t dim_kernel_y,
                                              const uint16_t padding_x,
                                              const uint16_t padding_y,
                                              const uint16_t stride_x,
                                              const uint16_t stride_y,
                                              const q7_t *bias,
                                              const uint16_t bias_shift,
                                              const uint16_t out_shift,
                                              q7_t *Im_out,
                                              const uint16_t dim_im_out_x,
                                              const uint16_t dim_im_out_y,
                                              q15_t *bufferA,
                                              q7_t *bufferB)
{
    (void)bufferB;
#if defined (RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)
    /* Run the following code for RISC-V Core with DSP enabled */

    int16_t i_out_y, i_out_x, i_ker_y, i_ker_x;

    /* -----------------------
     *  Here we use bufferA as q15_t internally as computation are done with q15_t level
     *  im2col are done to output in q15_t format from q7_t input
     */

    q7_t *pBuffer = (q7_t *)bufferA;
    q7_t *pOut = Im_out;

    if (ch_im_in % 4 != 0 || ch_im_out % 2 != 0)
    {
        /* check if the input dimension meets the constraints */
        return RISCV_NMSIS_NN_FAILURE;
    }

    /*
     *  Here we split the entire matrix into three regions depending on the padding situation
     *    Top: i_out_y from 0 to padding - 1
     * Middle: i_out_y from padding to dim_im_out-padding-1
     * Bottom: i_out_y from dim_im_out-padding to dim_im_out-1
     */

    /* top part */
    for (i_out_y = 0; i_out_y < padding_y; i_out_y++)
    {
        for (i_out_x = 0; i_out_x < dim_im_out_x; i_out_x++)
        {
            /* This part implements the im2col function */
            for (i_ker_y = i_out_y * stride_y - padding_y; i_ker_y < i_out_y * stride_y - padding_y + dim_kernel_y;
                 i_ker_y++)
            {
                for (i_ker_x = i_out_x * stride_x - padding_x; i_ker_x < i_out_x * stride_x - padding_x + dim_kernel_x;
                     i_ker_x++)
                {
                    if (i_ker_y < 0 || i_ker_y >= dim_im_in_y || i_ker_x < 0 || i_ker_x >= dim_im_in_x)
                    {
                        riscv_nn_fill_q7(0, pBuffer, ch_im_in);
                        /* memset(pBuffer, 0, sizeof(q15_t)*ch_im_in); */
                    } else
                    {
                        riscv_q7_to_q7_reordered_no_shift(
                            (q7_t *)Im_in + (i_ker_y * dim_im_in_x + i_ker_x) * ch_im_in, pBuffer, ch_im_in);
                    }
                    pBuffer += ch_im_in;
                }
            }

            if (pBuffer == (q7_t *)bufferA + 2 * ch_im_in * dim_kernel_x * dim_kernel_y)
            {
                pOut = riscv_nn_mat_mult_kernel_q7_reordered(
                    wt, (q7_t *)bufferA, ch_im_out, ch_im_in * dim_kernel_x * dim_kernel_y, bias_shift, out_shift, bias, pOut);
                /* counter reset */
                pBuffer = (q7_t *)bufferA;
            }
        }
    }

    /* middle part, here we also divide the x into left, mid and right */
    for (; i_out_y < dim_im_out_y - padding_y; i_out_y++)
    {

        /* left part */
        for (i_out_x = 0; i_out_x < padding_x; i_out_x++)
        {
            /* This part implements the im2col function */
            for (i_ker_y = i_out_y * stride_y - padding_y; i_ker_y < i_out_y * stride_y - padding_y + dim_kernel_y;
                 i_ker_y++)
            {
                for (i_ker_x = i_out_x * stride_x - padding_x; i_ker_x < i_out_x * stride_x - padding_x + dim_kernel_x;
                     i_ker_x++)
                {
                    if (i_ker_x < 0 || i_ker_x >= dim_im_in_x)
                    {
                        riscv_nn_fill_q7(0, pBuffer, ch_im_in);
                        /* memset(pBuffer, 0, sizeof(q15_t)*ch_im_in); */
                    }
                    else
                    {
                        riscv_q7_to_q7_reordered_no_shift(
                            (q7_t *)Im_in + (i_ker_y * dim_im_in_x + i_ker_x) * ch_im_in, pBuffer, ch_im_in);
                    }
                    pBuffer += ch_im_in;
                }
            }

            if (pBuffer == (q7_t *)bufferA + 2 * ch_im_in * dim_kernel_x * dim_kernel_y)
            {
                pOut = riscv_nn_mat_mult_kernel_q7_reordered(
                    wt, (q7_t *)bufferA, ch_im_out, ch_im_in * dim_kernel_x * dim_kernel_y, bias_shift, out_shift, bias, pOut);
                /* counter reset */
                pBuffer = (q7_t *)bufferA;
            }
        }

        /* mid part */
        for (; i_out_x < dim_im_out_x - padding_x; i_out_x++)
        {
            /* This part implements the im2col function */
            for (i_ker_y = i_out_y * stride_y - padding_y; i_ker_y < i_out_y * stride_y - padding_y + dim_kernel_y;
                 i_ker_y++)
            {
                riscv_q7_to_q7_reordered_no_shift(
                    (q7_t *)Im_in + (i_ker_y * dim_im_in_x + i_out_x * stride_x - padding_x) * ch_im_in,
                    pBuffer,
                    ch_im_in * dim_kernel_x);
                pBuffer += ch_im_in * dim_kernel_x;
            }

            if (pBuffer == (q7_t *)bufferA + 2 * ch_im_in * dim_kernel_x * dim_kernel_y)
            {
                pOut = riscv_nn_mat_mult_kernel_q7_reordered(
                    wt, (q7_t *)bufferA, ch_im_out, ch_im_in * dim_kernel_x * dim_kernel_y, bias_shift, out_shift, bias, pOut);
                /* counter reset */
                pBuffer = (q7_t *)bufferA;
            }
        }

        /* right part */
        for (; i_out_x < dim_im_out_x; i_out_x++)
        {
            /* This part implements the im2col function */
            for (i_ker_y = i_out_y * stride_y - padding_y; i_ker_y < i_out_y * stride_y - padding_y + dim_kernel_y;
                 i_ker_y++)
            {
                for (i_ker_x = i_out_x * stride_x - padding_x; i_ker_x < i_out_x * stride_x - padding_x + dim_kernel_x;
                     i_ker_x++)
                {
                    if (i_ker_x < 0 || i_ker_x >= dim_im_in_x)
                    {
                        riscv_nn_fill_q7(0, pBuffer, ch_im_in);
                        /* memset(pBuffer, 0, sizeof(q15_t)*ch_im_in); */
                    } else
                    {
                        riscv_q7_to_q7_reordered_no_shift(
                            (q7_t *)Im_in + (i_ker_y * dim_im_in_x + i_ker_x) * ch_im_in, pBuffer, ch_im_in);
                    }
                    pBuffer += ch_im_in;
                }
            }

            if (pBuffer == (q7_t *)bufferA + 2 * ch_im_in * dim_kernel_x * dim_kernel_y)
            {
                pOut = riscv_nn_mat_mult_kernel_q7_reordered(
                    wt, (q7_t *)bufferA, ch_im_out, ch_im_in * dim_kernel_x * dim_kernel_y, bias_shift, out_shift, bias, pOut);
                /* counter reset */
                pBuffer = (q7_t *)bufferA;
            }
        }
    }

    for (; i_out_y < dim_im_out_y; i_out_y++)
    {
        for (i_out_x = 0; i_out_x < dim_im_out_x; i_out_x++)
        {
            /* This part implements the im2col function */
            for (i_ker_y = i_out_y * stride_y - padding_y; i_ker_y < i_out_y * stride_y - padding_y + dim_kernel_y;
                 i_ker_y++)
            {
                for (i_ker_x = i_out_x * stride_x - padding_x; i_ker_x < i_out_x * stride_x - padding_x + dim_kernel_x;
                     i_ker_x++)
                {
                    if (i_ker_y < 0 || i_ker_y >= dim_im_in_y || i_ker_x < 0 || i_ker_x >= dim_im_in_x)
                    {
                        riscv_nn_fill_q7(0, pBuffer, ch_im_in);
                        /* memset(pBuffer, 0, sizeof(q15_t)*ch_im_in); */
                    } else
                    {
                        riscv_q7_to_q7_reordered_no_shift(
                            (q7_t *)Im_in + (i_ker_y * dim_im_in_x + i_ker_x) * ch_im_in, pBuffer, ch_im_in);
                    }
                    pBuffer += ch_im_in;
                }
            }

            if (pBuffer == (q7_t *)bufferA + 2 * ch_im_in * dim_kernel_x * dim_kernel_y)
            {
                pOut = riscv_nn_mat_mult_kernel_q7_reordered(
                    wt, (q7_t *)bufferA, ch_im_out, ch_im_in * dim_kernel_x * dim_kernel_y, bias_shift, out_shift, bias, pOut);
                /* counter reset */
                pBuffer = (q7_t *)bufferA;
            }
        }
    }

    /* check if there is left-over for compute */
    if (pBuffer != (q7_t *)bufferA)
    {
        const q7_t *pA = wt;
        int i;
        for (i = 0; i < ch_im_out; i++)
        {
            q31_t sum = ((q31_t)(bias[i]) << bias_shift) + NN_ROUND(out_shift);
            q7_t *pB = (q7_t *)bufferA;
#if defined (RISCV_MATH_VECTOR)
            uint16_t colCnt = ch_im_in * dim_kernel_x * dim_kernel_y;
            uint32_t vblkCnt = colCnt & (~RVV_OPT_THRESHOLD);
            size_t l;
            vint8m4_t vx, vz;
            vint32m1_t temp00m1;
            l = __riscv_vsetvl_e32m1(1);
            temp00m1 = __riscv_vmv_v_x_i32m1(0, l);
            for (; (l = __riscv_vsetvl_e8m4(vblkCnt)) > 0; vblkCnt -= l) {
                vx = __riscv_vle8_v_i8m4(pA, l);
                pA += l;
                vz = __riscv_vle8_v_i8m4(pB, l);
                pB += l;
                temp00m1 = __riscv_vwredsum_vs_i16m8_i32m1(__riscv_vwmul_vv_i16m8(vx, vz, l), temp00m1, l);
            }
            sum += (q31_t)__riscv_vmv_x_s_i32m1_i32(temp00m1);
            colCnt = colCnt & RVV_OPT_THRESHOLD;
#else
#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
            /* basically each time it process 4 entries */
            uint16_t  colCnt = ch_im_in * dim_kernel_x * dim_kernel_y >> 3;
            q63_t sum64 = 0;
            while (colCnt)
            {
                //pA = (const q7_t *)read_and_pad_reordered((void *)pA, &inA1, &inA2);
                q63_t inB1 = *__SIMD64(pB)++;
                q63_t inA1 = *__SIMD64(pA)++;
                sum64  = __RV_SMAQA(sum64, inA1, inB1);
                colCnt--;
            }
            sum += (q31_t)(sum64 & 0xFFFFFFFF) + (q31_t)((sum64 & 0xFFFFFFFF00000000)>>32);
            colCnt = (ch_im_in * dim_kernel_y * dim_kernel_x) & 0x7;
#else
            /* basically each time it process 4 entries */
            uint16_t colCnt = ch_im_in * dim_kernel_x * dim_kernel_y >> 2;

            while (colCnt)
            {

                q31_t inB1 = *__SIMD32(pB)++;
                q31_t inA1 = *__SIMD32(pA)++;
                sum  = __RV_SMAQA(sum, inA1, inB1);

                colCnt--;
            }
            colCnt = (ch_im_in * dim_kernel_y * dim_kernel_x) & 0x3;
#endif /* __RISCV_XLEN == 64 */

            while (colCnt)
            {
                q7_t inA1 = *pA++;
                q15_t inB1 = *pB++;
                sum += inA1 * inB1;
                colCnt--;
            }
#endif /* RISCV_MATH_DSP */
#endif /* RISCV_MATH_VECTOR */
            *pOut = (q7_t)__SSAT((sum >> out_shift), 8);
            pOut++;
        }
    }

#else
    (void)bufferA;
    /* Run the following code as reference implementation for RISC-V Core without DSP */
    int i, j, k, l, m, n;
    int conv_out;
    int in_row, in_col;

    if (ch_im_in % 4 != 0 || ch_im_out % 2 != 0)
    {
        /* check if the input dimension meets the constraints */
        return RISCV_NMSIS_NN_FAILURE;
    }

    for (i = 0; i < ch_im_out; i++)
    {
        for (j = 0; j < dim_im_out_y; j++)
        {
            for (k = 0; k < dim_im_out_x; k++)
            {
                conv_out = ((q31_t)(bias[i]) << bias_shift) + NN_ROUND(out_shift);
                for (m = 0; m < dim_kernel_y; m++)
                {
                    for (n = 0; n < dim_kernel_x; n++)
                    {
                        /* if-for implementation */
                        in_row = stride_y * j + m - padding_y;
                        in_col = stride_x * k + n - padding_x;
                        if (in_row >= 0 && in_col >= 0 && in_row < dim_im_in_y && in_col < dim_im_in_x)
                        {
                            for (l = 0; l < ch_im_in; l++)
                            {
                                conv_out += Im_in[(in_row * dim_im_in_x + in_col) * ch_im_in + l] *
                                    wt[i * ch_im_in * dim_kernel_y * dim_kernel_x + (m * dim_kernel_x + n) * ch_im_in +
                                       l];
                            }
                        }
                    }
                }
                Im_out[i + (j * dim_im_out_x + k) * ch_im_out] = (q7_t)__SSAT((conv_out >> out_shift), 8);
            }
        }
    }

#endif /* RISCV_MATH_DSP */

    /* Return to application */
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of NNConv group
 */
