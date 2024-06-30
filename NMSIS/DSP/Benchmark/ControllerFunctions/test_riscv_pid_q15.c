#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ControllerFunctions/pid_q15/test_data.h"

BENCH_DECLARE_VAR();

void pid_riscv_pid_q15(void)
{

    volatile int i = 0;

    q15_t pid_q15_output[ARRAY_SIZE_Q15];
    riscv_pid_instance_f32 PIDS;
    riscv_pid_instance_q15 PIDS_q15;
    PIDS.Kp = (float32_t)rand() / RAND_MAX;
    PIDS.Ki = (float32_t)rand() / RAND_MAX;
    PIDS.Kd = (float32_t)rand() / RAND_MAX;
    riscv_float_to_q15(&PIDS.Kp, &PIDS_q15.Kp, 1);
    riscv_float_to_q15(&PIDS.Ki, &PIDS_q15.Ki, 1);
    riscv_float_to_q15(&PIDS.Kd, &PIDS_q15.Kd, 1);
    q15_t target, ival, ee = 0;
    pid_q15_output[0] = 0;

    /* Target value*/
    target = (q15_t)(rand() % 0x7fff);
    /* Inital value */
    ival = 0;
    /* Initial value and target value error */
    ee = target - ival;
    /* Initial DSP PID controller function*/
    riscv_pid_init_q15(&PIDS_q15, 0);
    BENCH_START(riscv_pid_q15);
    for (i = 1; i < ARRAY_SIZE_Q15; i++) {
        pid_q15_output[i] = riscv_pid_q15(&PIDS_q15, ee);
        ee = target - pid_q15_output[i - 1];
    }
    BENCH_END(riscv_pid_q15);
}
