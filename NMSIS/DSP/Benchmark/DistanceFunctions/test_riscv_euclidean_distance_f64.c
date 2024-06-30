#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/DistanceFunctions/euclidean_distance_f64/test_data.h"

BENCH_DECLARE_VAR();

void euclidean_distance_riscv_euclidean_distance_f64(void)
{
    float64_t euclidean_distance_f64_output;

    generate_rand_f64(euclidean_distance_f64_input1, ARRAY_SIZE_F64);
    generate_rand_f64(euclidean_distance_f64_input2, ARRAY_SIZE_F64);

    BENCH_START(riscv_euclidean_distance_f64);
    euclidean_distance_f64_output = riscv_euclidean_distance_f64(euclidean_distance_f64_input1, euclidean_distance_f64_input2, ARRAY_SIZE_F64);
    BENCH_END(riscv_euclidean_distance_f64);

    return;
}