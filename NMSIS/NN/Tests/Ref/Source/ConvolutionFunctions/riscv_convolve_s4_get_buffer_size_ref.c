#include "ref_functions.h"

int32_t riscv_convolve_s4_get_buffer_size_ref(const nmsis_nn_dims *input_dims, const nmsis_nn_dims *filter_dims)
{
    const int32_t rhs_cols = filter_dims->w * filter_dims->h * input_dims->c;
    return (2 * rhs_cols) * (int32_t)sizeof(int16_t);
}
