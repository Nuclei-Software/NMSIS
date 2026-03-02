/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_levinson_durbin_f16.c
 * Description:  f16 version of Levinson Durbin algorithm
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


#include "dsp/filtering_functions_f16.h"

/**
  @ingroup groupFilters
 */



/**
  @addtogroup LD
  @{
 */

/**
  @brief         Levinson Durbin
  @param[in]     phi      autocovariance vector starting with lag 0 (length is nbCoefs + 1)
  @param[out]    a        autoregressive coefficients
  @param[out]    err      prediction error (variance)
  @param[in]     nbCoefs  number of autoregressive coefficients
 */



#if defined(RISCV_FLOAT16_SUPPORTED)

RISCV_DSP_ATTRIBUTE void riscv_levinson_durbin_f16(const float16_t *phi,
  float16_t *a,
  float16_t *err,
  int nbCoefs)
{
   float16_t e;

   a[0] = (float16_t)phi[1] / (float16_t)phi[0];

   e = (float16_t)phi[0] - (float16_t)phi[1] * (float16_t)a[0];
   for(int p=1; p < nbCoefs; p++)
   {
      float16_t suma=0.0f16;
      float16_t sumb=0.0f16;
      float16_t k;
      int nb,j;

      for(int i=0; i < p; i++)
      {
         suma += (float16_t)a[i] * (float16_t)phi[p - i];
         sumb += (float16_t)a[i] * (float16_t)phi[i + 1];
      }

      k = ((float16_t)phi[p+1]-suma)/((float16_t)phi[0] - sumb);


      nb = p >> 1;
      j=0;
      for(int i =0;i < nb ; i++)
      {
          float16_t x,y;

          x=(float16_t)a[j] - (float16_t)k * (float16_t)a[p-1-j];
          y=(float16_t)a[p-1-j] - (float16_t)k * (float16_t)a[j];

          a[j] = x;
          a[p-1-j] = y;

          j++;
      }

      nb = p & 1;
      if (nb)
      {
            a[j]=(float16_t)a[j]- (float16_t)k * (float16_t)a[p-1-j];
      }

      a[p] = k;
      e = e * (1.0f16 - k*k);


   }
   *err = e;
}
#endif /* defined(RISCV_FLOAT16_SUPPORTED */
/**
  @} end of LD group
 */
