#if defined (RISCV_FLOAT16_SUPPORTED)
extern void svm_riscv_svm_linear_predict_f16();
extern void svm_riscv_svm_polynomial_predict_f16();
extern void svm_riscv_svm_rbf_predict_f16();
extern void svm_riscv_svm_sigmoid_predict_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

extern void svm_riscv_svm_linear_predict_f32();
extern void svm_riscv_svm_polynomial_predict_f32();
extern void svm_riscv_svm_rbf_predict_f32();
extern void svm_riscv_svm_sigmoid_predict_f32();