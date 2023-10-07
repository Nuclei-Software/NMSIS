/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_radix2_f32.c
 * Description:  Radix-2 Decimation in Frequency CFFT & CIFFT Floating point processing function
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

#include "dsp/transform_functions.h"

#if defined (RISCV_MATH_VECTOR)
#define FFT_DOT 1024

//bit reverse table for vector
uint16_t bitrevIndexGrp [FFT_DOT] __attribute__((aligned(16)))= {0};
#endif /* defined (RISCV_MATH_VECTOR) */

void riscv_radix2_butterfly_f32(
        float32_t * pSrc,
        uint32_t fftLen,
  const float32_t * pCoef,
        uint16_t twidCoefModifier);

void riscv_radix2_butterfly_inverse_f32(
        float32_t * pSrc,
        uint32_t fftLen,
  const float32_t * pCoef,
        uint16_t twidCoefModifier,
        float32_t onebyfftLen);

extern void riscv_bitreversal_f32(
        float32_t * pSrc,
        uint16_t fftSize,
        uint16_t bitRevFactor,
  const uint16_t * pBitRevTab);



/**
  @addtogroup ComplexFFTDeprecated
  @{
 */

/**
  @brief         Radix-2 CFFT/CIFFT.
  @deprecated    Do not use this function. It has been superseded by \ref riscv_cfft_f32 and will be removed in the future
  @param[in]     S    points to an instance of the floating-point Radix-2 CFFT/CIFFT structure
  @param[in,out] pSrc points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place
  @return        none
 */

void riscv_cfft_radix2_f32(
const riscv_cfft_radix2_instance_f32 * S,
      float32_t * pSrc)
{

   if (S->ifftFlag == 1U)
   {
      /* Complex IFFT radix-2 */
      riscv_radix2_butterfly_inverse_f32(pSrc, S->fftLen, S->pTwiddle,
      S->twidCoefModifier, S->onebyfftLen);
   }
   else
   {
      /* Complex FFT radix-2 */
      riscv_radix2_butterfly_f32(pSrc, S->fftLen, S->pTwiddle,
      S->twidCoefModifier);
   }

   if (S->bitReverseFlag == 1U)
   {
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64))
#else
      /* Bit Reversal */
      riscv_bitreversal_f32(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
#endif
   }

}


/**
  @} end of ComplexFFTDeprecated group
 */



/* ----------------------------------------------------------------------
 ** Internal helper function used by the FFTs
 ** ------------------------------------------------------------------- */

/**
  brief  Core function for the floating-point CFFT butterfly process.
  param[in,out] pSrc             points to in-place buffer of floating-point data type
  param[in]     fftLen           length of the FFT
  param[in]     pCoef            points to twiddle coefficient buffer
  param[in]     twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
  return        none
 */

