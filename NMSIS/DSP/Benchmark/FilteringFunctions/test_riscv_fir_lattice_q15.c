#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_lattice_q15/test_data.h"

BENCH_DECLARE_VAR();

void firLattice_riscv_fir_lattice_q15(void)
{
    q15_t firStateq15[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    q15_t lattice_q15_output[TEST_LENGTH_SAMPLES];

    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_q15(firCoeffs32LP_q15, TEST_LENGTH_SAMPLES);
    /* clang-format off */
    riscv_fir_lattice_instance_q15 S;
    /* clang-format on */
    riscv_fir_lattice_init_q15(&S, NUM_TAPS, firCoeffs32LP_q15, firStateq15);
    BENCH_START(riscv_fir_lattice_q15);
    riscv_fir_lattice_q15(&S, testInput_q15_50Hz_200Hz, lattice_q15_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_lattice_q15);
}