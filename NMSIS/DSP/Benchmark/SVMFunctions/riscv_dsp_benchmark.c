#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start SVMFunctions benchmark test:\n");

    svm_riscv_svm_linear_predict_f16();
    svm_riscv_svm_linear_predict_f32();

    svm_riscv_svm_polynomial_predict_f16();
    svm_riscv_svm_polynomial_predict_f32();

    svm_riscv_svm_rbf_predict_f16();
    svm_riscv_svm_rbf_predict_f32();

    svm_riscv_svm_sigmoid_predict_f16();
    svm_riscv_svm_sigmoid_predict_f32();

    printf("All tests are passed.\n");
    printf("test for SVMFunctions benchmark finished.\n");
}