void riscv_radix2_butterfly_f32(
        float32_t * pSrc,
        uint32_t fftLen,
  const float32_t * pCoef,
        uint16_t twidCoefModifier)
{

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64))

    size_t stage_loop_count, group_count, group_total, group_size, butterfly_left, bufferfly_total;

    //length for each bufferfly compute
    size_t vl_bufferfly;

    ptrdiff_t bstride_bufferfly_pCoef;

    float32_t *bufferfly_pCoef, *bufferfly_group_a_src, *bufferfly_group_b_src;

    // bufferfly compute source
    vfloat32m2x2_t v_tuple;
    vfloat32m2x4_t v_tuple2;
    vfloat32m2_t  Coef_real_cos, Coef_imag_sin, group_a_real, group_a_imag,group_b_real, group_b_imag;

    // bufferfly compute result
    vfloat32m2_t bufferfly_result_a_real, bufferfly_result_a_imag, bufferfly_result_b_real, bufferfly_result_b_imag;

    // bufferfly compute temp
    vfloat32m2_t bufferfly_temp1, bufferfly_temp2, bufferfly_temp3, bufferfly_temp4, bufferfly_temp5, bufferfly_temp6;

    // loop for stage
    stage_loop_count = fftLen;

    for (; stage_loop_count > 4; stage_loop_count = stage_loop_count >> 1)
    {
        // group_total means we should do how many bufferfly for this stage
        group_total = (fftLen / stage_loop_count);
        group_count = 0;

        bufferfly_total = (fftLen >> 1) / group_total;
        butterfly_left = bufferfly_total;

        // group size means each group has how many points to compute
        group_size = stage_loop_count >> 1;

        // each stage ,initialize the two group source to the first element of the source array
        bufferfly_group_a_src = pSrc;
        bufferfly_group_b_src = pSrc + group_size * 2;

        // as we use LMUL =2 here, because the complex compute has two source (two real & two imag ) and one coef (one real & one imag)
        // so we don't want the compiler to backup the temporary vector result to stack
        vl_bufferfly = __riscv_vsetvl_e32m2(group_size);

        bufferfly_pCoef = (float32_t *)pCoef;

        // pCoef size is SIZE_COEF, each element is 8 Bytes (real is f32, imag is f32 )
        //bstride_bufferfly_pCoef = ((FFT_DOT*2)/bufferfly_total)*4*2;
        bstride_bufferfly_pCoef =  twidCoefModifier * 4 * 2;

        // the coef load should move to outside,each group share the same coef
        // one segment stride load for Coef , result 1 is Cos, Real part of Coef , result 2 is  Sin, imag part of Coef
        // void vlsseg2e32_v_f32m2 (vfloat32m2_t *v0, vfloat32m2_t *v1, const float32_t *base, ptrdiff_t bstride, size_t vl);
        //vlsseg2e32_v_f32m2 (&Coef_real_cos, &Coef_imag_sin, bufferfly_pCoef, bstride_bufferfly_pCoef, vl_bufferfly);

        v_tuple = __riscv_vlsseg2e32_v_f32m2x2 (bufferfly_pCoef, bstride_bufferfly_pCoef, vl_bufferfly);
        Coef_real_cos = __riscv_vget_v_f32m2x2_f32m2 (v_tuple, 0);
        Coef_imag_sin = __riscv_vget_v_f32m2x2_f32m2 (v_tuple, 1);
        do
        {

            for(; group_count < group_total; group_count ++ )
            {
                // one segment load for group a,  result 1 is real part of group a, result 2 is imag part of group a
                //vlseg2e32_v_f32m2(&group_a_real, &group_a_imag, bufferfly_group_a_src, vl_bufferfly);
                v_tuple = __riscv_vlseg2e32_v_f32m2x2 (bufferfly_group_a_src, vl_bufferfly);
                group_a_real = __riscv_vget_v_f32m2x2_f32m2 (v_tuple, 0);
                group_a_imag = __riscv_vget_v_f32m2x2_f32m2 (v_tuple, 1);
                // one segment load for group b, result 1 is real part of group b, result 2 is imag part of group b
                //vlseg2e32_v_f32m2(&group_b_real, &group_b_imag, bufferfly_group_b_src, vl_bufferfly);
                v_tuple = __riscv_vlseg2e32_v_f32m2x2 (bufferfly_group_b_src, vl_bufferfly);
                group_b_real = __riscv_vget_v_f32m2x2_f32m2 (v_tuple, 0);
                group_b_imag = __riscv_vget_v_f32m2x2_f32m2 (v_tuple, 1);

                // bufferfly compute

                // group_a_real + group_b_real is result_a_real
                bufferfly_result_a_real = __riscv_vfadd_vv_f32m2(group_a_real, group_b_real, vl_bufferfly);

                // group_a_imag + group_b_imag is result_a_imag
                bufferfly_result_a_imag = __riscv_vfadd_vv_f32m2(group_a_imag, group_b_imag, vl_bufferfly);

                // store
                // segment store bufferfly_result_a_real and bufferfly_result_a_imag
                //vsseg2e32_v_f32m2(bufferfly_group_a_src, bufferfly_result_a_real, bufferfly_result_a_imag, vl_bufferfly);
                v_tuple = __riscv_vset_v_f32m2_f32m2x2(v_tuple, 0, bufferfly_result_a_real);
                v_tuple = __riscv_vset_v_f32m2_f32m2x2(v_tuple, 1, bufferfly_result_a_imag);
                __riscv_vsseg2e32_v_f32m2x2 (bufferfly_group_a_src, v_tuple, vl_bufferfly);
                // temp1 = group_a_real - group_b_real
                bufferfly_temp1 = __riscv_vfsub_vv_f32m2(group_a_real, group_b_real, vl_bufferfly);

                // temp2 = group_a_imag - group_b_imag
                bufferfly_temp2 = __riscv_vfsub_vv_f32m2(group_a_imag, group_b_imag, vl_bufferfly);

                // temp3 = temp1 * coef_real_cos
                bufferfly_temp3 = __riscv_vfmul_vv_f32m2(bufferfly_temp1, Coef_real_cos, vl_bufferfly);

                // temp4 = temp2 * coef_imag_sin
                bufferfly_temp4 = __riscv_vfmul_vv_f32m2(bufferfly_temp2, Coef_imag_sin, vl_bufferfly);

                // temp5 = temp2 * coef_real_cos
                bufferfly_temp5 = __riscv_vfmul_vv_f32m2(bufferfly_temp2, Coef_real_cos, vl_bufferfly);

                // temp6 = temp1 * coef_imag_sin
                bufferfly_temp6 = __riscv_vfmul_vv_f32m2(bufferfly_temp1, Coef_imag_sin, vl_bufferfly);

                // result_b_real = temp3 + temp4;
                bufferfly_result_b_real = __riscv_vfadd_vv_f32m2(bufferfly_temp3, bufferfly_temp4, vl_bufferfly);

                // result_b_imag = temp5 - temp6;
                bufferfly_result_b_imag = __riscv_vfsub_vv_f32m2(bufferfly_temp5, bufferfly_temp6, vl_bufferfly);

                // store
                // segment store bufferfly_result_a_real and bufferfly_result_a_imag
                //vsseg2e32_v_f32m2(bufferfly_group_b_src,bufferfly_result_b_real,bufferfly_result_b_imag,vl_bufferfly);
                v_tuple = __riscv_vset_v_f32m2_f32m2x2(v_tuple, 0, bufferfly_result_b_real);
                v_tuple = __riscv_vset_v_f32m2_f32m2x2(v_tuple, 1, bufferfly_result_b_imag);
                __riscv_vsseg2e32_v_f32m2x2 (bufferfly_group_b_src, v_tuple, vl_bufferfly);

                // update group src pointer for next loop

                // gourp_a pointer
                bufferfly_group_a_src += (2 * group_size) * 2;
                // group_b pointer
                bufferfly_group_b_src += (2 * group_size) * 2;

            } // for loop end for all group with current coef

            // update address, vl, coef  for next if needed
            butterfly_left -= vl_bufferfly;
            if (butterfly_left > 0)
            {
                // update vl
                vl_bufferfly = __riscv_vsetvl_e32m2(butterfly_left);
                // coef update, should use bstride_bufferfly_pCoef here
                bufferfly_pCoef += vl_bufferfly * (bstride_bufferfly_pCoef >> 2);
                // stride segment load coef for next compute
                //vlsseg2e32_v_f32m2(&Coef_real_cos, &Coef_imag_sin, bufferfly_pCoef, bstride_bufferfly_pCoef, vl_bufferfly);
                v_tuple = __riscv_vlsseg2e32_v_f32m2x2(bufferfly_pCoef, bstride_bufferfly_pCoef, vl_bufferfly);
                Coef_real_cos = __riscv_vget_v_f32m2x2_f32m2 (v_tuple, 0);
                Coef_imag_sin = __riscv_vget_v_f32m2x2_f32m2 (v_tuple, 1);
                // gourp_a_src & bufferfly_group_b_src update , each element is 8 byte
                // each stage ,initialize the two group source to the first element of the source array
                bufferfly_group_a_src = pSrc;
                bufferfly_group_b_src = pSrc + group_size * 2;

                //bufferfly_group_a_src += vl_bufferfly*2;
                //bufferfly_group_b_src += vl_bufferfly*2;
                bufferfly_group_a_src += (bufferfly_total - butterfly_left) * 2;
                bufferfly_group_b_src += (bufferfly_total - butterfly_left) * 2;
                // reset the group count
                group_count = 0;

            }

        } while(butterfly_left > 0); // loop end for all bufferfly of current stage

        // update the twidCoefModifier for next stage
        twidCoefModifier <<= 1U;

    } // loop end for stage

    // handle the second to last stage, using pure c
    if(4 >= stage_loop_count)
    {
        float32_t coef_real_cos, coef_imag_sin, coef_2_real_cos, coef_2_imag_sin;

        float32_t src1_real, src1_imag, src2_real, src2_imag;

        float32_t dest1_real, dest1_imag, dest2_real, dest2_imag;

        float32_t temp1, temp2, temp3, temp4, temp5, temp6;

        float32_t *psrc1, *psrc2;

        for (; stage_loop_count > 2; stage_loop_count = stage_loop_count >>1)
        {
            // group_total means we should do how many bufferfly for this stage
            group_total = (fftLen / stage_loop_count);

            bufferfly_total = (fftLen >> 1) / group_total;
            butterfly_left = bufferfly_total;

            // group size means each group has how many points to compute
            group_size = stage_loop_count >> 1;

            size_t group_size_cnt = 0;

            for (; group_size_cnt < group_size; group_size_cnt ++)
            {
                coef_real_cos =  pCoef[(group_size_cnt * twidCoefModifier) * 2];
                coef_imag_sin = pCoef[(group_size_cnt * twidCoefModifier) * 2 + 1];

                psrc1 = pSrc + group_size_cnt * group_size;
                psrc2 = pSrc + group_size_cnt * group_size + 2 * group_size;

                group_count = 0;

                for (; group_count < group_total; group_count++)
                {
                    src1_real = psrc1[4 * group_count * group_size];
                    src1_imag = psrc1[4 * group_count * group_size + 1];

                    src2_real = psrc2[4 * group_count * group_size];
                    src2_imag = psrc2[4 * group_count * group_size + 1];

                    dest1_real = src1_real + src2_real;
                    dest1_imag = src1_imag + src2_imag;

                    temp1 = src1_real - src2_real;

                    temp2 = src1_imag - src2_imag;

                    temp3 = temp1 * coef_real_cos;

                    temp4 = temp2 * coef_imag_sin;

                    temp5 = temp2 * coef_real_cos;

                    temp6 = temp1 * coef_imag_sin;

                    dest2_real = temp3 + temp4;

                    dest2_imag = temp5 - temp6;

                    psrc1[4 * group_count * group_size] = dest1_real;
                    psrc1[4 * group_count * group_size + 1] = dest1_imag;

                    psrc2[4 * group_count * group_size] = dest2_real;
                    psrc2[4 * group_count * group_size + 1] = dest2_imag;

                }
            }

            twidCoefModifier >>= 1U;
        }

    }

    // handle the last layer, still using vector, but the coef is in scalar floating gpr
    // and also do bit reverse too
    if(2 == stage_loop_count)
    {
        float32_t coef_real_cos, coef_imag_sin;
        float32_t *psrc1, *pdst1;

        // group_total means we should do how many bufferfly for this stage
        group_total = fftLen / 2;
        coef_real_cos =  pCoef[0];
        coef_imag_sin =  pCoef[1];

        float32_t  pdst_copy[2 * fftLen];

        psrc1 = pSrc;
        pdst1 = pdst_copy;

        for (; (vl_bufferfly = __riscv_vsetvl_e32m2(group_total)) > 0; group_total -= vl_bufferfly)
        {
            // segment nf4 load
            //vlseg4e32_v_f32m2(&group_a_real, &group_a_imag, &group_b_real, &group_b_imag, psrc1, vl_bufferfly);
            v_tuple2 = __riscv_vlseg4e32_v_f32m2x4 (psrc1, vl_bufferfly);
            group_a_real = __riscv_vget_v_f32m2x4_f32m2(v_tuple2, 0);
            group_a_imag = __riscv_vget_v_f32m2x4_f32m2(v_tuple2, 1);
            group_b_real = __riscv_vget_v_f32m2x4_f32m2(v_tuple2, 2);
            group_b_imag = __riscv_vget_v_f32m2x4_f32m2(v_tuple2, 3);
            psrc1 += 4 * vl_bufferfly;

            // temp1 = group_a_real - group_b_real
            bufferfly_temp1 = __riscv_vfsub_vv_f32m2(group_a_real, group_b_real, vl_bufferfly);

            // temp2 = group_a_imag - group_b_imag
            bufferfly_temp2 = __riscv_vfsub_vv_f32m2(group_a_imag, group_b_imag, vl_bufferfly);

            // temp3 = temp1 * coef_real_cos
            bufferfly_temp3 = __riscv_vfmul_vf_f32m2(bufferfly_temp1, coef_real_cos, vl_bufferfly);

            // temp4 = temp2 * coef_imag_sin
            bufferfly_temp4 = __riscv_vfmul_vf_f32m2(bufferfly_temp2, coef_imag_sin, vl_bufferfly);

            // temp5 = temp2 * coef_real_cos
            bufferfly_temp5 = __riscv_vfmul_vf_f32m2(bufferfly_temp2, coef_real_cos, vl_bufferfly);

            // temp6 = temp1 * coef_imag_sin
            bufferfly_temp6 = __riscv_vfmul_vf_f32m2(bufferfly_temp1, coef_imag_sin, vl_bufferfly);

            // result_b_real = temp3 + temp4;
            bufferfly_result_b_real = __riscv_vfadd_vv_f32m2(bufferfly_temp3, bufferfly_temp4, vl_bufferfly);

            // result_b_imag = temp5 - temp6;
            bufferfly_result_b_imag = __riscv_vfsub_vv_f32m2(bufferfly_temp5, bufferfly_temp6, vl_bufferfly);

            // group_a_real + group_b_real is result_a_real
            bufferfly_result_a_real = __riscv_vfadd_vv_f32m2(group_a_real, group_b_real, vl_bufferfly);

            // group_a_imag + group_b_imag is result_a_imag
            bufferfly_result_a_imag = __riscv_vfadd_vv_f32m2(group_a_imag, group_b_imag, vl_bufferfly);

            /*segment nf4 store*/
            //vsseg4e32_v_f32m2(pdst1, bufferfly_result_a_real, bufferfly_result_a_imag, bufferfly_result_b_real, bufferfly_result_b_imag, vl_bufferfly);
            v_tuple2 = __riscv_vset_v_f32m2_f32m2x4 (v_tuple2, 0, bufferfly_result_a_real);
            v_tuple2 = __riscv_vset_v_f32m2_f32m2x4 (v_tuple2, 1, bufferfly_result_a_imag);
            v_tuple2 = __riscv_vset_v_f32m2_f32m2x4 (v_tuple2, 2, bufferfly_result_b_real);
            v_tuple2 = __riscv_vset_v_f32m2_f32m2x4 (v_tuple2, 3, bufferfly_result_b_imag);
            __riscv_vsseg4e32_v_f32m2x4 (pdst1, v_tuple2, vl_bufferfly);
            pdst1 += 4 * vl_bufferfly;
        }

        size_t bit_reverse_vl;
        uint16_t * pBitRevIndex;
        vuint16m2_t v_rev_index;
        vfloat64m8_t v_dot;

        pBitRevIndex = bitrevIndexGrp;
        //pdst1 = pdst_copy;
        float64_t *pcopy_src, *pcopy_dst;
        pcopy_src =  (float64_t *)pdst_copy;
        pcopy_dst = (float64_t *)pSrc;

        for (; (bit_reverse_vl = __riscv_vsetvl_e64m8(fftLen)) > 0; fftLen -= bit_reverse_vl)
        {
            //index seg load
            v_rev_index = __riscv_vle16_v_u16m2(pBitRevIndex, bit_reverse_vl);
            pBitRevIndex += bit_reverse_vl;
            //index load, so the src pointer should not update here
            v_dot = __riscv_vloxei16_v_f64m8(pcopy_src, v_rev_index, bit_reverse_vl);
            /*unit-stride store*/
            __riscv_vse64_v_f64m8(pcopy_dst,v_dot,bit_reverse_vl);
            pcopy_dst += bit_reverse_vl ;
        }

    }  /* #if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64)) */

