#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/iir_lattice_q31/test_data.h"

BENCH_DECLARE_VAR();

void iirLattice_riscv_iir_lattice_q31(void)
{
    q31_t iir_lattice_q31_output[TEST_LENGTH_SAMPLES];
    q31_t IIRStateQ31[TEST_LENGTH_SAMPLES + numStages];

    /* clang-format off */
    riscv_iir_lattice_instance_q31 S;
    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    /* clang-format on */
    riscv_iir_lattice_init_q31(&S, numStages, IIRCoeffs32LP_K_q31, IIRCoeffs32LP_V_q31, IIRStateQ31, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_iir_lattice_q31);
    riscv_iir_lattice_q31(&S, testInput_q31_50Hz_200Hz, iir_lattice_q31_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_iir_lattice_q31);

    return;
}
