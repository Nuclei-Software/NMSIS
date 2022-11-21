#include "riscv_math.h"
#include <stdint.h>
#include <stdlib.h>
#include "array.h"
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#define ARRAYA_SIZE_F32 10

float32_t f32_a_array[ARRAYA_SIZE_F32];
float32_t f32_b_array[ARRAYA_SIZE_F32];

int test_flag_error = 0;
int8_t s;
BENCH_DECLARE_VAR();

int main(void)
{
    int i;
    float32_t f32_output, f32_output_ref;

    BENCH_INIT();
    generate_rand_f32(f32_a_array, ARRAYA_SIZE_F32);
    generate_rand_f32(f32_b_array, ARRAYA_SIZE_F32);

    //1-2( sun(min(Sai,Sbi)) / (sum(Sai)+sum(Sbi)) )
    BENCH_START(riscv_braycurtis_distance_f32);
    f32_output = riscv_braycurtis_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    BENCH_END(riscv_braycurtis_distance_f32);
    f32_output_ref = ref_braycurtis_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_braycurtis_distance_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_braycurtis_distance_f32);

    //sum(|pi-qi|/(|pi|+|qi|))
    BENCH_START(riscv_canberra_distance_f32);
    f32_output = riscv_canberra_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    BENCH_END(riscv_canberra_distance_f32);
    f32_output_ref = ref_canberra_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_canberra_distance_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_canberra_distance_f32);

    //x12=max(|x1i-x2i|)
    BENCH_START(riscv_chebyshev_distance_f32);
    f32_output = riscv_chebyshev_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    BENCH_END(riscv_chebyshev_distance_f32);
    f32_output_ref = ref_chebyshev_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_chebyshev_distance_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_chebyshev_distance_f32);

    //also known as Manhattan Distance
    //x12=sum(|x1i-x2i|)
    BENCH_START(riscv_cityblock_distance_f32);
    f32_output = riscv_cityblock_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    BENCH_END(riscv_cityblock_distance_f32);
    f32_output_ref = ref_cityblock_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_cityblock_distance_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cityblock_distance_f32);

    //cos(sita)=a.b/|a||b|=sum(x1i*x2i)/ ( sum(abs(x1i)) * sum(abs(x1i)) )
    //actually it outputs 1-cos(sita)
    BENCH_START(riscv_cosine_distance_f32);
    f32_output = riscv_cosine_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    BENCH_END(riscv_cosine_distance_f32);
    f32_output_ref = ref_cosine_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_cosine_distance_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cosine_distance_f32);

    //sqrt(sum(square(x1i-x2i)))
    BENCH_START(riscv_euclidean_distance_f32);
    f32_output = riscv_euclidean_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    BENCH_END(riscv_euclidean_distance_f32);
    f32_output_ref = ref_euclidean_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_euclidean_distance_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_euclidean_distance_f32);

    //sqrt(sum(square(x1i-x2i)))
    BENCH_START(riscv_jensenshannon_distance_f32);
    f32_output = riscv_jensenshannon_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    BENCH_END(riscv_jensenshannon_distance_f32);
    f32_output_ref = ref_jensenshannon_distance_f32(f32_a_array, f32_b_array, ARRAYA_SIZE_F32);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_jensenshannon_distance_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_jensenshannon_distance_f32);

    //(sum(|xi-yi|p))1/p
    //p and 1/p stands for power p and power 1/p
    BENCH_START(riscv_minkowski_distance_f32);
    f32_output = riscv_minkowski_distance_f32(f32_a_array, f32_b_array, 3, ARRAYA_SIZE_F32);
    BENCH_END(riscv_minkowski_distance_f32);
    f32_output_ref = ref_minkowski_distance_f32(f32_a_array, f32_b_array, 3, ARRAYA_SIZE_F32);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_minkowski_distance_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_minkowski_distance_f32);

    f32_output_ref =  0.282051282051282;
    //1.0*(ctf + cft) / (2.0*ctt + cft + ctf)
    //t stands for true,f stands for false
    BENCH_START(riscv_dice_distance);
    f32_output = riscv_dice_distance(u32_dice_a_array, u32_dice_b_array, 36);
    BENCH_END(riscv_dice_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_dice_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_dice_distance);

    f32_output_ref =  0.583333333333333;
    //1.0*(ctf + cft) / numberOfBools
    BENCH_START(riscv_hamming_distance);
    f32_output = riscv_hamming_distance(u32_hamming_a_array, u32_hamming_b_array, 36);
    BENCH_END(riscv_hamming_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_hamming_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_hamming_distance);

    f32_output_ref =  0.769230769230769;
    //1.0*(ctf + cft) / (ctt + cft + ctf)
    BENCH_START(riscv_jaccard_distance);
    f32_output = riscv_jaccard_distance(u32_jaccard_a_array, u32_jaccard_b_array, 36);
    BENCH_END(riscv_jaccard_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_jaccard_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_jaccard_distance);

    f32_output_ref =  0.894736842105263;
    //1.0*(ctf + cft - ctt + numberOfBools) / (cft + ctf + numberOfBools)
    BENCH_START(riscv_kulsinski_distance);
    f32_output = riscv_kulsinski_distance(u32_kulsinski_a_array, u32_kulsinski_b_array, 36);
    BENCH_END(riscv_kulsinski_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_kulsinski_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_kulsinski_distance);

    f32_output_ref =  0.615384615384615;
    //r = 2*(ctf + cft)
    //return(1.0*r / (r + ctt + cff))
    BENCH_START(riscv_rogerstanimoto_distance);
    f32_output = riscv_rogerstanimoto_distance(u32_rogerstanimoto_a_array, u32_rogerstanimoto_b_array, 36);
    BENCH_END(riscv_rogerstanimoto_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_rogerstanimoto_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_rogerstanimoto_distance);

    f32_output_ref =  0.833333333333333;
    //1.0*(numberOfBools - ctt) / ((float32_t)numberOfBools)
    BENCH_START(riscv_russellrao_distance);
    f32_output = riscv_russellrao_distance(u32_russellrao_a_array, u32_russellrao_b_array, 36);
    BENCH_END(riscv_russellrao_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_russellrao_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_russellrao_distance);

    f32_output_ref =  0.714285714285714;
    //r = 2.0*(ctf + cft);
    //s = 1.0*(cff + ctt);
    //r / (s+r)
    BENCH_START(riscv_sokalmichener_distance);
    f32_output = riscv_sokalmichener_distance(u32_sokalmichener_a_array, u32_sokalmichener_b_array, 36);
    BENCH_END(riscv_sokalmichener_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_sokalmichener_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sokalmichener_distance);

    f32_output_ref =  0.790697674418605;
    //r = 2.0*(ctf + cft);
    //r / (r + ctt)
    BENCH_START(riscv_sokalsneath_distance);
    f32_output = riscv_sokalsneath_distance(u32_sokalsneath_a_array, u32_sokalsneath_b_array, 36);
    BENCH_END(riscv_sokalsneath_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_sokalsneath_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sokalsneath_distance);

    f32_output_ref =  1.26760563380282;
    //r = 2*(ctf * cft);
    //1.0*r / (r/2.0 + ctt * cff)
    BENCH_START(riscv_yule_distance);
    f32_output = riscv_yule_distance(u32_yule_a_array, u32_yule_b_array, 36);
    BENCH_END(riscv_yule_distance);
    s = verify_results_f32(&f32_output_ref, &f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_yule_distance);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_yule_distance);

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
};
