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
 * Title:        riscv_pool_q7_HWC.c
 * Description:  Pooling function implementations
 *
 * $Date:        20. July 2021
 * $Revision:    V.1.1.1
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

#if defined (RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)

/**
 * @brief A few utility functions used by pooling functions
 *
 *
 */

static void buffer_scale_back_q15_to_q7(q15_t *buffer, q7_t *target, uint16_t length, uint16_t scale)
{
    int i;

#if defined (RISCV_MATH_VECTOR)
    uint16_t blkCnt = length & (~RVV_OPT_THRESHOLD);                               /* Loop counter */
    uint16_t tmp_i = blkCnt;
    size_t l;
    q15_t *pA = buffer;
    q7_t *pOut = target;

    for (; (l = vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
        vse8_v_i8m4(pOut, vnclip_wx_i8m4(vdiv_vx_i16m8(vle16_v_i16m8(pA, l), scale, l), 0, l), l);
        pA += l;
        pOut += l;
    }
	i = tmp_i;
#else
	i = 0;
#endif

    for (; i < length; i++)
    {
        target[i] = (q7_t)(buffer[i] / scale);
    }
}


static void compare_and_replace_if_larger_q7(q7_t * base,   // base data
                                             const q7_t * target,   // compare target
                                             const uint16_t length  // data length
    )
{
    q7_t *pIn = base;
    const q7_t *pCom = target;
    uint16_t cnt;

#if defined (RISCV_MATH_VECTOR)
    uint16_t blkCnt = length & (~RVV_OPT_THRESHOLD);                               /* Loop counter */
    size_t l;
    vint8m8_t a0m8, b0m8;

    for (; (l = vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l) {
        a0m8 = vle8_v_i8m8(pCom, l);
        b0m8 = vle8_v_i8m8(pIn, l);
        vse8_v_i8m8(pIn, vmax_vv_i8m8(a0m8, b0m8, l), l);
        pCom += l;
        pIn += l;
    }
    cnt = length & RVV_OPT_THRESHOLD;
#elif defined (RISCV_MATH_DSP)
    union riscv_nnword in;
    union riscv_nnword com;
    cnt = length >> 2;

    while (cnt > 0u)
    {
        in.word = *__SIMD32(pIn);
        com.word = *__SIMD32(pCom)++;
        //__SMAX8(in.word,com.word);
        // if version
       /* if (com.bytes[0] > in.bytes[0])
            in.bytes[0] = com.bytes[0];
        if (com.bytes[1] > in.bytes[1])
            in.bytes[1] = com.bytes[1];
        if (com.bytes[2] > in.bytes[2])
            in.bytes[2] = com.bytes[2];
        if (com.bytes[3] > in.bytes[3])
            in.bytes[3] = com.bytes[3];*/
        *__SIMD32(pIn)++ = __RV_SMAX8(in.word,com.word);
        ///*__SIMD32(pIn)++ = in.word;

        cnt--;
    }

    cnt = length & 0x3;
#else
	cnt = length;
#endif /*defined (RISCV_MATH_VECTOR)*/
    while (cnt > 0u)
    {
        if (*pCom > *pIn)
        {
            *pIn = *pCom;
        }
        pIn++;
        pCom++;
        cnt--;
    }

}

static void accumulate_q7_to_q15(q15_t *base, q7_t *target, const uint16_t length)
{
    q15_t *pCnt = base;
    q7_t *pV = target;
    q31_t v1, v2, vo1, vo2;
    uint16_t cnt;
    q31_t in;

#if defined (RISCV_MATH_VECTOR)
    uint32_t blkCnt = length & (~RVV_OPT_THRESHOLD);                               /* Loop counter */
    size_t l;
	vint8m4_t a0m4;
	vint16m8_t b0m8;

    for (; (l = vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l) {
        a0m4 = vle8_v_i8m4(pV, l);
        b0m8 = vle16_v_i16m8(pCnt, l);
        vse16_v_i16m8(pCnt, vwadd_wv_i16m8(b0m8, a0m4, l), l);
        pV += l;
        pCnt += l;
    }
	cnt = length & RVV_OPT_THRESHOLD;
#elif defined (RISCV_MATH_DSP)
	cnt = length >> 2;
    while (cnt > 0u)
    {

        q31_t  value = *__SIMD32(pV)++;
        v1 = __SXTB16(__ROR(value, 8));
        v2 = __SXTB16(value);

        vo2 = __RV_PKTT16(v1,v2);
        vo1 = __RV_PKBB16(v1,v2);



        in = *__SIMD32(pCnt);
        *__SIMD32(pCnt)++ = __RV_KADD16(vo1, in);

        in = *__SIMD32(pCnt);
        *__SIMD32(pCnt)++ = __RV_KADD16(vo2, in);

        cnt--;
    }
    cnt = length & 0x3;
#else
	cnt = length;
#endif
    while (cnt > 0u)
    {
        *pCnt++ += *pV++;
        cnt--;
    }
}

#endif

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup Pooling
 * @{
 */

/**
 * @brief Q7 max pooling function
 * @param[in, out]  Im_in       pointer to input tensor
 * @param[in]       dim_im_in   input tensor dimention
 * @param[in]       ch_im_in    number of input tensor channels
 * @param[in]       dim_kernel  filter kernel size
 * @param[in]       padding     padding sizes
 * @param[in]       stride      convolution stride
 * @param[in]       dim_im_out  output tensor dimension
 * @param[in,out]   bufferA     Not used
 * @param[in,out]   Im_out      pointer to output tensor
 *
 * @details
 *
 * The pooling function is implemented as split x-pooling then
 * y-pooling.
 *
 * This pooling function is input-destructive. Input data is undefined
 * after calling this function.
 *
 */

void riscv_maxpool_q7_HWC(q7_t *Im_in,
                        const uint16_t dim_im_in,
                        const uint16_t ch_im_in,
                        const uint16_t dim_kernel,
                        const uint16_t padding,
                        const uint16_t stride,
                        const uint16_t dim_im_out,
                        q7_t *bufferA,
                        q7_t *Im_out)
{
    (void)bufferA;
#if defined (RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)
    /* Run the following code for RISC-V Core with DSP enabled */

    int16_t i_x, i_y;

    /* first does the pooling along x axis */
    for (i_y = 0; i_y < dim_im_in; i_y++)
    {

        for (i_x = 0; i_x < dim_im_out; i_x++)
        {
            /* for each output pixel */
            q7_t *target = Im_in + (i_y * dim_im_in + i_x) * ch_im_in;
            q7_t *win_start;
            q7_t *win_stop;
            if (i_x * stride - padding < 0)
            {
                win_start = target;
            }
            else
            {
                win_start = Im_in + (i_y * dim_im_in + i_x * stride - padding) * ch_im_in;
            }

            if (i_x * stride - padding + dim_kernel >= dim_im_in)
            {
                win_stop = Im_in + (i_y * dim_im_in + dim_im_in) * ch_im_in;
            }
            else
            {
                win_stop = Im_in + (i_y * dim_im_in + i_x * stride - padding + dim_kernel) * ch_im_in;
            }

            /* first step is to copy over initial data */
            riscv_copy_q7(win_start, target, ch_im_in);
            /* memmove(target, win_start, ch_im_in); */

            /* start the max operation from the second part */
            win_start += ch_im_in;
            for (; win_start < win_stop; win_start += ch_im_in)
            {
                compare_and_replace_if_larger_q7(target, win_start, ch_im_in);
            }
        }
    }

    /* then does the pooling along y axis */
    for (i_y = 0; i_y < dim_im_out; i_y++)
    {

        /* for each output row */
        q7_t *target = Im_out + i_y * dim_im_out * ch_im_in;
        q7_t *row_start;
        q7_t *row_end;
        /* setting the starting row */
        if (i_y * stride - padding < 0)
        {
            row_start = Im_in;
        }
        else
        {
            row_start = Im_in + (i_y * stride - padding) * dim_im_in * ch_im_in;
        }
        /* setting the stopping row */
        if (i_y * stride - padding + dim_kernel >= dim_im_in)
        {
            row_end = Im_in + dim_im_in * dim_im_in * ch_im_in;
        }
        else
        {
            row_end = Im_in + (i_y * stride - padding + dim_kernel) * dim_im_in * ch_im_in;
        }

        /* copy over the first row */
        riscv_copy_q7(row_start, target, dim_im_out * ch_im_in);
        /* memmove(target, row_start, dim_im_out * ch_im_in); */

        /* move over to next row */
        row_start += ch_im_in * dim_im_in;

        for (; row_start < row_end; row_start += dim_im_in * ch_im_in)
        {
            compare_and_replace_if_larger_q7(target, row_start, dim_im_out * ch_im_in);
        }
    }

#else
    /* Run the following code as reference implementation for RISC-V Core without DSP */
    int16_t i_ch_in, i_x, i_y;
    int16_t k_x, k_y;

    for (i_ch_in = 0; i_ch_in < ch_im_in; i_ch_in++)
    {
        for (i_y = 0; i_y < dim_im_out; i_y++)
        {
            for (i_x = 0; i_x < dim_im_out; i_x++)
            {
                int max = -129;
                for (k_y = i_y * stride - padding; k_y < i_y * stride - padding + dim_kernel; k_y++)
                {
                    for (k_x = i_x * stride - padding; k_x < i_x * stride - padding + dim_kernel; k_x++)
                    {
                        if (k_y >= 0 && k_x >= 0 && k_y < dim_im_in && k_x < dim_im_in)
                        {
                            if (Im_in[i_ch_in + ch_im_in * (k_x + k_y * dim_im_in)] > max)
                            {
                                max = Im_in[i_ch_in + ch_im_in * (k_x + k_y * dim_im_in)];
                            }
                        }
                    }
                }
                Im_out[i_ch_in + ch_im_in * (i_x + i_y * dim_im_out)] = max;
            }
        }
    }

#endif /* RISCV_MATH_DSP */
}

/**
 * @brief Q7 average pooling function
 * @param[in,out]   Im_in       pointer to input tensor
 * @param[in]       dim_im_in   input tensor dimention
 * @param[in]       ch_im_in    number of input tensor channels
 * @param[in]       dim_kernel  filter kernel size
 * @param[in]       padding     padding sizes
 * @param[in]       stride      convolution stride
 * @param[in]       dim_im_out  output tensor dimension
 * @param[in,out]   bufferA     pointer to buffer space for input
 * @param[in,out]   Im_out      pointer to output tensor
 *
 * @details
 *
 * <b>Buffer size:</b>
 *
 * bufferA size:  2*dim_im_out*ch_im_in
 *
 * The pooling function is implemented as split x-pooling then
 * y-pooling.
 *
 * This pooling function is input-destructive. Input data is undefined
 * after calling this function.
 *
 */

void riscv_avepool_q7_HWC(q7_t *Im_in,
                        const uint16_t dim_im_in,
                        const uint16_t ch_im_in,
                        const uint16_t dim_kernel,
                        const uint16_t padding,
                        const uint16_t stride,
                        const uint16_t dim_im_out,
                        q7_t *bufferA,
                        q7_t *Im_out)
{

#if defined (RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)
    /* Run the following code for RISC-V Core with DSP enabled */

    q15_t *buffer = (q15_t *)bufferA;
    int16_t i_x, i_y;
    int16_t count = 0;

    /* first does the pooling along x axis */
    for (i_y = 0; i_y < dim_im_in; i_y++)
    {

        for (i_x = 0; i_x < dim_im_out; i_x++)
        {
            /* for each output pixel */
            q7_t *target = Im_in + (i_y * dim_im_in + i_x) * ch_im_in;
            q7_t *win_start;
            q7_t *win_stop;
            if (i_x * stride - padding < 0)
            {
                win_start = target;
            }
            else
            {
                win_start = Im_in + (i_y * dim_im_in + i_x * stride - padding) * ch_im_in;
            }

            if (i_x * stride - padding + dim_kernel >= dim_im_in)
            {
                win_stop = Im_in + (i_y * dim_im_in + dim_im_in) * ch_im_in;
            }
            else
            {
                win_stop = Im_in + (i_y * dim_im_in + i_x * stride - padding + dim_kernel) * ch_im_in;
            }

            /* first step is to copy over initial data */
            riscv_q7_to_q15_no_shift(win_start, buffer, ch_im_in);
            count = 1;

            /* start the max operation from the second part */
            win_start += ch_im_in;
            for (; win_start < win_stop; win_start += ch_im_in)
            {
                accumulate_q7_to_q15(buffer, win_start, ch_im_in);
                count++;
            }
            buffer_scale_back_q15_to_q7(buffer, target, ch_im_in, count);
        }
    }

    /* then does the pooling along y axis */
    for (i_y = 0; i_y < dim_im_out; i_y++)
    {
        /* for each output row */
        q7_t *target = Im_out + i_y * dim_im_out * ch_im_in;
        q7_t *row_start;
        q7_t *row_end;
        /* setting the starting row */
        if (i_y * stride - padding < 0)
        {
            row_start = Im_in;
        }
        else
        {
            row_start = Im_in + (i_y * stride - padding) * dim_im_in * ch_im_in;
        }
        /* setting the stopping row */
        if (i_y * stride - padding + dim_kernel >= dim_im_in)
        {
            row_end = Im_in + dim_im_in * dim_im_in * ch_im_in;
        }
        else
        {
            row_end = Im_in + (i_y * stride - padding + dim_kernel) * dim_im_in * ch_im_in;
        }

        /* copy over the first row */
        riscv_q7_to_q15_no_shift(row_start, buffer, dim_im_out * ch_im_in);
        count = 1;

        /* move over to next row */
        row_start += ch_im_in * dim_im_in;

        for (; row_start < row_end; row_start += dim_im_in * ch_im_in)
        {
            accumulate_q7_to_q15(buffer, row_start, dim_im_out * ch_im_in);
            count++;
        }
        buffer_scale_back_q15_to_q7(buffer, target, dim_im_out * ch_im_in, count);
    }

#else
    /* Run the following code as reference implementation for RISC-V Core without DSP */

    (void)bufferA;
    int16_t i_ch_in, i_x, i_y;
    int16_t k_x, k_y;

    for (i_ch_in = 0; i_ch_in < ch_im_in; i_ch_in++)
    {
        for (i_y = 0; i_y < dim_im_out; i_y++)
        {
            for (i_x = 0; i_x < dim_im_out; i_x++)
            {
                int sum = 0;
                int count = 0;
                for (k_y = i_y * stride - padding; k_y < i_y * stride - padding + dim_kernel; k_y++)
                {
                    for (k_x = i_x * stride - padding; k_x < i_x * stride - padding + dim_kernel; k_x++)
                    {
                        if (k_y >= 0 && k_x >= 0 && k_y < dim_im_in && k_x < dim_im_in)
                        {
                            sum += Im_in[i_ch_in + ch_im_in * (k_x + k_y * dim_im_in)];
                            count++;
                        }
                    }
                }
                Im_out[i_ch_in + ch_im_in * (i_x + i_y * dim_im_out)] = sum / count;
            }
        }
    }

#endif /* RISCV_MATH_DSP */
}

/**
 * @} end of Pooling group
 */
