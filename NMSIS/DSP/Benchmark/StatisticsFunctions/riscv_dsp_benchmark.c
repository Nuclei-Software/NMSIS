#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start StatisticsFunctions benchmark test:\n");

#if defined (RISCV_FLOAT16_SUPPORTED)
    absmax_riscv_absmax_f16();
    absmaxNoIdx_riscv_absmax_no_idx_f16();
    absmin_riscv_absmin_f16();
    absminNoIdx_riscv_absmin_no_idx_f16();
    entropy_riscv_entropy_f16();
    kullbackLeibler_riscv_kullback_leibler_f16();
    logsumexpDotProd_riscv_logsumexp_dot_prod_f32();
    logsumexp_riscv_logsumexp_f16();
    max_riscv_max_f16();
    maxNoIdx_riscv_max_no_idx_f16();
    mean_riscv_mean_f16();
    min_riscv_min_f16();
    minNoIdx_riscv_min_no_idx_f16();
    mse_riscv_mse_f16();
    power_riscv_power_f16();
    rms_riscv_rms_f16();
    std_riscv_std_f16();
    var_riscv_var_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    absmax_riscv_absmax_f32();
    absmax_riscv_absmax_q7();
    absmax_riscv_absmax_q15();
    absmax_riscv_absmax_q31();

    absmaxNoIdx_riscv_absmax_no_idx_f32();
    absmaxNoIdx_riscv_absmax_no_idx_q7();
    absmaxNoIdx_riscv_absmax_no_idx_q15();
    absmaxNoIdx_riscv_absmax_no_idx_q31();

    absmin_riscv_absmin_f32();
    absmin_riscv_absmin_q7();
    absmin_riscv_absmin_q15();
    absmin_riscv_absmin_q31();

    absminNoIdx_riscv_absmin_no_idx_f32();
    absminNoIdx_riscv_absmin_no_idx_q7();
    absminNoIdx_riscv_absmin_no_idx_q15();
    absminNoIdx_riscv_absmin_no_idx_q31();

    entropy_riscv_entropy_f32();

    kullbackLeibler_riscv_kullback_leibler_f32();

    logsumexpDotProd_riscv_logsumexp_dot_prod_f32();

    logsumexp_riscv_logsumexp_f32();

    max_riscv_max_f32();
    max_riscv_max_q7();
    max_riscv_max_q15();
    max_riscv_max_q31();

    maxNoIdx_riscv_max_no_idx_f32();
    maxNoIdx_riscv_max_no_idx_q7();
    maxNoIdx_riscv_max_no_idx_q15();
    maxNoIdx_riscv_max_no_idx_q31();

    mean_riscv_mean_f32();
    mean_riscv_mean_q7();
    mean_riscv_mean_q15();
    mean_riscv_mean_q31();

    min_riscv_min_f32();
    min_riscv_min_q7();
    min_riscv_min_q15();
    min_riscv_min_q31();

    minNoIdx_riscv_min_no_idx_f32();
    minNoIdx_riscv_min_no_idx_q7();
    minNoIdx_riscv_min_no_idx_q15();
    minNoIdx_riscv_min_no_idx_q31();

    mse_riscv_mse_f32();
    mse_riscv_mse_q7();
    mse_riscv_mse_q15();
    mse_riscv_mse_q31();

    power_riscv_power_f32();
    power_riscv_power_q7();
    power_riscv_power_q15();
    power_riscv_power_q31();

    rms_riscv_rms_f32();
    rms_riscv_rms_q15();
    rms_riscv_rms_q31();

    std_riscv_std_f32();
    std_riscv_std_q15();
    std_riscv_std_q31();

    var_riscv_var_f32();
    var_riscv_var_q15();
    var_riscv_var_q31();

#ifdef F64
    absmax_riscv_absmax_f64();
    absmaxNoIdx_riscv_absmax_no_idx_f64();
    absmin_riscv_absmin_f64();
    absminNoIdx_riscv_absmin_no_idx_f64();
    entropy_riscv_entropy_f64();
    kullbackLeibler_riscv_kullback_leibler_f64();
    max_riscv_max_f64();
    maxNoIdx_riscv_max_no_idx_f64();
    min_riscv_min_f64();
    minNoIdx_riscv_min_no_idx_f64
    mean_riscv_mean_f64();
    mse_riscv_mse_f64();
    power_riscv_power_f64();
    std_riscv_std_f64();
    var_riscv_var_f64();
#endif
    printf("test for StatisticsFunctions benchmark finished.\n");
}
