#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_lattice_q31/test_data.h"

BENCH_DECLARE_VAR();

void firLattice_riscv_fir_lattice_q31(void)
{
    q31_t firStateq31[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    q31_t lattice_q31_output[TEST_LENGTH_SAMPLES];

    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_q31(firCoeffs32LP_q31, TEST_LENGTH_SAMPLES);
    /* clang-format off */
    riscv_fir_lattice_instance_q31 S;
    /* clang-format on */
    riscv_fir_lattice_init_q31(&S, NUM_TAPS, firCoeffs32LP_q31, firStateq31);
    BENCH_START(riscv_fir_lattice_q31);
    riscv_fir_lattice_q31(&S, testInput_q31_50Hz_200Hz, lattice_q31_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_lattice_q31);
}