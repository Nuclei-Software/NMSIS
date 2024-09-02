#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_lattice_f32/test_data.h"

BENCH_DECLARE_VAR();

void firLattice_riscv_fir_lattice_f32(void)
{
    float32_t firStatef32[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    float32_t lattice_f32_output[TEST_LENGTH_SAMPLES];

    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_f32(firCoeffs32LP, TEST_LENGTH_SAMPLES);
    /* clang-format off */
    riscv_fir_lattice_instance_f32 S;
    /* clang-format on */
    riscv_fir_lattice_init_f32(&S, NUM_TAPS, firCoeffs32LP, firStatef32);
    BENCH_START(riscv_fir_lattice_f32);
    riscv_fir_lattice_f32(&S, testInput_f32_50Hz_200Hz, lattice_f32_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_lattice_f32);
}