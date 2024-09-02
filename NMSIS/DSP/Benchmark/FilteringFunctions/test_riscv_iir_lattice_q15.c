#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/iir_lattice_q15/test_data.h"

BENCH_DECLARE_VAR();

void iirLattice_riscv_iir_lattice_q15(void)
{
    q15_t iir_lattice_q15_output[TEST_LENGTH_SAMPLES];
    q15_t IIRStateQ15[TEST_LENGTH_SAMPLES + numStages];

    /* clang-format off */
    riscv_iir_lattice_instance_q15 S;
    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    /* clang-format on */
    riscv_iir_lattice_init_q15(&S, numStages, IIRCoeffs32LP_K_q15, IIRCoeffs32LP_V_q15, IIRStateQ15, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_iir_lattice_q15);
    riscv_iir_lattice_q15(&S, testInput_q15_50Hz_200Hz, iir_lattice_q15_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_iir_lattice_q15);
}
