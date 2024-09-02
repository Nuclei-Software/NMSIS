#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ControllerFunctions/pid_f32/test_data.h"

BENCH_DECLARE_VAR();

void pid_riscv_pid_f32(void)
{

    volatile int i = 0;

    float32_t pid_f32_output[ARRAY_SIZE_F32];
    riscv_pid_instance_f32 PIDS;
    PIDS.Kp = (float32_t)rand() / RAND_MAX;
    PIDS.Ki = (float32_t)rand() / RAND_MAX;
    PIDS.Kd = (float32_t)rand() / RAND_MAX;
    float32_t target, ival, ee = 0;
    pid_f32_output[0] = 0;

    /* Target value*/
    target = (float32_t)rand() / 0x7fff;
    /* Inital value */
    ival = 0;
    /* Initial value and target value error */
    ee = target - ival;
    /* Initial DSP PID controller function*/
    riscv_pid_init_f32(&PIDS, 1);
    BENCH_START(riscv_pid_f32);
    for (i = 1; i < ARRAY_SIZE_F32; i++) {
        pid_f32_output[i] = riscv_pid_f32(&PIDS, ee);
        ee = target - pid_f32_output[i - 1];
    }
    BENCH_END(riscv_pid_f32);
}