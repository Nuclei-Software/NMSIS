/*
 * SPDX-FileCopyrightText: Copyright 2010-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * Copyright (c) 2025 Nuclei Limited. All rights reserved.
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

#include <riscv_nnfunctions.h>
#include <stdlib.h>
#include <unity.h>

#include "../TestData/maximum_broadcast_batch_int8/test_data.h"
#include "../TestData/maximum_broadcast_ch_int8/test_data.h"
#include "../TestData/maximum_broadcast_height_int8/test_data.h"
#include "../TestData/maximum_broadcast_width_int8/test_data.h"
#include "../TestData/maximum_no_broadcast_int8/test_data.h"
#include "../TestData/maximum_scalar_1_int8/test_data.h"
#include "../TestData/maximum_scalar_2_int8/test_data.h"

#include "../Utils/validate.h"

void maximum_scalar_1_int8(void)
{
    nmsis_nn_context ctx;

    int8_t output[MAXIMUM_SCALAR_1_INT8_DST_SIZE] = {0};

    int buf_size = 0;
    ctx.buf = malloc(buf_size);
    ctx.size = 0;

    nmsis_nn_dims input_1_dims;
    nmsis_nn_dims input_2_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_1_data = maximum_scalar_1_int8_input_tensor_1;
    const int8_t *input_2_data = maximum_scalar_1_int8_input_tensor_2;
    const int8_t *output_ref_data = maximum_scalar_1_int8_output;

    input_1_dims.n = MAXIMUM_SCALAR_1_INT8_BATCH_1;
    input_1_dims.h = MAXIMUM_SCALAR_1_INT8_HEIGHT_1;
    input_1_dims.w = MAXIMUM_SCALAR_1_INT8_WIDTH_1;
    input_1_dims.c = MAXIMUM_SCALAR_1_INT8_CHANNEL_1;

    input_2_dims.n = MAXIMUM_SCALAR_1_INT8_BATCH_2;
    input_2_dims.h = MAXIMUM_SCALAR_1_INT8_HEIGHT_2;
    input_2_dims.w = MAXIMUM_SCALAR_1_INT8_WIDTH_2;
    input_2_dims.c = MAXIMUM_SCALAR_1_INT8_CHANNEL_2;

    output_dims.n = MAXIMUM_SCALAR_1_INT8_OUTPUT_BATCH;
    output_dims.h = MAXIMUM_SCALAR_1_INT8_OUTPUT_HEIGHT;
    output_dims.w = MAXIMUM_SCALAR_1_INT8_OUTPUT_WIDTH;
    output_dims.c = MAXIMUM_SCALAR_1_INT8_OUTPUT_CHANNEL;

    riscv_nmsis_nn_status result =
        riscv_maximum_s8(&ctx, input_1_data, &input_1_dims, input_2_data, &input_2_dims, output, &output_dims);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    int dst_size = MAXIMUM_SCALAR_1_INT8_DST_SIZE;
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate(output, output_ref_data, dst_size));
}

void maximum_scalar_2_int8(void)
{
    nmsis_nn_context ctx;

    int8_t output[MAXIMUM_SCALAR_2_INT8_DST_SIZE] = {0};

    int buf_size = 0;
    ctx.buf = malloc(buf_size);
    ctx.size = 0;

    nmsis_nn_dims input_1_dims;
    nmsis_nn_dims input_2_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_1_data = maximum_scalar_2_int8_input_tensor_1;
    const int8_t *input_2_data = maximum_scalar_2_int8_input_tensor_2;
    const int8_t *output_ref_data = maximum_scalar_2_int8_output;

    input_1_dims.n = MAXIMUM_SCALAR_2_INT8_BATCH_1;
    input_1_dims.h = MAXIMUM_SCALAR_2_INT8_HEIGHT_1;
    input_1_dims.w = MAXIMUM_SCALAR_2_INT8_WIDTH_1;
    input_1_dims.c = MAXIMUM_SCALAR_2_INT8_CHANNEL_1;

    input_2_dims.n = MAXIMUM_SCALAR_2_INT8_BATCH_2;
    input_2_dims.h = MAXIMUM_SCALAR_2_INT8_HEIGHT_2;
    input_2_dims.w = MAXIMUM_SCALAR_2_INT8_WIDTH_2;
    input_2_dims.c = MAXIMUM_SCALAR_2_INT8_CHANNEL_2;

    output_dims.n = MAXIMUM_SCALAR_2_INT8_OUTPUT_BATCH;
    output_dims.h = MAXIMUM_SCALAR_2_INT8_OUTPUT_HEIGHT;
    output_dims.w = MAXIMUM_SCALAR_2_INT8_OUTPUT_WIDTH;
    output_dims.c = MAXIMUM_SCALAR_2_INT8_OUTPUT_CHANNEL;

    riscv_nmsis_nn_status result =
        riscv_maximum_s8(&ctx, input_1_data, &input_1_dims, input_2_data, &input_2_dims, output, &output_dims);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    int dst_size = MAXIMUM_SCALAR_2_INT8_DST_SIZE;
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate(output, output_ref_data, dst_size));
}

void maximum_no_broadcast_int8(void)
{
    nmsis_nn_context ctx;
    int8_t output[MAXIMUM_NO_BROADCAST_INT8_DST_SIZE] = {0};

    int buf_size = 0;
    ctx.buf = malloc(buf_size);
    ctx.size = 0;

    nmsis_nn_dims input_1_dims;
    nmsis_nn_dims input_2_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_1_data = maximum_no_broadcast_int8_input_tensor_1;
    const int8_t *input_2_data = maximum_no_broadcast_int8_input_tensor_2;
    const int8_t *output_ref_data = maximum_no_broadcast_int8_output;

    input_1_dims.n = MAXIMUM_NO_BROADCAST_INT8_BATCH_1;
    input_1_dims.h = MAXIMUM_NO_BROADCAST_INT8_HEIGHT_1;
    input_1_dims.w = MAXIMUM_NO_BROADCAST_INT8_WIDTH_1;
    input_1_dims.c = MAXIMUM_NO_BROADCAST_INT8_CHANNEL_1;

    input_2_dims.n = MAXIMUM_NO_BROADCAST_INT8_BATCH_2;
    input_2_dims.h = MAXIMUM_NO_BROADCAST_INT8_HEIGHT_2;
    input_2_dims.w = MAXIMUM_NO_BROADCAST_INT8_WIDTH_2;
    input_2_dims.c = MAXIMUM_NO_BROADCAST_INT8_CHANNEL_2;

    output_dims.n = MAXIMUM_NO_BROADCAST_INT8_OUTPUT_BATCH;
    output_dims.h = MAXIMUM_NO_BROADCAST_INT8_OUTPUT_HEIGHT;
    output_dims.w = MAXIMUM_NO_BROADCAST_INT8_OUTPUT_WIDTH;
    output_dims.c = MAXIMUM_NO_BROADCAST_INT8_OUTPUT_CHANNEL;

    riscv_nmsis_nn_status result =
        riscv_maximum_s8(&ctx, input_1_data, &input_1_dims, input_2_data, &input_2_dims, output, &output_dims);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    int dst_size = MAXIMUM_NO_BROADCAST_INT8_DST_SIZE;
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate(output, output_ref_data, dst_size));
}

void maximum_broadcast_batch_int8(void)
{
    nmsis_nn_context ctx;
    int8_t output[MAXIMUM_BROADCAST_BATCH_INT8_DST_SIZE] = {0};

    int buf_size = 0;
    ctx.buf = malloc(buf_size);
    ctx.size = 0;

    nmsis_nn_dims input_1_dims;
    nmsis_nn_dims input_2_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_1_data = maximum_broadcast_batch_int8_input_tensor_1;
    const int8_t *input_2_data = maximum_broadcast_batch_int8_input_tensor_2;
    const int8_t *output_ref_data = maximum_broadcast_batch_int8_output;

    input_1_dims.n = MAXIMUM_BROADCAST_BATCH_INT8_BATCH_1;
    input_1_dims.h = MAXIMUM_BROADCAST_BATCH_INT8_HEIGHT_1;
    input_1_dims.w = MAXIMUM_BROADCAST_BATCH_INT8_WIDTH_1;
    input_1_dims.c = MAXIMUM_BROADCAST_BATCH_INT8_CHANNEL_1;

    input_2_dims.n = MAXIMUM_BROADCAST_BATCH_INT8_BATCH_2;
    input_2_dims.h = MAXIMUM_BROADCAST_BATCH_INT8_HEIGHT_2;
    input_2_dims.w = MAXIMUM_BROADCAST_BATCH_INT8_WIDTH_2;
    input_2_dims.c = MAXIMUM_BROADCAST_BATCH_INT8_CHANNEL_2;

    output_dims.n = MAXIMUM_BROADCAST_BATCH_INT8_OUTPUT_BATCH;
    output_dims.h = MAXIMUM_BROADCAST_BATCH_INT8_OUTPUT_HEIGHT;
    output_dims.w = MAXIMUM_BROADCAST_BATCH_INT8_OUTPUT_WIDTH;
    output_dims.c = MAXIMUM_BROADCAST_BATCH_INT8_OUTPUT_CHANNEL;

    riscv_nmsis_nn_status result =
        riscv_maximum_s8(&ctx, input_1_data, &input_1_dims, input_2_data, &input_2_dims, output, &output_dims);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    int dst_size = MAXIMUM_BROADCAST_BATCH_INT8_DST_SIZE;
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate(output, output_ref_data, dst_size));
}

void maximum_broadcast_height_int8(void)
{
    nmsis_nn_context ctx;
    int8_t output[MAXIMUM_BROADCAST_HEIGHT_INT8_DST_SIZE] = {0};

    int buf_size = 0;
    ctx.buf = malloc(buf_size);
    ctx.size = 0;

    nmsis_nn_dims input_1_dims;
    nmsis_nn_dims input_2_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_1_data = maximum_broadcast_height_int8_input_tensor_1;
    const int8_t *input_2_data = maximum_broadcast_height_int8_input_tensor_2;
    const int8_t *output_ref_data = maximum_broadcast_height_int8_output;

    input_1_dims.n = MAXIMUM_BROADCAST_HEIGHT_INT8_BATCH_1;
    input_1_dims.h = MAXIMUM_BROADCAST_HEIGHT_INT8_HEIGHT_1;
    input_1_dims.w = MAXIMUM_BROADCAST_HEIGHT_INT8_WIDTH_1;
    input_1_dims.c = MAXIMUM_BROADCAST_HEIGHT_INT8_CHANNEL_1;

    input_2_dims.n = MAXIMUM_BROADCAST_HEIGHT_INT8_BATCH_2;
    input_2_dims.h = MAXIMUM_BROADCAST_HEIGHT_INT8_HEIGHT_2;
    input_2_dims.w = MAXIMUM_BROADCAST_HEIGHT_INT8_WIDTH_2;
    input_2_dims.c = MAXIMUM_BROADCAST_HEIGHT_INT8_CHANNEL_2;

    output_dims.n = MAXIMUM_BROADCAST_HEIGHT_INT8_OUTPUT_BATCH;
    output_dims.h = MAXIMUM_BROADCAST_HEIGHT_INT8_OUTPUT_HEIGHT;
    output_dims.w = MAXIMUM_BROADCAST_HEIGHT_INT8_OUTPUT_WIDTH;
    output_dims.c = MAXIMUM_BROADCAST_HEIGHT_INT8_OUTPUT_CHANNEL;

    riscv_nmsis_nn_status result =
        riscv_maximum_s8(&ctx, input_1_data, &input_1_dims, input_2_data, &input_2_dims, output, &output_dims);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    int dst_size = MAXIMUM_BROADCAST_HEIGHT_INT8_DST_SIZE;
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate(output, output_ref_data, dst_size));
}

void maximum_broadcast_width_int8(void)
{
    nmsis_nn_context ctx;

    int8_t output[MAXIMUM_BROADCAST_WIDTH_INT8_DST_SIZE] = {0};

    int buf_size = 0;
    ctx.buf = malloc(buf_size);
    ctx.size = 0;

    nmsis_nn_dims input_1_dims;
    nmsis_nn_dims input_2_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_1_data = maximum_broadcast_width_int8_input_tensor_1;
    const int8_t *input_2_data = maximum_broadcast_width_int8_input_tensor_2;
    const int8_t *output_ref_data = maximum_broadcast_width_int8_output;

    input_1_dims.n = MAXIMUM_BROADCAST_WIDTH_INT8_BATCH_1;
    input_1_dims.h = MAXIMUM_BROADCAST_WIDTH_INT8_HEIGHT_1;
    input_1_dims.w = MAXIMUM_BROADCAST_WIDTH_INT8_WIDTH_1;
    input_1_dims.c = MAXIMUM_BROADCAST_WIDTH_INT8_CHANNEL_1;

    input_2_dims.n = MAXIMUM_BROADCAST_WIDTH_INT8_BATCH_2;
    input_2_dims.h = MAXIMUM_BROADCAST_WIDTH_INT8_HEIGHT_2;
    input_2_dims.w = MAXIMUM_BROADCAST_WIDTH_INT8_WIDTH_2;
    input_2_dims.c = MAXIMUM_BROADCAST_WIDTH_INT8_CHANNEL_2;

    output_dims.n = MAXIMUM_BROADCAST_WIDTH_INT8_OUTPUT_BATCH;
    output_dims.h = MAXIMUM_BROADCAST_WIDTH_INT8_OUTPUT_HEIGHT;
    output_dims.w = MAXIMUM_BROADCAST_WIDTH_INT8_OUTPUT_WIDTH;
    output_dims.c = MAXIMUM_BROADCAST_WIDTH_INT8_OUTPUT_CHANNEL;

    riscv_nmsis_nn_status result =
        riscv_maximum_s8(&ctx, input_1_data, &input_1_dims, input_2_data, &input_2_dims, output, &output_dims);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    int dst_size = MAXIMUM_BROADCAST_WIDTH_INT8_DST_SIZE;
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate(output, output_ref_data, dst_size));
}

void maximum_broadcast_ch_int8(void)
{
    nmsis_nn_context ctx;

    int8_t output[MAXIMUM_BROADCAST_CH_INT8_DST_SIZE] = {0};

    int buf_size = 0;
    ctx.buf = malloc(buf_size);
    ctx.size = 0;

    nmsis_nn_dims input_1_dims;
    nmsis_nn_dims input_2_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_1_data = maximum_broadcast_ch_int8_input_tensor_1;
    const int8_t *input_2_data = maximum_broadcast_ch_int8_input_tensor_2;
    const int8_t *output_ref_data = maximum_broadcast_ch_int8_output;

    input_1_dims.n = MAXIMUM_BROADCAST_CH_INT8_BATCH_1;
    input_1_dims.h = MAXIMUM_BROADCAST_CH_INT8_HEIGHT_1;
    input_1_dims.w = MAXIMUM_BROADCAST_CH_INT8_WIDTH_1;
    input_1_dims.c = MAXIMUM_BROADCAST_CH_INT8_CHANNEL_1;

    input_2_dims.n = MAXIMUM_BROADCAST_CH_INT8_BATCH_2;
    input_2_dims.h = MAXIMUM_BROADCAST_CH_INT8_HEIGHT_2;
    input_2_dims.w = MAXIMUM_BROADCAST_CH_INT8_WIDTH_2;
    input_2_dims.c = MAXIMUM_BROADCAST_CH_INT8_CHANNEL_2;

    output_dims.n = MAXIMUM_BROADCAST_CH_INT8_OUTPUT_BATCH;
    output_dims.h = MAXIMUM_BROADCAST_CH_INT8_OUTPUT_HEIGHT;
    output_dims.w = MAXIMUM_BROADCAST_CH_INT8_OUTPUT_WIDTH;
    output_dims.c = MAXIMUM_BROADCAST_CH_INT8_OUTPUT_CHANNEL;

    riscv_nmsis_nn_status result =
        riscv_maximum_s8(&ctx, input_1_data, &input_1_dims, input_2_data, &input_2_dims, output, &output_dims);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    int dst_size = MAXIMUM_BROADCAST_CH_INT8_DST_SIZE;
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate(output, output_ref_data, dst_size));
}