/*
 * SPDX-FileCopyrightText: Copyright 2010-2019, 2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_relu6_s8.c
 * Description:  Basic s8 version of ReLU6
 *
 * $Date:        26 October 2022
 * $Revision:    V.1.0.2
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/**
 *  @ingroup Public
 */

/**
 * @addtogroup Acti
 * @{
 */

/*
 *  Basic ReLU6 function
 *
 * Refer to header file for details.
 *
 */

void riscv_relu6_s8(int8_t *data, uint16_t size)
{
    int32_t i;

#if defined(RISCV_MATH_VECTOR)
    uint16_t blkCnt = size & (~RVV_OPT_THRESHOLD);                               /* Loop counter */
    uint16_t tmp_i = blkCnt;
    size_t l;
    vint8m8_t vx;
    q7_t *px = data;
    int8_t vy1 = 0, vy2 = 6;
    for (; (l = vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l) {
        vx = vle8_v_i8m8(px, l);
        vse8_v_i8m8(px, vmin_vx_i8m8(vmax_vx_i8m8(vx, vy1, l), vy2, l), l);
        px += l;
    }
    i = tmp_i;
#else
    i = 0;
#endif

    for (i = 0; i < size; i++)
    {
        int32_t ip = data[i];

        ip = MAX(ip, 0);
        data[i] = MIN(ip, 6);
    }
}

/**
 * @} end of Acti group
 */