#else

   uint32_t i, j, k, l;
   uint32_t n1, n2, ia;
   float32_t xt, yt, cosVal, sinVal;
   float32_t p0, p1, p2, p3;
   float32_t a0, a1;
#if defined (RISCV_MATH_DSP)

   /*  Initializations for the first stage */
   n2 = fftLen >> 1;
   ia = 0;
   i = 0;

   // loop for groups
   for (k = n2; k > 0; k--)
   {
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];

      /*  Twiddle coefficients index modifier */
      ia += twidCoefModifier;

      /*  index calculation for the input as, */
      /*  pSrc[i + 0], pSrc[i + fftLen/1] */
      l = i + n2;

      /*  Butterfly implementation */
      a0 = pSrc[2 * i] + pSrc[2 * l];
      xt = pSrc[2 * i] - pSrc[2 * l];

      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
      a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];

      p0 = xt * cosVal;
      p1 = yt * sinVal;
      p2 = yt * cosVal;
      p3 = xt * sinVal;

      pSrc[2 * i] = a0;
      pSrc[2 * i + 1] = a1;

      pSrc[2 * l] = p0 + p1;
      pSrc[2 * l + 1] = p2 - p3;

      i++;
   }                             // groups loop end

   twidCoefModifier <<= 1U;

   // loop for stage
   for (k = n2; k > 2; k = k >> 1)
   {
      n1 = n2;
      n2 = n2 >> 1;
      ia = 0;

      // loop for groups
      j = 0;
      do
      {
         cosVal = pCoef[ia * 2];
         sinVal = pCoef[(ia * 2) + 1];
         ia += twidCoefModifier;

         // loop for butterfly
         i = j;
         do
         {
            l = i + n2;
            a0 = pSrc[2 * i] + pSrc[2 * l];
            xt = pSrc[2 * i] - pSrc[2 * l];

            yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
            a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];

            p0 = xt * cosVal;
            p1 = yt * sinVal;
            p2 = yt * cosVal;
            p3 = xt * sinVal;

            pSrc[2 * i] = a0;
            pSrc[2 * i + 1] = a1;

            pSrc[2 * l]     = p0 + p1;
            pSrc[2 * l + 1] = p2 - p3;

            i += n1;
         } while ( i < fftLen );                        // butterfly loop end
         j++;
      } while ( j < n2);                          // groups loop end
      twidCoefModifier <<= 1U;
   }                             // stages loop end

   // loop for butterfly
   for (i = 0; i < fftLen; i += 2)
   {
      a0 = pSrc[2 * i] + pSrc[2 * i + 2];
      xt = pSrc[2 * i] - pSrc[2 * i + 2];

      yt = pSrc[2 * i + 1] - pSrc[2 * i + 3];
      a1 = pSrc[2 * i + 3] + pSrc[2 * i + 1];

      pSrc[2 * i] = a0;
      pSrc[2 * i + 1] = a1;
      pSrc[2 * i + 2] = xt;
      pSrc[2 * i + 3] = yt;
   }                             // groups loop end

