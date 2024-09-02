#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start DistanceFunctions benchmark test:\n");

    braycurtis_distance_riscv_braycurtis_distance_f16();
    braycurtis_distance_riscv_braycurtis_distance_f32();

    canberra_distance_riscv_canberra_distance_f16();
    canberra_distance_riscv_canberra_distance_f32();

    chebyshev_distance_riscv_chebyshev_distance_f16();
    chebyshev_distance_riscv_chebyshev_distance_f32();
    chebyshev_distance_riscv_chebyshev_distance_f64();

    cityblock_distance_riscv_cityblock_distance_f16();
    cityblock_distance_riscv_cityblock_distance_f32();
    cityblock_distance_riscv_cityblock_distance_f64();

    correlation_distance_riscv_correlation_distance_f16();
    correlation_distance_riscv_correlation_distance_f32();

    cosine_distance_riscv_cosine_distance_f16();
    cosine_distance_riscv_cosine_distance_f32();
    cosine_distance_riscv_cosine_distance_f64();

    euclidean_distance_riscv_euclidean_distance_f16();
    euclidean_distance_riscv_euclidean_distance_f32();
    euclidean_distance_riscv_euclidean_distance_f64();

    jensenshannon_distance_riscv_jensenshannon_distance_f16();
    jensenshannon_distance_riscv_jensenshannon_distance_f32();

    minkowski_distance_riscv_minkowski_distance_f16();
    minkowski_distance_riscv_minkowski_distance_f32();

    dice_distance_riscv_dice_distance();
    hamming_distance_riscv_hamming_distance();
    jaccard_distance_riscv_jaccard_distance();
    kulsinski_distance_riscv_kulsinski_distance();
    rogerstanimoto_distance_riscv_rogerstanimoto_distance();
    russellrao_distance_riscv_russellrao_distance();
    sokalmichener_distance_riscv_sokalmichener_distance();
    sokalsneath_distance_riscv_sokalsneath_distance();
    yule_distance_riscv_yule_distance();

    printf("All tests are passed.\n");
    printf("test for DistanceFunctions benchmark finished.\n");
}
