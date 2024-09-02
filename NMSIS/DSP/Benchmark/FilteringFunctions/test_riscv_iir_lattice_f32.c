#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/iir_lattice_f32/test_data.h"

BENCH_DECLARE_VAR();

void iirLattice_riscv_iir_lattice_f32(void)
{
    float32_t iir_lattice_f32_output[TEST_LENGTH_SAMPLES];
    float32_t IIRStateF32[TEST_LENGTH_SAMPLES + numStages];

    /* clang-format off */
    riscv_iir_lattice_instance_f32 S;
    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    /* clang-format on */
    riscv_iir_lattice_init_f32(&S, numStages, IIRCoeffs32LP_K, IIRCoeffs32LP_V, IIRStateF32, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_iir_lattice_f32);
    riscv_iir_lattice_f32(&S, testInput_f32_50Hz_200Hz, iir_lattice_f32_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_iir_lattice_f32);
}