#else /* #if defined (RISCV_MATH_DSP) */

   n2 = fftLen;

   // loop for stage
   for (k = fftLen; k > 1; k = k >> 1)
   {
      n1 = n2;
      n2 = n2 >> 1;
      ia = 0;

      // loop for groups
      j = 0;
      do
      {
         cosVal = pCoef[ia * 2];
         sinVal = pCoef[(ia * 2) + 1];
         ia += twidCoefModifier;

         // loop for butterfly
         i = j;
         do
         {
            l = i + n2;
            a0 = pSrc[2 * i] + pSrc[2 * l];
            xt = pSrc[2 * i] - pSrc[2 * l];

            yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
            a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];

            p0 = xt * cosVal;
            p1 = yt * sinVal;
            p2 = yt * cosVal;
            p3 = xt * sinVal;

            pSrc[2 * i] = a0;
            pSrc[2 * i + 1] = a1;

            pSrc[2 * l]     = p0 + p1;
            pSrc[2 * l + 1] = p2 - p3;

            i += n1;
         } while (i < fftLen);
         j++;
      } while (j < n2);
      twidCoefModifier <<= 1U;
   }
#endif /* #if defined (RISCV_MATH_DSP) */
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64)) */

}


void riscv_radix2_butterfly_inverse_f32(
        float32_t * pSrc,
        uint32_t fftLen,
  const float32_t * pCoef,
        uint16_t twidCoefModifier,
        float32_t onebyfftLen)
{
   uint32_t i, j, k, l;
   uint32_t n1, n2, ia;
   float32_t xt, yt, cosVal, sinVal;
   float32_t p0, p1, p2, p3;
   float32_t a0, a1;

#if defined (RISCV_MATH_DSP)

   n2 = fftLen >> 1;
   ia = 0;

   // loop for groups
   for (i = 0; i < n2; i++)
   {
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];
      ia += twidCoefModifier;

      l = i + n2;
      a0 = pSrc[2 * i] + pSrc[2 * l];
      xt = pSrc[2 * i] - pSrc[2 * l];

      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
      a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];

      p0 = xt * cosVal;
      p1 = yt * sinVal;
      p2 = yt * cosVal;
      p3 = xt * sinVal;

      pSrc[2 * i] = a0;
      pSrc[2 * i + 1] = a1;

      pSrc[2 * l]     = p0 - p1;
      pSrc[2 * l + 1] = p2 + p3;
   }                             // groups loop end

   twidCoefModifier <<= 1U;

   // loop for stage
   for (k = fftLen / 2; k > 2; k = k >> 1)
   {
      n1 = n2;
      n2 = n2 >> 1;
      ia = 0;

      // loop for groups
      j = 0;
      do
      {
         cosVal = pCoef[ia * 2];
         sinVal = pCoef[(ia * 2) + 1];
         ia += twidCoefModifier;

         // loop for butterfly
         i = j;
         do
         {
            l = i + n2;
            a0 = pSrc[2 * i] + pSrc[2 * l];
            xt = pSrc[2 * i] - pSrc[2 * l];

            yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
            a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];

            p0 = xt * cosVal;
            p1 = yt * sinVal;
            p2 = yt * cosVal;
            p3 = xt * sinVal;

            pSrc[2 * i] = a0;
            pSrc[2 * i + 1] = a1;

            pSrc[2 * l] = p0 - p1;
            pSrc[2 * l + 1] = p2 + p3;

            i += n1;
         } while ( i < fftLen );                 // butterfly loop end
         j++;
      } while (j < n2);                      // groups loop end

      twidCoefModifier <<= 1U;
   }                             // stages loop end

   // loop for butterfly
   for (i = 0; i < fftLen; i += 2)
   {
      a0 = pSrc[2 * i] + pSrc[2 * i + 2];
      xt = pSrc[2 * i] - pSrc[2 * i + 2];

      a1 = pSrc[2 * i + 3] + pSrc[2 * i + 1];
      yt = pSrc[2 * i + 1] - pSrc[2 * i + 3];

      p0 = a0 * onebyfftLen;
      p2 = xt * onebyfftLen;
      p1 = a1 * onebyfftLen;
      p3 = yt * onebyfftLen;

      pSrc[2 * i] = p0;
      pSrc[2 * i + 1] = p1;
      pSrc[2 * i + 2] = p2;
      pSrc[2 * i + 3] = p3;
   }                             // butterfly loop end

