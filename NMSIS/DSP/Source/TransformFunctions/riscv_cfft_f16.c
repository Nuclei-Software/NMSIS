/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_f32.c
 * Description:  Combined Radix Decimation in Frequency CFFT Floating point processing function
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


#include "dsp/transform_functions_f16.h"
#include "riscv_common_tables_f16.h"


#if defined(RISCV_MATH_VECTOR_FLOAT16) && defined(RISCV_FLOAT16_SUPPORTED)

RISCV_DSP_ATTRIBUTE void riscv_cfft_f16(
  const riscv_cfft_instance_f16 * S,
        const float16_t * pIn,
        float16_t * pOut,
        float16_t * pBuffer, /* When used, in is not modified */
        uint8_t ifftFlag)
{
    const long N = S->fftLen;
    float16_t *buf[2] = {pBuffer, pBuffer + N * 2};
    int buf_idx = 0; // data in buf_idx

    const float16_t *ptwd_re = S->ptwd_re;
    const float16_t *ptwd_im = S->ptwd_im;

    size_t avl = N >> 1;
    const float16_t *px = pIn;
    float16_t *py = buf[buf_idx];
    do {
        size_t vl = __riscv_vsetvl_e16m2(avl);

        vfloat16m2x2_t v_tuple = __riscv_vlseg2e16_v_f16m2x2(px, vl);
        vfloat16m2_t va_re = __riscv_vget_v_f16m2x2_f16m2(v_tuple, 0);
        vfloat16m2_t va_im = __riscv_vget_v_f16m2x2_f16m2(v_tuple, 1);
        v_tuple = __riscv_vlseg2e16_v_f16m2x2(px + N, vl);
        vfloat16m2_t vb_re = __riscv_vget_v_f16m2x2_f16m2(v_tuple, 0);
        vfloat16m2_t vb_im = __riscv_vget_v_f16m2x2_f16m2(v_tuple, 1);
        px += 2 * vl;

        if (ifftFlag) {
            va_im = __riscv_vfneg_v_f16m2(va_im, vl);
            vb_im = __riscv_vfneg_v_f16m2(vb_im, vl);
        }

        vfloat16m2_t vre0 = __riscv_vfadd_vv_f16m2(va_re, vb_re, vl);
        vfloat16m2_t vim0 = __riscv_vfadd_vv_f16m2(va_im, vb_im, vl);

        vfloat16m2_t vtmp_re = __riscv_vfsub_vv_f16m2(va_re, vb_re, vl);
        vfloat16m2_t vtmp_im = __riscv_vfsub_vv_f16m2(va_im, vb_im, vl);

        vfloat16m2_t vtwd_re = __riscv_vle16_v_f16m2(ptwd_re, vl);
        ptwd_re += vl;
        vfloat16m2_t vtwd_im = __riscv_vle16_v_f16m2(ptwd_im, vl);
        ptwd_im += vl;

        vfloat16m2_t vre1 = __riscv_vfsub_vv_f16m2(
            __riscv_vfmul_vv_f16m2(vtmp_re, vtwd_re, vl),
            __riscv_vfmul_vv_f16m2(vtmp_im, vtwd_im, vl), vl);
        vfloat16m2_t vim1 = __riscv_vfadd_vv_f16m2(
            __riscv_vfmul_vv_f16m2(vtmp_im, vtwd_re, vl),
            __riscv_vfmul_vv_f16m2(vtmp_re, vtwd_im, vl), vl);

        v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 0, vre0);
        v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 1, vre1);
        __riscv_vsseg2e16_v_f16m2x2(py, v_tuple, vl);
        v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 0, vim0);
        v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 1, vim1);
        __riscv_vsseg2e16_v_f16m2x2(py + N, v_tuple, vl);
        py += 2 * vl;

        avl -= vl;
    } while (avl != 0);

    for (int a = N >> 1, stage = 0; a > 2; a >>= 1, stage++) {
        avl = N >> 1;
        const float16_t *px = buf[buf_idx];
        float16_t *py = buf[1 - buf_idx];
            do {
                size_t vl = __riscv_vsetvl_e16m2(avl);

                vfloat16m2_t va_re = __riscv_vle16_v_f16m2(px, vl);
                vfloat16m2_t va_im = __riscv_vle16_v_f16m2(px + N, vl);
                vfloat16m2_t vb_re = __riscv_vle16_v_f16m2(px + N / 2, vl);
                vfloat16m2_t vb_im = __riscv_vle16_v_f16m2(px + N * 3 / 2, vl);
                px += vl;

                vfloat16m2_t vre0 = __riscv_vfadd_vv_f16m2(va_re, vb_re, vl);
                vfloat16m2_t vim0 = __riscv_vfadd_vv_f16m2(va_im, vb_im, vl);

                vfloat16m2_t vtmp_re = __riscv_vfsub_vv_f16m2(va_re, vb_re, vl);
                vfloat16m2_t vtmp_im = __riscv_vfsub_vv_f16m2(va_im, vb_im, vl);

                vfloat16m2_t vtwd_re = __riscv_vle16_v_f16m2(ptwd_re, vl);
                ptwd_re += vl;
                vfloat16m2_t vtwd_im = __riscv_vle16_v_f16m2(ptwd_im, vl);
                ptwd_im += vl;

                vfloat16m2_t vre1 = __riscv_vfsub_vv_f16m2(
                    __riscv_vfmul_vv_f16m2(vtmp_re, vtwd_re, vl),
                    __riscv_vfmul_vv_f16m2(vtmp_im, vtwd_im, vl), vl);
                vfloat16m2_t vim1 = __riscv_vfadd_vv_f16m2(
                    __riscv_vfmul_vv_f16m2(vtmp_re, vtwd_im, vl),
                    __riscv_vfmul_vv_f16m2(vtmp_im, vtwd_re, vl), vl);

                vfloat16m2x2_t v_tuple;
                v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 0, vre0);
                v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 1, vre1);
                __riscv_vsseg2e16_v_f16m2x2(py, v_tuple, vl);
                v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 0, vim0);
                v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 1, vim1);
                __riscv_vsseg2e16_v_f16m2x2(py + N, v_tuple, vl);
                py += 2 * vl;

                avl -= vl;
            } while (avl != 0);
        buf_idx = 1 - buf_idx;
    }

    avl = N >> 1;
    px = buf[buf_idx];
    float16_t invL = 1.0f / N;
    py = pOut;
    const uint16_t *pidx = S->pBitRevTable;
    do {
        size_t vl = __riscv_vsetvl_e16m2(avl);

        vfloat16m2_t va_re = __riscv_vle16_v_f16m2(px, vl);
        vfloat16m2_t va_im = __riscv_vle16_v_f16m2(px + N, vl);
        vfloat16m2_t vb_re = __riscv_vle16_v_f16m2(px + N / 2, vl);
        vfloat16m2_t vb_im = __riscv_vle16_v_f16m2(px + N * 3 / 2, vl);
        px += vl;

        vfloat16m2_t vre0 = __riscv_vfadd_vv_f16m2(va_re, vb_re, vl);
        vfloat16m2_t vim0 = __riscv_vfadd_vv_f16m2(va_im, vb_im, vl);

        vfloat16m2_t vre1 = __riscv_vfsub_vv_f16m2(va_re, vb_re, vl);
        vfloat16m2_t vim1 = __riscv_vfsub_vv_f16m2(va_im, vb_im, vl);

        if (ifftFlag) {
            vre0 = __riscv_vfmul_vf_f16m2(vre0, invL, vl);
            vre1 = __riscv_vfmul_vf_f16m2(vre1, invL, vl);
            vim0 = __riscv_vfmul_vf_f16m2(vim0, -invL, vl);
            vim1 = __riscv_vfmul_vf_f16m2(vim1, -invL, vl);
        }

        vuint16m2_t vidx = __riscv_vle16_v_u16m2(pidx, vl);
        vfloat16m2x2_t v_tuple;
        v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 0, vre0);
        v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 1, vim0);
        __riscv_vsoxseg2ei16_v_f16m2x2(py, vidx, v_tuple, vl);

        vidx = __riscv_vle16_v_u16m2(pidx + (N >> 1), vl);
        v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 0, vre1);
        v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 1, vim1);

        pidx += vl;
        __riscv_vsoxseg2ei16_v_f16m2x2(py, vidx, v_tuple, vl);

        avl -= vl;
    } while (avl != 0);

}
#else

