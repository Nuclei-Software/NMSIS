#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/DistanceFunctions/chebyshev_distance_f64/test_data.h"

BENCH_DECLARE_VAR();

void chebyshev_distance_riscv_chebyshev_distance_f64(void)
{
#ifdef F64
    float64_t chebyshev_distance_f64_output;

    generate_rand_f64(chebyshev_distance_f64_input1, ARRAY_SIZE_F64);
    generate_rand_f64(chebyshev_distance_f64_input2, ARRAY_SIZE_F64);

    BENCH_START(riscv_chebyshev_distance_f64);
    chebyshev_distance_f64_output = riscv_chebyshev_distance_f64(chebyshev_distance_f64_input1, chebyshev_distance_f64_input2, ARRAY_SIZE_F64);
    BENCH_END(riscv_chebyshev_distance_f64);
#endif
}