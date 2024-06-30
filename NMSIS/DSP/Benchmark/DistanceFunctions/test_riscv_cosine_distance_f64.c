#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/DistanceFunctions/cosine_distance_f64/test_data.h"

BENCH_DECLARE_VAR();

void cosine_distance_riscv_cosine_distance_f64(void)
{
    float64_t cosine_distance_f64_output;

    generate_rand_f64(cosine_distance_f64_input1, ARRAY_SIZE_F64);
    generate_rand_f64(cosine_distance_f64_input2, ARRAY_SIZE_F64);

    BENCH_START(riscv_cosine_distance_f64);
    cosine_distance_f64_output = riscv_cosine_distance_f64(cosine_distance_f64_input1, cosine_distance_f64_input2, ARRAY_SIZE_F64);
    BENCH_END(riscv_cosine_distance_f64);

    return;
}