#if defined(RISCV_FLOAT16_SUPPORTED)

extern void riscv_bitreversal_16(
        uint16_t * pSrc,
  const uint16_t bitRevLen,
  const uint16_t * pBitRevTable);


extern void riscv_cfft_radix4by2_f16(
    float16_t * pSrc,
    uint32_t fftLen,
    const float16_t * pCoef);

extern void riscv_radix4_butterfly_f16(
        float16_t * pSrc,
        uint16_t fftLen,
  const float16_t * pCoef,
        uint16_t twidCoefModifier);

/**
  @addtogroup ComplexFFTF16
  @{
 */

/**
  @brief         Processing function for the floating-point complex FFT.
  @param[in]     S              points to an instance of the floating-point CFFT structure
  @param[in,out] p1             points to the complex data buffer. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".
 */

RISCV_DSP_ATTRIBUTE void riscv_cfft_f16(
    const riscv_cfft_instance_f16 * S,
    float16_t * p1,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag)
{
    uint32_t  L = S->fftLen, l;
    float16_t invL, * pSrc;

    if (ifftFlag == 1U)
    {
        /*  Conjugate input data  */
        pSrc = p1 + 1;
        for(l=0; l<L; l++)
        {
            *pSrc = -(_Float16)*pSrc;
            pSrc += 2;
        }
    }

    switch (L)
    {

        case 16:
        case 64:
        case 256:
        case 1024:
        case 4096:
        riscv_radix4_butterfly_f16  (p1, L, (float16_t*)S->pTwiddle, 1U);
        break;

        case 32:
        case 128:
        case 512:
        case 2048:
        riscv_cfft_radix4by2_f16  ( p1, L, (float16_t*)S->pTwiddle);
        break;

    }

    if ( bitReverseFlag )
        riscv_bitreversal_16((uint16_t*)p1, S->bitRevLength,(uint16_t*)S->pBitRevTable);

    if (ifftFlag == 1U)
    {
        invL = 1.0f16/(_Float16)L;
        /*  Conjugate and scale output data */
        pSrc = p1;
        for(l=0; l<L; l++)
        {
            *pSrc++ *=   (_Float16)invL ;
            *pSrc  = -(_Float16)(*pSrc) * (_Float16)invL;
            pSrc++;
        }
    }
}
#endif /* if defined(RISCV_FLOAT16_SUPPORTED) */
#endif /* defined(RISCV_MATH_MVEF) && !defined(RISCV_MATH_AUTOVECTORIZE) */

/**
  @} end of ComplexFFTF16 group
 */
