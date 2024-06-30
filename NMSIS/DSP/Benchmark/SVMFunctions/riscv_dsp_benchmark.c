#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start SVMFunctions benchmark test:\n");

#if defined (RISCV_FLOAT16_SUPPORTED)
    svm_riscv_svm_linear_predict_f16();
    svm_riscv_svm_polynomial_predict_f16();
    svm_riscv_svm_rbf_predict_f16();
    svm_riscv_svm_sigmoid_predict_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    svm_riscv_svm_linear_predict_f32();
    svm_riscv_svm_polynomial_predict_f32();
    svm_riscv_svm_rbf_predict_f32();
    svm_riscv_svm_sigmoid_predict_f32();

    printf("test for SVMFunctions benchmark finished.\n");
}