#else /* #if defined (RISCV_MATH_DSP) */

   n2 = fftLen;

   // loop for stage
   for (k = fftLen; k > 2; k = k >> 1)
   {
      n1 = n2;
      n2 = n2 >> 1;
      ia = 0;

      // loop for groups
      j = 0;
      do
      {
         cosVal = pCoef[ia * 2];
         sinVal = pCoef[(ia * 2) + 1];
         ia = ia + twidCoefModifier;

         // loop for butterfly
         i = j;
         do
         {
            l = i + n2;
            a0 = pSrc[2 * i] + pSrc[2 * l];
            xt = pSrc[2 * i] - pSrc[2 * l];

            yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
            a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];

            p0 = xt * cosVal;
            p1 = yt * sinVal;
            p2 = yt * cosVal;
            p3 = xt * sinVal;

            pSrc[2 * i] = a0;
            pSrc[2 * i + 1] = a1;

            pSrc[2 * l]     = p0 - p1;
            pSrc[2 * l + 1] = p2 + p3;

            i += n1;
         } while ( i < fftLen );                    // butterfly loop end
         j++;
      } while ( j < n2 );                      // groups loop end

      twidCoefModifier = twidCoefModifier << 1U;
   }                             // stages loop end

   n1 = n2;
   n2 = n2 >> 1;

   // loop for butterfly
   for (i = 0; i < fftLen; i += n1)
   {
      l = i + n2;

      a0 = pSrc[2 * i] + pSrc[2 * l];
      xt = pSrc[2 * i] - pSrc[2 * l];

      a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];
      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];

      p0 = a0 * onebyfftLen;
      p2 = xt * onebyfftLen;
      p1 = a1 * onebyfftLen;
      p3 = yt * onebyfftLen;

      pSrc[2 * i] = p0;
      pSrc[2 * l] = p2;

      pSrc[2 * i + 1] = p1;
      pSrc[2 * l + 1] = p3;
   }                             // butterfly loop end

#endif /* #if defined (RISCV_MATH_DSP) */

}
