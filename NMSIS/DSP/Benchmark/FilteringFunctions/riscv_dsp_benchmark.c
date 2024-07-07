#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

#define Test_IIRdf1
#define Test_IIRdf2t
#define Test_Conv
#define Test_Correlate
#define Test_ConvPartial
#define Test_FIR
#define Test_FIRDecimate
#define Test_FIRInterpolate
#define Test_FIRLattice
#define Test_FIRSparse
#define Test_IIRLatice
#define Test_LevinsonDurbin
#define Test_LMS
#define Test_NLMS

int main()
{
    printf("Start FilteringFunctions benchmark test:\n");

#ifdef Test_IIRdf1
#if defined (RISCV_FLOAT16_SUPPORTED)
    df1_riscv_biquad_cascade_df1_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    df1_riscv_biquad_cascade_df1_f32();
    df1_riscv_biquad_cascade_df1_q15();
    df1_riscv_biquad_cascade_df1_q31();
    df1_riscv_biquad_cascade_df1_fast_q15();
    df1_riscv_biquad_cascade_df1_fast_q31();
    df1_riscv_biquad_cas_df1_32x64_q31();
#endif

#ifdef Test_IIRdf2t
#if defined (RISCV_FLOAT16_SUPPORTED)
    df2T_riscv_biquad_cascade_df2T_f16();
    df2T_riscv_biquad_cascade_stereo_df2T_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    df2T_riscv_biquad_cascade_df2T_f32();
    df2T_riscv_biquad_cascade_stereo_df2T_f32();
#endif

#ifdef Test_Conv
    conv_riscv_conv_f32();
    conv_riscv_conv_q7();
    conv_riscv_conv_q15();
    conv_riscv_conv_q31();
    conv_riscv_conv_fast_q15();
    conv_riscv_conv_fast_q31();
    conv_riscv_conv_opt_q7();
    conv_riscv_conv_opt_q15();
    conv_iscv_conv_fast_opt_q15();
#endif

#ifdef Test_Correlate
#if defined (RISCV_FLOAT16_SUPPORTED)
    correlate_riscv_correlate_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    correlate_riscv_correlate_f32();
    correlate_riscv_correlate_q7();
    correlate_riscv_correlate_q15();
    correlate_riscv_correlate_q31();
    correlate_riscv_correlate_fast_q15();
    correlate_riscv_correlate_fast_q31();
    correlate_riscv_correlate_opt_q7();
    correlate_riscv_correlate_opt_q15();
    correlate_riscv_correlate_fast_opt_q15();
#endif

#ifdef Test_ConvPartial
    convPartial_riscv_conv_partial_f32();
    convPartial_riscv_conv_partial_q7();
    convPartial_riscv_conv_partial_q15();
    convPartial_riscv_conv_partial_q31();
    convPartial_riscv_conv_partial_fast_q15();
    convPartial_riscv_conv_partial_fast_q31();
    convPartial_riscv_conv_partial_opt_q7();
    convPartial_riscv_conv_partial_opt_q15();
    convPartial_riscv_conv_partial_fast_opt_q15();
#endif

#ifdef Test_FIR
#if defined (RISCV_FLOAT16_SUPPORTED)
    fir_riscv_fir_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
    fir_riscv_fir_f32();
    fir_riscv_fir_q7();
    fir_riscv_fir_q15();
    fir_riscv_fir_q31();
    fir_riscv_fir_fast_q15();
    fir_riscv_fir_fast_q31();
#endif

#ifdef Test_FIRDecimate
    firDecimate_riscv_fir_decimate_f32();
    firDecimate_riscv_fir_decimate_q15();
    firDecimate_riscv_fir_decimate_q31();
    firDecimate_riscv_fir_decimate_fast_q15();
    firDecimate_riscv_fir_decimate_fast_q31();
#endif

#ifdef Test_FIRInterpolate
    firInterpolate_riscv_fir_interpolate_f32();
    firInterpolate_riscv_fir_interpolate_q15();
    firInterpolate_riscv_fir_interpolate_q31();
#endif

#ifdef Test_FIRLattice
    firLattice_riscv_fir_lattice_f32();
    firLattice_riscv_fir_lattice_q15();
    firLattice_riscv_fir_lattice_q31();
#endif

#ifdef Test_FIRSparse
    firSparse_riscv_fir_sparse_f32();
    firSparse_riscv_fir_sparse_q7();
    firSparse_riscv_fir_sparse_q15();
    firSparse_riscv_fir_sparse_q31();
#endif

#ifdef Test_IIRLatice
    iirLattice_riscv_iir_lattice_f32();
    iirLattice_riscv_iir_lattice_q15();
    iirLattice_riscv_iir_lattice_q31();
#endif

#ifdef Test_LevinsonDurbin
#if defined (RISCV_FLOAT16_SUPPORTED)
    levinsonDurbin_riscv_levinson_durbin_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    levinsonDurbin_riscv_levinson_durbin_f32();
    levinsonDurbin_riscv_levinson_durbin_q31();
#endif

#ifdef Test_LMS
    lms_riscv_lms_f32();
    lms_riscv_lms_q15();
    lms_riscv_lms_q31();
#endif

#ifdef Test_NLMS
    lmsNorm_riscv_lms_norm_f32();
    lmsNorm_riscv_lms_norm_q15();
    lmsNorm_riscv_lms_norm_q31();
#endif


#ifdef F64
    fir_riscv_fir_f64();
    df2T_riscv_biquad_cascade_df2T_f64();
    correlate_riscv_correlate_f64();
#endif

    printf("All tests are passed.\n");
    printf("test for FilteringFunctions benchmark finished.\n");
}
