#if defined (RISCV_FLOAT16_SUPPORTED)
extern void df1_riscv_biquad_cascade_df1_f16();
extern void df2T_riscv_biquad_cascade_df2T_f16();
extern void df2T_riscv_biquad_cascade_stereo_df2T_f16();
extern void df2T_riscv_biquad_cascade_df2T_f16();
extern void df2T_riscv_biquad_cascade_stereo_df2T_f16();
extern void fir_riscv_fir_f16();
extern void correlate_riscv_correlate_f16();
extern void levinsonDurbin_riscv_levinson_durbin_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

extern void df1_riscv_biquad_cas_df1_32x64_ins_q31();
extern void df1_riscv_biquad_cascade_df1_f32();
extern void df1_riscv_biquad_cascade_df1_q15();
extern void df1_riscv_biquad_cascade_df1_fast_q15();
extern void df1_riscv_biquad_cascade_df1_fast_q31();

extern void df2T_riscv_biquad_cascade_df2T_f32();
extern void df2T_riscv_biquad_cascade_stereo_df2T_f32();

extern void conv_riscv_conv_f32();
extern void conv_riscv_conv_q7();
extern void conv_riscv_conv_q15();
extern void conv_riscv_conv_q31();
extern void conv_riscv_conv_opt_q7();
extern void conv_riscv_conv_opt_q15();
extern void conv_riscv_conv_fast_q15();
extern void conv_riscv_conv_fast_q31();
extern void conv_iscv_conv_fast_opt_q15();

extern void convPartial_riscv_conv_partial_f32();
extern void convPartial_riscv_conv_partial_q7();
extern void convPartial_riscv_conv_partial_q15();
extern void convPartial_riscv_conv_partial_q31();
extern void convPartial_riscv_conv_partial_fast_q15();
extern void convPartial_riscv_conv_partial_fast_q31();
extern void convPartial_riscv_conv_partial_opt_q7();
extern void convPartial_riscv_conv_partial_opt_q15();
extern void convPartial_riscv_conv_partial_fast_opt_q15();

extern void correlate_riscv_correlate_f32();
extern void correlate_riscv_correlate_q7();
extern void correlate_riscv_correlate_q15();
extern void correlate_riscv_correlate_q31();
extern void correlate_riscv_correlate_fast_q15();
extern void correlate_riscv_correlate_fast_q31();
extern void correlate_riscv_correlate_opt_q7();
extern void correlate_riscv_correlate_opt_q15();
extern void correlate_riscv_correlate_fast_opt_q15();

extern void fir_riscv_fir_f32();
extern void fir_riscv_fir_q7();
extern void fir_riscv_fir_q15();
extern void fir_riscv_fir_q31();
extern void fir_riscv_fir_fast_q15();
extern void fir_riscv_fir_fast_q31();

extern void firDecimate_riscv_fir_decimate_f32();
extern void firDecimate_riscv_fir_decimate_q15();
extern void firDecimate_riscv_fir_decimate_q31();
extern void firDecimate_riscv_fir_decimate_fast_q15();
extern void firDecimate_riscv_fir_decimate_fast_q31();

extern void firInterpolate_riscv_fir_interpolate_f32();
extern void firInterpolate_riscv_fir_interpolate_q15();
extern void firInterpolate_riscv_fir_interpolate_q31();

extern void firLattice_riscv_fir_lattice_f32();
extern void firLattice_riscv_fir_lattice_q15();
extern void firLattice_riscv_fir_lattice_q31();

extern void firSparse_riscv_fir_sparse_f32();
extern void firSparse_riscv_fir_sparse_q7();
extern void firSparse_riscv_fir_sparse_q15();
extern void firSparse_riscv_fir_sparse_q31();

extern void iirLattice_riscv_iir_lattice_f32();
extern void iirLattice_riscv_iir_lattice_q15();
extern void iirLattice_riscv_iir_lattice_q31();

extern void levinsonDurbin_riscv_levinson_durbin_f32();
extern void levinsonDurbin_riscv_levinson_durbin_q31();

extern void lms_riscv_lms_f32();
extern void lms_riscv_lms_q15();
extern void lms_riscv_lms_q31();

extern void lmsNorm_riscv_lms_norm_f32();
extern void lmsNorm_riscv_lms_norm_q15();
extern void lmsNorm_riscv_lms_norm_q31();

#ifdef F64
extern void fir_riscv_fir_f64();
extern void df2T_riscv_biquad_cascade_df2T_f64();
extern void correlate_riscv_correlate_f64();
#endif
